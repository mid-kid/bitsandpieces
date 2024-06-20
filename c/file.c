#include "file.h"

#include <stdio.h>
#include <stdlib.h>

struct file *file_read(const char *name)
{
    FILE *f = fopen(name, "rb");
    if (!f) return NULL;

    long size;
    if (fseek(f, 0, SEEK_END) == -1 ||
            (size = ftell(f)) == -1 ||
            fseek(f, 0, SEEK_SET) == -1) {
        fclose(f);
        return NULL;
    }

    struct file *file = malloc(sizeof(struct file) + size);
    if (!file) {
        fclose(f);
        return NULL;
    }
    file->size = size;

    if (fread(file->data, file->size, 1, f) != 1) {
        free(file);
        fclose(f);
        return NULL;
    }
    fclose(f);
    return file;
}

bool file_write(const char *name, const struct file *file)
{
    FILE *f = fopen(name, "wb");
    if (!f) return false;
    if (fwrite(file->data, file->size, 1, f) != 1) {
        fclose(f);
        return false;
    }
    fclose(f);
    return true;
}
