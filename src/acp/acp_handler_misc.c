#include "acp_handler.h"
#include "acp.h"
#include "acp_opcodes.h"
#include "acp_errorcodes.h"
#include "acp_handler_impl.h"
#include "battery.h"
#include "adc.h"


void acp_handler_nop(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
}


void acp_handler_HWC_offAct(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	//TODO
}


void acp_handler_HWC_resetAct(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	//TODO
}


void acp_handler_HWC_typeReq(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t buf[2];
	uint8_t len = 0;

	buf[len++] = HWC_TYPE_REPLY;
	buf[len++] = HWC_TYPE;

	ringbuffer_push_multiple(conn.txBuffer, buf, len);
}


void acp_handler_battery_chargeReq(acp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint8_t charge;
	if(adc_getBatteryVoltage() <= BATTERY_EMTY_THRESHOLD) charge = 0;
	else charge = (adc_getBatteryVoltage() - BATTERY_EMTY_THRESHOLD) * 255 / (BATTERY_FULL_VOLTAGE - BATTERY_EMTY_THRESHOLD);

	uint8_t buf[2];
	uint8_t len = 0;

	buf[len++] = HWC_BATTERY_CHARGE_REPLY;
	buf[len++] = charge;

	ringbuffer_push_multiple(conn.txBuffer, buf, len);
}
