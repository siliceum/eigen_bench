# Benchmarks for the [Eigen](http://eigen.tuxfamily.org) library

Benchmarks of the [Eigen](http://eigen.tuxfamily.org) library adapted to use Google benchmark.
Only the benchmarks under the folder `bench/perf_monitoring` were adapted.

## Requirements

- CMake 3.15+
- Git
- C++11 standard compliant compiler
- An internet connection for the 1st compilation (will fetch google benchmark and eigen)
- python3 and the pyperf module installed if you want to use the MR replay script

## How to use

This project assumes eigen has been cloned into the ./eigen_src directory. use `./bootstrap.sh` to automatically fetch and configure git for merge request fetches.

CMake will feth google benchmark on the first configuration of a build directory if needed. This means you need an internet access the first time you configure cmake for a given build directory.

### Compile and run the tests manually

```sh
# Configure the build directory for Release
cmake -B build -GNinja -DCMAKE_BUILD_TYPE=Release
# Compile the benchmarks in Release
cmake --build build --config Release
# Navigate to the build directory and run the benchmarks
pushd build
ctest -VV
popd
```

### Replay all merge requests

The `./run_benchs_for_PRs.sh` handles bootstrapping, compilation and running the tests by itself.
The test results will be in the `results/` folder, with each subfolder being a commit SHA.
