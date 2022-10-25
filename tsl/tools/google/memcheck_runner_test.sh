#!/usr/bin/env bash

set -e
source gbash.sh || exit

if [ ! -r /usr/crosstool/v18/x86_64-grtev4-linux-gnu/lib64/libstdc++.so.6 ]; then
  gbash::die "Expected to find libstdc++.so.6, but could not."
fi
