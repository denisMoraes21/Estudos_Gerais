#include "../Inc/ring_buffer.h"

void rb_init(RingBuffer *rb)
{
    rb->head = 0;
    rb->tail = 0;
}

bool rb_empty(RingBuffer *rb)
{
    return rb->head == rb->tail;
}

bool rb_push(RingBuffer *rb, uint8_t c)
{
    uint16_t next = (rb->head + 1) % RB_SIZE;

    if(next == rb->tail)
        return false;

    rb->buffer[rb->head] = c;

    rb->head = next;

    return true;
}

bool rb_pop(RingBuffer *rb, uint8_t *c)
{
    if(rb_empty(rb))
        return false;

    *c = rb->buffer[rb->tail];

    rb->tail = (rb->tail + 1) % RB_SIZE;

    return true;
}