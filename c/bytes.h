#include <stdint.h>

uint16_t rd_u16(const unsigned char *mem);
uint32_t rd_u32(const unsigned char *mem);
uint64_t rd_u64(const unsigned char *mem);
void wr_u16(unsigned char *mem, uint16_t val);
void wr_u32(unsigned char *mem, uint32_t val);
void wr_u64(unsigned char *mem, uint64_t val);
