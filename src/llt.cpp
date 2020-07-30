#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <complex>
#include <Eigen/Core>
#include <Eigen/Cholesky>

#include <benchmark/benchmark.h>

using namespace Eigen;

template <class Scalar>
static void BM_llt(benchmark::State &state)
{
    using Mat = Eigen::Matrix<Scalar, Dynamic, Dynamic>;

    const int64_t n = state.range(0);

    Mat original(n, n);
    original.setRandom();
    original.diagonal().array() += 1000;

    for (auto _ : state)
    {
        state.PauseTiming();
        benchmark::DoNotOptimize(original);
        Mat mat = original;
        state.ResumeTiming();
        Eigen::internal::llt_inplace<Scalar, Lower>::blocked(mat);
        benchmark::DoNotOptimize(mat);
    }
}

static void CustomArguments(benchmark::internal::Benchmark *b)
{
    std::ifstream settings("llt_settings.txt");
    if (settings)
    {
        int64_t n;
        while (settings >> n)
        {
            b->Args({n});
        }
    }
    else
    {
        b->Args({8});
        b->Args({9});
        b->Args({12});
        b->Args({15});
        b->Args({16});
        b->Args({24});
        b->Args({102});
        b->Args({239});
        b->Args({240});
        b->Args({2400});
        b->Args({2463});
    }
}

BENCHMARK_TEMPLATE(BM_llt, float)->Apply(CustomArguments);
BENCHMARK_TEMPLATE(BM_llt, double)->Apply(CustomArguments);
BENCHMARK_TEMPLATE(BM_llt, std::complex<double>)->Apply(CustomArguments);
