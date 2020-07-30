#!/bin/bash

if [ ! -d eigen_src ]; then

    git clone https://gitlab.com/libeigen/eigen.git eigen_src

    pushd eigen_src
    git config --add remote.origin.fetch +refs/merge-requests/*/merge:refs/remotes/origin/merge-requests/*/merge
    git fetch
    popd
else
    echo "eigen_src already exists."
    echo "If you need access to the pull requests, be sure to execute `git config --add remote.origin.fetch +refs/merge-requests/*/merge:refs/remotes/origin/merge-requests/*/merge` inside eigen_src."
fi
