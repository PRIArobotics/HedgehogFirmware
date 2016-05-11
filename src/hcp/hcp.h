#ifndef HCP_H_
#define HCP_H_


#include <stdint.h>
#include "ringbuffer.h"


//connection states
#define WAIT_OPCODE 0
#define WAIT_LENGTH 1
#define WAIT_PAYLOAD 2

typedef struct {
	ringbuffer_t *txBuffer;
	ringbuffer_t *rxBuffer;
} hcp_conn_t;

//initialize HCP
void hcp_init();

//update the HCP handler, call this regularly
void hcp_update();


#endif /* HCP_H_ */
