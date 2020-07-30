#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <complex>
#include <Eigen/Core>

#include <benchmark/benchmark.h>

template <class Scalar>
static void BM_gemm(benchmark::State &state)
{
    using Mat = Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>;

    const int64_t m = state.range(0);
    const int64_t n = state.range(1);
    const int64_t k = state.range(2);

    Mat A(m, k);
    Mat B(k, n);
    Mat C(m, n);
    A.setRandom();
    B.setRandom();
    C.setZero();

    for (auto _ : state)
    {
        C.noalias() += A * B;
    }
    benchmark::DoNotOptimize(C);
}

static void CustomArguments(benchmark::internal::Benchmark *b)
{
    std::ifstream settings("gemm_settings.txt");
    if (settings)
    {
        int64_t m, n, k;
        while (settings >> m >> n >> k)
        {
            b->Args({m, n, k});
        }
    }
    else
    {
        b->Args({8, 8, 8});
        b->Args({9, 9, 9});
        b->Args({24, 24, 24});
        b->Args({239, 239, 239});
        b->Args({240, 240, 240});
        b->Args({2400, 24, 24});
        b->Args({24, 2400, 24});
        b->Args({24, 24, 2400});
        b->Args({24, 2400, 2400});
        b->Args({2400, 24, 2400});
        b->Args({2400, 2400, 24});
        b->Args({2400, 2400, 64});
        b->Args({4800, 23, 160});
        b->Args({23, 4800, 160});
        b->Args({2400, 2400, 2400});
    }
}
BENCHMARK_TEMPLATE(BM_gemm, float)->Apply(CustomArguments);
BENCHMARK_TEMPLATE(BM_gemm, double)->Apply(CustomArguments);
BENCHMARK_TEMPLATE(BM_gemm, std::complex<double>)->Apply(CustomArguments);

