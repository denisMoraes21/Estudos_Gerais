#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define RB_SIZE 2048

typedef struct
{
    uint8_t buffer[RB_SIZE];

    volatile uint16_t head;

    volatile uint16_t tail;

} RingBuffer;

void rb_init(RingBuffer *rb);

bool rb_push(RingBuffer *rb, uint8_t c);

bool rb_pop(RingBuffer *rb, uint8_t *c);

bool rb_empty(RingBuffer *rb);

#endif