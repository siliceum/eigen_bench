#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <complex>
#include <Eigen/Core>

#include <benchmark/benchmark.h>

using namespace Eigen;

template <class Scalar, int m, int n, int k, int TA>
static void BM_lazy_gemm_impl(benchmark::State &state)
{
    using MatA = Matrix<Scalar, m, k, TA>;
    using MatB = Matrix<Scalar, k, n>;
    using MatC = Matrix<Scalar, m, n>;

    MatA A(m, k);
    MatB B(k, n);
    MatC C(m, n);
    A.setRandom();
    B.setRandom();
    C.setZero();

    for (auto _ : state)
    {
        C.noalias() += A.lazyProduct(B);
    }
    benchmark::DoNotOptimize(C);
}

template <class Scalar, int m, int n, int k>
static void BM_lazy_gemm_impl(benchmark::State &state, int64_t matrixOptions)
{
    if(matrixOptions == ColMajor)
        return BM_lazy_gemm_impl<Scalar,m,n,k,ColMajor>(state);
    if(matrixOptions == RowMajor)
        return BM_lazy_gemm_impl<Scalar,m,n,k,RowMajor>(state);
}


template <class Scalar>
static void BM_lazy_gemm(benchmark::State &state)
{
    const int64_t m = state.range(0);
    const int64_t n = state.range(1);
    const int64_t k = state.range(2);

    const int64_t matrixOptions = state.range(3);
    // Dispatch to static dimensions for square matrices up to 12.
    const int64_t id = m*10000 + n*100 + k;
    switch(id)
    {
        case  10101 : return BM_lazy_gemm_impl<Scalar,  1,  1,  1>(state, matrixOptions);
        case  20202 : return BM_lazy_gemm_impl<Scalar,  2,  2,  2>(state, matrixOptions); \
        case  30303 : return BM_lazy_gemm_impl<Scalar,  3,  3,  3>(state, matrixOptions); \
        case  40404 : return BM_lazy_gemm_impl<Scalar,  4,  4,  4>(state, matrixOptions); \
        case  50505 : return BM_lazy_gemm_impl<Scalar,  5,  5,  5>(state, matrixOptions); \
        case  60606 : return BM_lazy_gemm_impl<Scalar,  6,  6,  6>(state, matrixOptions); \
        case  70707 : return BM_lazy_gemm_impl<Scalar,  7,  7,  7>(state, matrixOptions); \
        case  80808 : return BM_lazy_gemm_impl<Scalar,  8,  8,  8>(state, matrixOptions); \
        case  90909 : return BM_lazy_gemm_impl<Scalar,  9,  9,  9>(state, matrixOptions); \
        case 101010 : return BM_lazy_gemm_impl<Scalar, 10, 10, 10>(state, matrixOptions); \
        case 111111 : return BM_lazy_gemm_impl<Scalar, 11, 11, 11>(state, matrixOptions); \
        case 121212 : return BM_lazy_gemm_impl<Scalar, 12, 12, 12>(state, matrixOptions); \
        default: return BM_lazy_gemm_impl<Scalar, Dynamic, Dynamic, Dynamic>(state, matrixOptions);
    }
}


static void CustomArguments(benchmark::internal::Benchmark* b) {
  std::ifstream settings("gemm_settings.txt");
  if(settings)
  {
    int64_t m, n, k, t;
    while(settings >> m >> n >> k >> t)
    {
      b->Args({m,n,k,t});
    }
  }
  else
  {
      b->Args({ 1, 1, 1,ColMajor});
      b->Args({ 2, 2, 2,ColMajor});
      b->Args({ 3, 3, 3,ColMajor});
      b->Args({ 4, 4, 4,ColMajor});
      b->Args({ 4, 4, 4,RowMajor});
      b->Args({ 5, 5, 5,ColMajor});
      b->Args({ 6, 6, 6,ColMajor});
      b->Args({ 7, 7, 7,ColMajor});
      b->Args({ 7, 7, 7,RowMajor});
      b->Args({ 8, 8, 8,ColMajor});
      b->Args({ 9, 9, 9,ColMajor});
      b->Args({10,10,10,ColMajor});
      b->Args({11,11,11,ColMajor});
      b->Args({12,12,12,ColMajor});
      b->Args({12,12,12,RowMajor});
  }
  
}


BENCHMARK_TEMPLATE(BM_lazy_gemm, float)->Apply(CustomArguments);
BENCHMARK_TEMPLATE(BM_lazy_gemm, double)->Apply(CustomArguments);
BENCHMARK_TEMPLATE(BM_lazy_gemm, std::complex<double>)->Apply(CustomArguments);
