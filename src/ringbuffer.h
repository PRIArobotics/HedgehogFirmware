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

//push value into ringbuffer (make sure there is at least one free slot!)
void ringbuffer_push(ringbuffer_t *rb, uint8_t value);

//pop value out of buffer (make sure the buffer is not emty!)
uint8_t ringbuffer_pop(ringbuffer_t *rb);


#endif /* RINGBUFFER_H_ */