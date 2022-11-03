#!/bin/bash
# Copyright 2022 Google LLC All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# -e: abort script if one command fails
# -u: error if undefined variable used
# -o pipefail: entire command fails if pipe fails. watch out for yes | ...
# -o history: record shell history
set -euo pipefail -o history

# Pull the container (in case it was updated since the instance started) and
# store its SHA in the Sponge log.
docker pull "$DOCKER_IMAGE"
echo "TF_INFO_DOCKER_IMAGE,$DOCKER_IMAGE" >> "$KOKORO_ARTIFACTS_DIR/custom_sponge_config.csv"
echo "TF_INFO_DOCKER_SHA,$(docker pull "$DOCKER_IMAGE" | sed -n '/Digest:/s/Digest: //g p')" >> "$KOKORO_ARTIFACTS_DIR/custom_sponge_config.csv"

# Start a container in the background
docker run --name tsl -w /tf/tsl -itd --rm \
    -v "$KOKORO_ARTIFACTS_DIR/github/tsl:/tf/tsl" \
    "$DOCKER_IMAGE" \
    bash

# Build TSL
docker exec tsl bazel build \
    --deleted_packages="tsl/distributed_runtime/rpc,tsl/profiler/backends/cpu,tsl/profiler/utils,tsl/profiler/lib,tsl/profiler/convert" \
    --output_filter="" \
    --nocheck_visibility \
    --keep_going \
    -- //tsl/... -//tsl/platform/cloud/... -//tsl/framework:allocator_registry_impl -//tsl/framework:bfc_allocator -//tsl/platform/default/build_config:platformlib -//tsl/platform/default/build_config:proto_parsing -//tsl/distributed_runtime:*

docker stop tsl
