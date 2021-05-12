#include "ringbuf.h"

void ringbuf_init(volatile struct ringbuf *buffer, unsigned char *mem, unsigned size)
{
    buffer->buffer = mem;
    buffer->size = size;
    buffer->head = buffer->tail = 0;
}

void ringbuf_reset(volatile struct ringbuf *buffer)
{
    buffer->head = buffer->tail = 0;
}

// More appropriate modulo for devices without division instruction
// Gets slower the bigger var is, but we don't expect more than one iteration
__attribute__((always_inline))
inline static unsigned mod(unsigned var, unsigned div)
{
    while (var >= div) var -= div;
    return var;
}

__attribute__((always_inline))
inline int ringbuf_isempty(volatile struct ringbuf *buffer)
{
    return buffer->head == buffer->tail;
}

__attribute__((always_inline))
inline int ringbuf_isfull(volatile struct ringbuf *buffer)
{
    return mod(buffer->head + 1, buffer->size) == buffer->tail;
}

__attribute__((always_inline))
inline void ringbuf_put(volatile struct ringbuf *buffer, unsigned char c)
{
    buffer->buffer[buffer->head] = c;
    buffer->head = mod(buffer->head + 1, buffer->size);
}

__attribute__((always_inline))
inline unsigned char ringbuf_get(volatile struct ringbuf *buffer)
{
    unsigned char c = buffer->buffer[buffer->tail];
    buffer->tail = mod(buffer->tail + 1, buffer->size);
    return c;
}
