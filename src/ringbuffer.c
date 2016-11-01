#include "ringbuffer.h"
#include <string.h>

//TODO: make asserts work and add it to all functions! or create some other way to show errors

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
	if(num == 0) //tail=head --> completely empty
	{
		return rb->size;
	}
	else //not empty
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

	if((ringbuffer_getFilled(rb) == 1) && (rb->startReadFunction != NULL)) //buffer was empty before, and start-read-function is specified
		(rb->startReadFunction)(); //execute start-read-function
}

void ringbuffer_push_multiple(ringbuffer_t *rb, uint8_t *data, size_t size)
{
	size_t length;
	if((rb->head & (rb->size-1)) + size > rb->size) //wrap
	{
		length = rb->size - (rb->head & (rb->size-1)); //number of values before wrap
		memcpy(rb->buffer, &data[length], size - length); //copy data after wrap
	}
	else length = size; //no wrap
	memcpy(&rb->buffer[(rb->head) & (rb->size -1)], data, length); //copy data before wrap
	rb->head += size;

	if((ringbuffer_getFilled(rb) == size) && (rb->startReadFunction != NULL)) //buffer was empty before, and start-read-function is specified
		(rb->startReadFunction)(); //execute start-read-function
}


uint8_t ringbuffer_peek(ringbuffer_t *rb)
{
	return rb->buffer[(rb->tail) & (rb->size -1)];
}

void ringbuffer_peek_multiple(ringbuffer_t *rb, uint8_t *data, size_t size)
{
	ringbuffer_peek_at_multiple(rb, rb->tail, data, size);
}

uint8_t ringbuffer_peek_at(ringbuffer_t *rb, size_t position)
{
	return rb->buffer[position & (rb->size-1)];
}

void ringbuffer_peek_at_multiple(ringbuffer_t *rb, size_t position, uint8_t *data, size_t size)
{
	if((position & (rb->size -1)) + size < rb->size) //no wrap
	{
		memcpy(data, &rb->buffer[rb->tail & (rb->size -1)], size); //copy data
	}
	else //wrap
	{
		size_t length = rb->size - (position & (rb->size -1)); //number of values before wrap
		memcpy(data, &rb->buffer[rb->tail & (rb->size -1)], length); //copy data before wrap
		memcpy(&data[length], rb->buffer, size - length); //copy data after wrap
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
	rb->tail += size;
}


uint8_t ringbuffer_pop(ringbuffer_t *rb)
{
	//assert(ringbuffer_getFilled(rb) != 0);

	uint8_t value = ringbuffer_peek(rb);
	ringbuffer_consume(rb, 1);
	return value;
}

void ringbuffer_pop_multiple(ringbuffer_t *rb, uint8_t *data, size_t size)
{
	ringbuffer_peek_multiple(rb, data, size);
	ringbuffer_consume(rb, size);
}


void ringbuffer_peek_multiple_trans(ringbuffer_t *rb, ringbuffer_t *dst, size_t size)
{
	ringbuffer_peek_at_multiple_trans(rb, rb->tail, dst, size);
}

void ringbuffer_peek_at_multiple_trans(ringbuffer_t *rb, size_t position, ringbuffer_t *dst, size_t size)
{
	if((position & (rb->size -1)) + size -1 < rb->size) //no wrap
	{
		ringbuffer_push_multiple(dst, &rb->buffer[rb->tail & (rb->size -1)], size); //copy data
	}
	else //wrap
	{
		size_t length = rb->size - (position & (rb->size -1)); //number of values before wrap
		ringbuffer_push_multiple(dst, &rb->buffer[rb->tail & (rb->size -1)], length); //copy data before wrap
		ringbuffer_push_multiple(dst, &rb->buffer[(rb->tail + length) & (rb->size -1)], size - length); //copy data after wrap
	}
}

void ringbuffer_peek_relative_multiple_trans(ringbuffer_t *rb, size_t offset, ringbuffer_t *dst, size_t size)
{
	ringbuffer_peek_at_multiple_trans(rb, rb->tail + offset, dst, size);
}

void ringbuffer_pop_multiple_trans(ringbuffer_t *rb, ringbuffer_t *dst, size_t size)
{
	ringbuffer_peek_multiple_trans(rb, dst, size);
	ringbuffer_consume(rb, size);
}
