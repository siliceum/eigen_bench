#!/bin/bash

# Exit on error
set -e

# Prepare system for benchmarking
sudo python3 -m pyperf system tune

cleanup()
{
    echo "================================================================="
    echo "=========== Reseting system to normal settings =================="
    echo "================================================================="
    sudo python3 -m pyperf system reset
}
trap cleanup EXIT

results_folder="$(pwd)/results"
mkdir -p $results_folder

build_and_bench () {
    local results_prefix="$results_folder/$1"
    local num_iterations=$2

    if [ ! -d $results_prefix ] || [ -e $results_prefix/.dirty ]; then
        mkdir $results_prefix -p
        touch $results_prefix/.dirty

        cmake -B build -GNinja -DCMAKE_BUILD_TYPE=Release || touch $results_prefix/.doesnotbuild
        cmake --build build --config Release || touch $results_prefix/.doesnotbuild
        
        if [ ! -e $results_prefix/.doesnotbuild ]; then
            echo "==== Running benchmark $num_iterations times ===="
            for i in $(seq 1 $num_iterations)
            do
                BENCHMARK_BENCHMARK_OUT="$results_prefix/$i.json"
                echo "output is $BENCHMARK_BENCHMARK_OUT"
                pushd build
                BENCHMARK_BENCHMARK_OUT=$BENCHMARK_BENCHMARK_OUT ctest -VV
                popd
            done
        else
            echo "$1 does not build." > build_errors.txt
        fi

        rm $results_prefix/.dirty
    else
        echo "Skipping $1, was already done."
    fi
}

checkout_build_and_bench()
{
    pushd eigen_src
    git checkout $1
    popd
    build_and_bench $1 $2
}

benchPR()
{
    pushd eigen_src

    prnumber=$1

    mergecommit=$(git rev-parse origin/merge-requests/$prnumber/merge || touch $results_folder/.mr$prnumber-notfound)

    if [ -e $results_folder/.mr$prnumber-notfound ]; then
        echo "================================================================="
        echo "Could not get information about MR $prnumber. Skipping it."
        echo "It is possible it was closed and never merged."
        echo "================================================================="
    else
        headcommit=$(git rev-parse origin/merge-requests/$prnumber/merge^2)
        targetcommit=$(git rev-parse origin/merge-requests/$prnumber/merge^1)

        echo "================================================================="
        echo " Benchmarking MR $prnumber"
        echo "-----------------------------------------------------------------"
        echo " mergecommit: $mergecommit"
        echo " headcommit: $headcommit"
        echo " targetcommit: $targetcommit"
        echo "================================================================="
    fi

    popd

    if [ ! -e $results_folder/.mr$prnumber-notfound ]; then
        checkout_build_and_bench $targetcommit $2
        checkout_build_and_bench $mergecommit $2
    fi

}


iterations=1

./bootstrap.sh

for mr in $(seq 170 -1 1)
do
    benchPR $mr $iterations
done

