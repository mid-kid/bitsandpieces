void hex_dump(const unsigned char *buf, const unsigned len)
{
    for (unsigned i = 0; i < len; i += 0x10) {
        printf("   ");
        for (unsigned x = i; x < i + 0x10 && x < len; x++)  {
            printf(" %02X", buf[x]);
        }
        printf("\n");
    }
}

unsigned hex_read(unsigned char *buf, const unsigned len, const char *str)
{
    unsigned cur = 0;
    while (*str) {
        if (cur >= len) return 0;

        if (*str == ' ') {
            str++;
            continue;
        }

        unsigned char val = 0;
        for (int i = 0; i < 2; i++) {
            char c = str[i];
            unsigned char v;
            if (c >= '0' && c <= '9') {
                v = c - '0';
            } else if (c >= 'A' && c <= 'F') {
                v = c - 'A' + 10;
            } else if (c >= 'a' && c <= 'f') {
                v = c - 'a' + 10;
            } else {
                return 0;
            }
            val |= v << (4 * (1 - i));
        }
        buf[cur++] = val;
        str += 2;
    }
    return cur;
}
