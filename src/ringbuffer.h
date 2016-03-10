#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_


#include <stdint.h>
#include <stddef.h>


#define MAX_BUFFER_SIZE ((SIZE_MAX + 1) / 2)


typedef struct
{
    size_t size;
    uint8_t *buffer;
    volatile size_t head;
    volatile size_t tail;
    void (*startReadFunction)(void);
} ringbuffer_t;


//initialize ringbuffer, bufSize must be a power of 2, bigger than 0 and smaller than MAX_BUFFER_SIZE
void ringbuffer_init(ringbuffer_t *rb, uint8_t *buf, size_t bufSize);

//set a function to be called when the emty ringbuffer gets filled (to start a buffer read sequence)
void ringbuffer_setStartReadFunction(ringbuffer_t *rb, void (*function)(void));

//get the number of free slots
size_t ringbuffer_getFree(ringbuffer_t *rb);
//get the number of filled slots
size_t ringbuffer_getFilled(ringbuffer_t *rb);

//push functions, make sure there is at least one free slot!
//push value into ringbuffer
void ringbuffer_push(ringbuffer_t *rb, uint8_t value);
//push multiple values into ringbuffer
void ringbuffer_push_multiple(ringbuffer_t *rb, uint8_t *data, size_t size);

//peek functions, does not modify the ringbuffer, make sure the buffer is not emty!
//peek at next value in ringbuffer
uint8_t ringbuffer_peek(ringbuffer_t *rb);
//peek at next multiple values in ringbuffer
void ringbuffer_peek_multiple(ringbuffer_t *rb, uint8_t *data, size_t size);
//peek at value at specific position in ringbuffer
uint8_t ringbuffer_peek_at(ringbuffer_t *rb, size_t position);
//peek at multiple values at specific position in ringbuffer
void ringbuffer_peek_at_multiple(ringbuffer_t *rb, size_t position, uint8_t *data, size_t size);
//peek at value at value at specific offset in ringbuffer
uint8_t ringbuffer_peek_relative(ringbuffer_t *rb, size_t offset);
//peek at multiple values at value at specific offset in ringbuffer
void ringbuffer_peek_relative_multiple(ringbuffer_t *rb, size_t offset, uint8_t *data, size_t size);

//pop functions, make sure the buffer is not emty!
//pop value out of buffer
uint8_t ringbuffer_pop(ringbuffer_t *rb);
//pop multiple values out of buffer
void ringbuffer_pop_multiple(ringbuffer_t *rb, uint8_t *data, size_t size);

//transfer functions,ringbuffer as the data destination, make sure the buffer is not emty and the destination buffer is not full!
//peek at next multiple values in ringbuffer *rb and push data to destination ringbuffer *dst
void ringbuffer_peek_multiple_trans(ringbuffer_t *rb, ringbuffer_t *dst, size_t size);
//peek at multiple values at specific position in ringbuffer *rb and push data to destination ringbuffer *dst
void ringbuffer_peek_at_multiple_trans(ringbuffer_t *rb, size_t position, ringbuffer_t *dst, size_t size);
//peek at multiple values at value at specific offset in ringbuffer *rb and push data to destination ringbuffer *dst
void ringbuffer_peek_relative_multiple_trans(ringbuffer_t *rb, size_t offset, ringbuffer_t *dst, size_t size);
//pop multiple values out of buffer push data to destination ringbuffer *dst
void ringbuffer_pop_multiple_trans(ringbuffer_t *rb, ringbuffer_t *dst, size_t size);

#endif /* RINGBUFFER_H_ */
