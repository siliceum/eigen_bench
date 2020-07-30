cmake_minimum_required(VERSION 3.15)

project(eigen-benchmarks)

option(EIGEN_BENCH_ENABLE_TESTING "Enable test targets." ON)
include(FetchContent)

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.5.1
)

set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_MakeAvailable(benchmark)

add_executable(bench_eigen
  src/main.cpp
  src/gemm.cpp
  src/lazy_gemm.cpp
  src/gemv_trmv.cpp
  src/llt.cpp
)
target_link_libraries(bench_eigen PRIVATE benchmark::benchmark)
target_include_directories(bench_eigen PRIVATE eigen_src)
target_compile_options(bench_eigen PRIVATE -DEIGEN_DONT_PARALLELIZE)


if(EIGEN_BENCH_ENABLE_TESTING)
  enable_testing()

  set(EIGEN_BENCH_NB_ITER 5 CACHE STRING "Number of repetitions to be done when running benchmarks")
  set(EIGEN_BENCH_MIN_TIME 0.01 CACHE STRING "Minimum time to spend per benchmark")

  add_test(NAME eigen COMMAND $<TARGET_FILE:bench_eigen> --benchmark_min_time=${EIGEN_BENCH_MIN_TIME} --benchmark_repetitions=${EIGEN_BENCH_NB_ITER})
endif()















