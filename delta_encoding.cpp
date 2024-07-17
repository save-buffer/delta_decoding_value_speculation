#include <stdint.h>
#include <stddef.h>

void DeltaEncode(uint32_t *uncompressed, uint32_t *compressed, size_t n)
{
    uint32_t prev = 0;
    for(size_t i = 0; i < n; i++)
    {
        uint32_t cur = uncompressed[i];
        compressed[i] = cur - prev;
        prev = cur;
    }
}

void DeltaDecode(uint32_t *compressed, uint32_t *uncompressed, size_t n)
{
    uint32_t prev = 0;
    for(size_t i = 0; i < n; i++)
    {
        uint32_t delta = compressed[i];
        uint32_t val = prev + delta;
        uncompressed[i] = val;
        prev = val;
    }
}

