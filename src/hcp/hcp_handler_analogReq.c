#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "adc.h"


void hcp_handler_analogReq(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t port = ringbuffer_pop(conn.rxBuffer);

	if((port >= ANALOG_COUNT) && (port != 0x80))
	{
		ringbuffer_push(conn.txBuffer, HCP_INVALID_PORT);
		return;
	}

	uint16_t value;
	if(port == 0x80) value = adc_getBatteryVoltage_mV();
	else value = adc_getAnalogInput(port);

	ringbuffer_push(conn.txBuffer, HCP_ANALOG_REP);
	ringbuffer_push(conn.txBuffer, port);
	ringbuffer_push(conn.txBuffer, (uint8_t)(value >> 8)); //high byte
	ringbuffer_push(conn.txBuffer, (uint8_t)value); //low byte
}
