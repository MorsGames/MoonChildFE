#include "Random.h"

#include <stdint.h>

static constexpr uint64_t PCG32_DEFAULT_STATE = 0x853c49e6748fea9bULL;
static constexpr uint64_t PCG32_MULT = 0x5851f42d4c957f2dULL;

static uint64_t RandomState = PCG32_DEFAULT_STATE;
static uint64_t RandomIncrement = 3;

static uint32_t NextRandom()
{
    const uint64_t oldState = RandomState;
    RandomState = oldState * PCG32_MULT + RandomIncrement;

    const uint32_t xorshifted = static_cast<uint32_t>(((oldState >> 18u) ^ oldState) >> 27u);
    const uint32_t rot = static_cast<uint32_t>(oldState >> 59u);
    return (xorshifted >> rot) | (xorshifted << ((0u - rot) & 31u));
}

void PCG32Seed(unsigned int seed)
{
    RandomState = 0;
    RandomIncrement = 3;
    NextRandom();
    RandomState += seed;
    NextRandom();
}

int PCG32Random(int scale)
{
    if (scale < 1)
        return 0;

    const uint32_t randomScale = static_cast<uint32_t>(scale);
    const uint32_t threshold = (0u - randomScale) % randomScale;

    while (true) // true...
    {
        const uint32_t value = NextRandom();
        if (value >= threshold)
        {
            return static_cast<int>(value % randomScale);
        }
    }
}
