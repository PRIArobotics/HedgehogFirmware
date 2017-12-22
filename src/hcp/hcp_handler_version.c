#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"

#define STM32_UUID ((uint32_t *)0x1FFF7A10)

#define HW_VERSION 2
#define SW_VERSION 0

void hcp_handler_version(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	ringbuffer_push(conn.txBuffer, HCP_VERS_REP);
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[2] >> 24));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[2] >> 16));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[2] >> 8));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[2] >> 0));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[1] >> 24));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[1] >> 16));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[1] >> 8));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[1] >> 0));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[0] >> 24));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[0] >> 16));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[0] >> 8));
	ringbuffer_push(conn.txBuffer, (uint8_t)(STM32_UUID[0] >> 0));
	ringbuffer_push(conn.txBuffer, HW_VERSION);
	ringbuffer_push(conn.txBuffer, SW_VERSION);
}
