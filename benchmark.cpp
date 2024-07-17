#include "delta_encoding.h"

#include <immintrin.h>
#include <random>
#include <functional>

void Benchmark(const char *label, std::function<void()> fn, size_t batch = 1, size_t niter = 1000)
{
    size_t start = __rdtsc();
    for(size_t i = 0; i < niter; i++)
        fn();
    size_t end = __rdtsc();
    double cycles_per_iter = static_cast<double>(end - start) / static_cast<double>(niter * batch);
    printf("%s: %.2f\n", label, cycles_per_iter);
}

int main()
{
    constexpr size_t NumValues = 4096; // We want this to fit in L1 cache
    constexpr uint32_t ProbablyValue = 100;
    constexpr uint32_t ValueProbabilityPercent = 95;

    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<uint32_t> distrib(0, 99);

    std::vector<uint32_t> uncompressed(NumValues);
    std::vector<uint32_t> compressed(NumValues);
    uncompressed[0] = 0;
    for(size_t i = 1; i < NumValues; i++)
    {
        if(distrib(gen) < ValueProbabilityPercent)
            uncompressed[i] = ProbablyValue;
        else
            uncompressed[i] = distrib(gen);
    }

#if 0
    Benchmark("Encode_Normal", [&]()
    {
        DeltaEncode(uncompressed.data(), compressed.data(), NumValues);
    }, NumValues);

    Benchmark("Encode_Speculative", [&]()
    {
        DeltaEncode_ValueSpeculative(uncompressed.data(), compressed.data(), NumValues, ProbablyValue);
    }, NumValues);

#endif

    DeltaEncode(uncompressed.data(), compressed.data(), NumValues);

#if 1
    Benchmark("Decode_Normal", [&]()
    {
        DeltaDecode(compressed.data(), uncompressed.data(), NumValues);
    }, NumValues);
#else
    Benchmark("Decode_Speculative", [&]()
    {
        DeltaDecode_ValueSpeculative(compressed.data(), uncompressed.data(), NumValues, ProbablyValue);
    }, NumValues);
#endif
}
