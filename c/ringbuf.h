#pragma once

struct ringbuf {
    volatile unsigned char *buffer;
    unsigned size;
    unsigned head;
    unsigned tail;
};

void ringbuf_init(volatile struct ringbuf *buffer, unsigned char *mem, unsigned size);
void ringbuf_reset(volatile struct ringbuf *buffer);
int ringbuf_isempty(volatile struct ringbuf *buffer);
int ringbuf_isfull(volatile struct ringbuf *buffer);
void ringbuf_put(volatile struct ringbuf *buffer, unsigned char c);
unsigned char ringbuf_get(volatile struct ringbuf *buffer);
