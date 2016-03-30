#ifndef ACP_HANDLER_H_
#define ACP_HANDLER_H_


#include <stddef.h>
#include "acp.h"


void acp_handler_nop(acp_conn_t conn, uint8_t opcode, size_t payload);
#define not_implemented_handler acp_handler_nop

void acp_handler_analog_valueReq(acp_conn_t conn, uint8_t opcode, size_t payload);
void acp_handler_analog_valueSub(acp_conn_t conn, uint8_t opcode, size_t payload);
void acp_analog_sendSubUpdate(acp_conn_t conn);
void acp_handler_analog_pullAct(acp_conn_t conn, uint8_t opcode, size_t payload);

void acp_handler_digital_valueReq(acp_conn_t conn, uint8_t opcode, size_t payload);
void acp_handler_digital_valueSub(acp_conn_t conn, uint8_t opcode, size_t payload);
void acp_digital_sendSubUpdate(acp_conn_t conn);
void acp_handler_digital_pullAct(acp_conn_t conn, uint8_t opcode, size_t payload);

void acp_handler_servo_onOffAct(acp_conn_t conn, uint8_t opcode, size_t payload);
void acp_handler_servo_positionAct(acp_conn_t conn, uint8_t opcode, size_t payload);
void acp_handler_servo_positionSyncAct(acp_conn_t conn, uint8_t opcode, size_t payload);

void acp_handler_motor_offAct(acp_conn_t conn, uint8_t opcode, size_t payload);
void acp_handler_motor_powerAct(acp_conn_t conn, uint8_t opcode, size_t payload);
void acp_handler_motor_brakeAct(acp_conn_t conn, uint8_t opcode, size_t payload);

void acp_handler_HWC_offAct(acp_conn_t conn, uint8_t opcode, size_t payload);
void acp_handler_HWC_resetAct(acp_conn_t conn, uint8_t opcode, size_t payload);

void acp_handler_HWC_typeReq(acp_conn_t conn, uint8_t opcode, size_t payload);

//TODO SPI?
//void acp_handler_extPort_connectAct(acp_conn_t conn, uint8_t opcode, size_t payload);
//void acp_handler_extPort_sendAct(acp_conn_t conn, uint8_t opcode, size_t payload);
//void acp_extPort_sendReceived(acp_conn_t conn);

void acp_handler_battery_voltageReq(acp_conn_t conn, uint8_t opcode, size_t payload);


#endif /* ACP_HANDLER_H_ */
