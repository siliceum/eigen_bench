#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <complex>
#include <Eigen/Core>

#include <benchmark/benchmark.h>

using namespace Eigen;

enum Operation {
    gemv,
    gemvt,
    trmv_lo,
    trmv_lot,
    trmv_up,
    trmv_upt,
};

template <class Scalar, Operation op>
static void BM_gemv_base(benchmark::State &state)
{
    using Mat = Eigen::Matrix<Scalar, Dynamic, Dynamic>;
    using Vec = Eigen::Matrix<Scalar, Dynamic, 1>;

    const int64_t m = state.range(0);
    const int64_t n = state.range(1);

    Mat A(m,n);
    Vec B(n);
    Vec C(m);
    A.setRandom();
    B.setRandom();
    C.setRandom();

    for (auto _ : state)
    {
        // Dispatch is optimized by all major compilers
        switch(op)
        {
            case gemv: 
                C.noalias() += A * B;
                break;
            case gemvt:
                B.noalias() += A.transpose() * C;
                break;
            case trmv_lo:
                C.noalias() += A.template triangularView<Lower>() * B;
                break;
            case trmv_lot:
                B.noalias() += A.transpose().template triangularView<Lower>() * C;
                break;
            case trmv_up:
                C.noalias() += A.template triangularView<Upper>() * B;
                break;
            case trmv_upt:
                B.noalias() += A.transpose().template triangularView<Upper>() * C;
                break;
            
        }
    }
    benchmark::DoNotOptimize(B);
    benchmark::DoNotOptimize(C);
}

template <class Scalar>
static void BM_gemv(benchmark::State &state)
{
    return BM_gemv_base<Scalar, gemv>(state);
}

template <class Scalar>
static void BM_gemvt(benchmark::State &state)
{
    return BM_gemv_base<Scalar, gemvt>(state);
}

template <class Scalar>
static void BM_trmv_lo(benchmark::State &state)
{
    return BM_gemv_base<Scalar, trmv_lo>(state);
}

template <class Scalar>
static void BM_trmv_lot(benchmark::State &state)
{
    return BM_gemv_base<Scalar, trmv_lot>(state);
}

template <class Scalar>
static void BM_trmv_up(benchmark::State &state)
{
    return BM_gemv_base<Scalar, trmv_up>(state);
}

template <class Scalar>
static void BM_trmv_upt(benchmark::State &state)
{
    return BM_gemv_base<Scalar, trmv_upt>(state);
}


static void gemv_settings(benchmark::internal::Benchmark *b)
{
    std::ifstream settings("gemv_settings.txt");
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
        b->Args({8, 8});
        b->Args({9, 9});
        b->Args({24, 24});
        b->Args({239,239});
        b->Args({240, 240});
        b->Args({2400, 24});
        b->Args({24, 2400});
        b->Args({24, 240});
        b->Args({2400, 2400});
        b->Args({4800, 23});
        b->Args({23, 4800});
    }
}


static void gemv_squared_settings(benchmark::internal::Benchmark *b)
{
    std::ifstream settings("gemv_squared_settings.txt");
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
        b->Args({8, 8});
        b->Args({9, 9});
        b->Args({12, 12});
        b->Args({15, 15});
        b->Args({16, 16});
        b->Args({24, 24});
        b->Args({53, 53});
        b->Args({74, 74});
        b->Args({102, 102});
        b->Args({239, 239});
        b->Args({240, 240});
        b->Args({2400, 2400});
        b->Args({2463, 2463});
    }
}


#define BENCH_ALL_SCALAR_TYPES(operation, settings) \
    BENCHMARK_TEMPLATE(BM_##operation, double)->Apply(settings); \
    BENCHMARK_TEMPLATE(BM_##operation, float)->Apply(settings); \
    BENCHMARK_TEMPLATE(BM_##operation, std::complex<double>)->Apply(settings); \

BENCH_ALL_SCALAR_TYPES(gemv, gemv_settings)
BENCH_ALL_SCALAR_TYPES(gemvt, gemv_settings)
BENCH_ALL_SCALAR_TYPES(trmv_lo, gemv_squared_settings)
BENCH_ALL_SCALAR_TYPES(trmv_lot, gemv_squared_settings)
BENCH_ALL_SCALAR_TYPES(trmv_up, gemv_squared_settings)
BENCH_ALL_SCALAR_TYPES(trmv_upt, gemv_squared_settings)

