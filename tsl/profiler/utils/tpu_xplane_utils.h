/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef TENSORFLOW_TSL_PROFILER_UTILS_TPU_XPLANE_UTILS_H_
#define TENSORFLOW_TSL_PROFILER_UTILS_TPU_XPLANE_UTILS_H_

#include <vector>

#include "third_party/tensorflow/core/profiler/protobuf/xplane.pb.h"

namespace tsl {
namespace profiler {

// Find and return TensorCore XPlanes from the XSpace.
std::vector<const tensorflow::profiler::XPlane*> FindTensorCorePlanes(
    const tensorflow::profiler::XSpace& xspace);

// Find and return Mutable TensorCore XPlanes from the XSpace.
std::vector<tensorflow::profiler::XPlane*> FindMutableTensorCorePlanes(
    tensorflow::profiler::XSpace* xspace);

}  // namespace profiler
}  // namespace tsl

#endif  // TENSORFLOW_TSL_PROFILER_UTILS_TPU_XPLANE_UTILS_H_
