#!/usr/bin/env bash

set -e

CUDA_MEMCHECK="$1"
TEST="$2"
shift 2

LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:/usr/crosstool/v18/x86_64-grtev4-linux-gnu/lib64"

# set TMPDIR. Otherwise some cuda-memcheck variants will attempt to create temp
# files in the current directory.
TMPDIR=${TEST_TMPDIR}

# cuda-memcheck triggers heap check failures; disable **all** heap checking.
TF_GPU_ALLOCATOR=cuda_malloc $CUDA_MEMCHECK $TEST --heap_check="" "$@"
