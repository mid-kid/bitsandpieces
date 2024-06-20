#include "bytes.h"

// Funky notes:
// Due to the fact that smaller types are upcasted to "int" in arithmetic,
// it's only unnecessary to cast in rd_u16, as "int" has a minimum of 16 bits.

uint16_t rd_u16(const unsigned char *mem)
{
    return mem[0] << 0 | mem[1] << 8;
}

uint32_t rd_u32(const unsigned char *mem)
{
    return (uint32_t)mem[0] << 0 | (uint32_t)mem[1] << 8 |
        (uint32_t)mem[2] << 16 | (uint32_t)mem[3] << 24;
}

uint64_t rd_u64(const unsigned char *mem)
{
    return (uint64_t)mem[0] << 0 | (uint64_t)mem[1] << 8 |
        (uint64_t)mem[2] << 16 | (uint64_t)mem[3] << 24 |
        (uint64_t)mem[4] << 32 | (uint64_t)mem[5] << 40 |
        (uint64_t)mem[6] << 48 | (uint64_t)mem[7] << 56;
}

void wr_u16(unsigned char *mem, uint16_t val)
{
    mem[0] = val >> 0; mem[1] = val >> 8;
}

void wr_u32(unsigned char *mem, uint32_t val)
{
    mem[0] = val >> 0; mem[1] = val >> 8;
    mem[2] = val >> 16; mem[3] = val >> 24;
}

void wr_u64(unsigned char *mem, uint64_t val)
{
    mem[0] = val >> 0; mem[1] = val >> 8;
    mem[2] = val >> 16; mem[3] = val >> 24;
    mem[4] = val >> 32; mem[5] = val >> 40;
    mem[6] = val >> 48; mem[7] = val >> 56;
}
