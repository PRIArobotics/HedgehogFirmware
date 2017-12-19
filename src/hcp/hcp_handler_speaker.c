#include "hcp_handler.h"
#include "hcp_opcodes.h"
#include "ringbuffer.h"
#include "output.h"

void hcp_handler_speaker(hcp_conn_t conn, uint8_t opcode, size_t payloadLength)
{
	uint16_t frequency = 0;
	frequency |= (ringbuffer_pop(conn.rxBuffer) << 8); //high byte
	frequency |= ringbuffer_pop(conn.rxBuffer); //low byte

	speaker(frequency); //TODO: avoid overwriting startup/shutdown/battery-tones

	ringbuffer_push(conn.txBuffer, HCP_OK);
}
