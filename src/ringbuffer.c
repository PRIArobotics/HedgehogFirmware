#include "ringbuffer.h"

//TODO: make asserts work! or create some way to show errors


void ringbuffer_init(ringbuffer_t *rb, uint8_t *buf, size_t bufSize)
{
	//assert(!((bufSize - 1) & bufSize));
	//assert(bufSize <= MAX_BUFFER_SIZE);

	rb->size = bufSize;
	rb->buffer = buf;
	rb->head = 0;
	rb->tail = 0;
	rb->startReadFunction = NULL;
}


void ringbuffer_setStartReadFunction(ringbuffer_t *rb, void (*function)(void))
{
	rb->startReadFunction = function;
}


size_t ringbuffer_getFree(ringbuffer_t *rb)
{
	size_t num = rb->tail - rb->head;
	if(num == 0) //tail=head --> completely emty
	{
		return rb->size;
	}
	else //not emty
	{
		num &= (rb->size - 1); //cut away leading ones in case tail<head
		return num;
	}
}


size_t ringbuffer_getFilled(ringbuffer_t *rb)
{
	return rb->size - ringbuffer_getFree(rb);
}


void ringbuffer_push(ringbuffer_t *rb, uint8_t value)
{
	//assert(ringbuffer_getFree(rb) != 0);

	rb->buffer[(rb->head) & (rb->size -1)] = value;
	rb->head++;

	if((ringbuffer_getFilled(rb) == 1) && (rb->startReadFunction != NULL)) //buffer was emty before, and start-read-function is specified
		(rb->startReadFunction)(); //execute start-read-function
}


uint8_t ringbuffer_pop(ringbuffer_t *rb)
{
	//assert(ringbuffer_getFilled(rb) != 0);

	uint8_t value = rb->buffer[(rb->tail) & (rb->size -1)];
	rb->tail++;
	return value;
}
