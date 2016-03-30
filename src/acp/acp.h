#ifndef ACP_H_
#define ACP_H_


#include <stdint.h>
#include "ringbuffer.h"


#define HWC_TYPE 4


//connection states
#define WAIT_OPCODE 0
#define WAIT_LENGTH 1
#define WAIT_PAYLOAD 2

typedef struct {
	ringbuffer_t *txBuffer;
	ringbuffer_t *rxBuffer;
} acp_conn_t;

//initialize ACP
void acp_init();

//update the acp handler, call this regularly
void acp_update();


#endif /* ACP_H_ */
