#include "ringbuffer.h"
#include <string.h>


void ringbuffer_error()
{

}


void ringbuffer_init(ringbuffer_t *rb, uint8_t *buf, size_t bufSize)
{
	if((bufSize - 1) & bufSize)  ringbuffer_error();
	if(bufSize > MAX_BUFFER_SIZE) ringbuffer_error();

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
	return rb->size - ringbuffer_getFilled(rb);
}

size_t ringbuffer_getFilled(ringbuffer_t *rb)
{
	return rb->head - rb->tail;
}


void ringbuffer_push(ringbuffer_t *rb, uint8_t value)
{
	if(ringbuffer_getFree(rb) == 0) ringbuffer_error();

	rb->buffer[(rb->head) & (rb->size -1)] = value;
	rb->head++;

	if(rb->startReadFunction != NULL) //start-read-function is specified
		(rb->startReadFunction)(); //execute start-read-function
}

void ringbuffer_push_multiple(ringbuffer_t *rb, uint8_t *data, size_t size)
{
	if(ringbuffer_getFree(rb) < size) ringbuffer_error();

	size_t length;
	if((rb->head & (rb->size-1)) + size > rb->size) //wrap
	{
		length = rb->size - (rb->head & (rb->size-1)); //number of values before wrap
		memcpy(&rb->buffer[0], &data[length], size - length); //copy data after wrap
	}
	else length = size; //no wrap
	memcpy(&rb->buffer[(rb->head) & (rb->size -1)], data, length); //copy data before wrap
	rb->head += size;

	if(rb->startReadFunction != NULL) //start-read-function is specified
		(rb->startReadFunction)(); //execute start-read-function
}


uint8_t ringbuffer_peek(ringbuffer_t *rb)
{
	if(ringbuffer_getFilled(rb) == 0) ringbuffer_error();
	return rb->buffer[(rb->tail) & (rb->size -1)];
}

void ringbuffer_peek_multiple(ringbuffer_t *rb, uint8_t *data, size_t size)
{
	if(ringbuffer_getFilled(rb) < size) ringbuffer_error();
	ringbuffer_peek_at_multiple(rb, rb->tail, data, size);
}

uint8_t ringbuffer_peek_at(ringbuffer_t *rb, size_t position)
{
	return rb->buffer[position & (rb->size-1)];
}

void ringbuffer_peek_at_multiple(ringbuffer_t *rb, size_t position, uint8_t *data, size_t size)
{
	if((position & (rb->size -1)) + size - 1 < rb->size) //no wrap
	{
		memcpy(data, &rb->buffer[position & (rb->size -1)], size); //copy data
	}
	else //wrap
	{
		size_t length = rb->size - (position & (rb->size -1)); //number of values before wrap
		memcpy(data, &rb->buffer[position & (rb->size -1)], length); //copy data before wrap
		memcpy(&data[length], &rb->buffer[0], size - length); //copy data after wrap
	}
}

uint8_t ringbuffer_peek_relative(ringbuffer_t *rb, size_t offset)
{
	return ringbuffer_peek_at(rb, (rb->tail + offset));
}

void ringbuffer_peek_relative_multiple(ringbuffer_t *rb, size_t offset, uint8_t *data, size_t size)
{
	ringbuffer_peek_at_multiple(rb, rb->tail + offset, data, size);
}


void ringbuffer_consume(ringbuffer_t *rb, size_t size)
{
	if(ringbuffer_getFilled(rb) < size) ringbuffer_error();
	rb->tail += size;
}


uint8_t ringbuffer_pop(ringbuffer_t *rb)
{
	if(ringbuffer_getFilled(rb) == 0) ringbuffer_error();

	uint8_t value = ringbuffer_peek(rb);
	ringbuffer_consume(rb, 1);
	return value;
}

void ringbuffer_pop_multiple(ringbuffer_t *rb, uint8_t *data, size_t size)
{
	if(ringbuffer_getFilled(rb) < size) ringbuffer_error();

	ringbuffer_peek_multiple(rb, data, size);
	ringbuffer_consume(rb, size);
}


void ringbuffer_peek_multiple_trans(ringbuffer_t *rb, ringbuffer_t *dst, size_t size)
{
	if(ringbuffer_getFilled(rb) < size) ringbuffer_error();
	
	ringbuffer_peek_at_multiple_trans(rb, rb->tail, dst, size);
}

void ringbuffer_peek_at_multiple_trans(ringbuffer_t *rb, size_t position, ringbuffer_t *dst, size_t size)
{
	if((position & (rb->size -1)) + size - 1 < rb->size) //no wrap
	{
		ringbuffer_push_multiple(dst, &rb->buffer[position & (rb->size -1)], size); //copy data
	}
	else //wrap
	{
		size_t length = rb->size - (position & (rb->size -1)); //number of values before wrap
		ringbuffer_push_multiple(dst, &rb->buffer[position & (rb->size -1)], length); //copy data before wrap
		ringbuffer_push_multiple(dst, &rb->buffer[0], size - length); //copy data after wrap
	}
}

void ringbuffer_peek_relative_multiple_trans(ringbuffer_t *rb, size_t offset, ringbuffer_t *dst, size_t size)
{
	ringbuffer_peek_at_multiple_trans(rb, rb->tail + offset, dst, size);
}

void ringbuffer_pop_multiple_trans(ringbuffer_t *rb, ringbuffer_t *dst, size_t size)
{
	if(ringbuffer_getFilled(rb) < size) ringbuffer_error();

	ringbuffer_peek_multiple_trans(rb, dst, size);
	ringbuffer_consume(rb, size);
}
