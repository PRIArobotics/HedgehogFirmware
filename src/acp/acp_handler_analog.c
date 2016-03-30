#include "acp_handler.h"
#include "acp_opcodes.h"
#include "acp_errorcodes.h"
#include "acp_handler_impl.h"
#include "ringbuffer.h"
#include "adc.h"


static uint8_t subscriptions = 0;


void acp_handler_analog_valueReq(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t buf[4];
	uint8_t len = 0;

	uint8_t port = ringbuffer_pop(conn.rxBuffer);
	if(port >= ANALOG_COUNT)
	{
		acp_error(conn, ERR_ANA_OOR, opcode);
		return;
	}
	uint16_t ana = adc_getAnalogInput(port);
	buf[len++] = ANALOG_SENSOR_REPLY;
	buf[len++] = port;
	buf[len++] = (uint8_t) (ana >> 8);
	buf[len++] = (uint8_t) (ana & 0xFF);

	ringbuffer_push_multiple(conn.txBuffer, buf, len);
}


void acp_handler_analog_valueSub(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	if(payloadLength > 32)
	{
		acp_error(conn, ERR_INVALID_PAYLOAD, opcode);
		return;
	}

	subscriptions = ringbuffer_pop(conn.rxBuffer);
	ringbuffer_consume(conn.rxBuffer, payloadLength - 1);
}


void acp_analog_sendSubUpdate(acp_conn_t conn)
{
	uint8_t buf[2 + 3 * ANALOG_COUNT];
	uint8_t len = 0, i;

	buf[len++] = ANALOG_SENSOR_UPDATE;
	len++;
	for(i = 0; i < ANALOG_COUNT; i++)
	{
		if((subscriptions >> i) & 0x01)
		{
			uint16_t value = adc_getAnalogInput(i);
			buf[len++] = i; //number of the used port
			buf[len++] = (uint8_t) (value >> 8); //MSB of the value
			buf[len++] = (uint8_t) (value & 0xFF); //LSB of the value
		}
	}
	buf[1] = len - 2; //minus opcode and VPL byte
	//only send if there were subscriptions
	if(buf[1] == 0) return;

	ringbuffer_push_multiple(conn.txBuffer, buf, len);
}


void acp_handler_analog_pullAct(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	if(payloadLength > 32)
	{
		acp_error(conn, ERR_INVALID_PAYLOAD, opcode);
		return;
	}

	uint8_t pullups = ringbuffer_pop(conn.rxBuffer);
	ringbuffer_consume(conn.rxBuffer, payloadLength - 1);

	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		adc_setPullup(i, (pullups >> i) & 0x01);
	}
}
