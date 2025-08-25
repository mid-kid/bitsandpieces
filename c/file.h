#pragma once

#include <stddef.h>
#include <stdbool.h>

struct file {
    size_t size;
    unsigned char data[];
};

struct file *file_read(const char *name);
bool file_write(const char *name, const struct file *file);
unsigned file_lines(const struct file *file);
