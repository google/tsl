/* Copyright 2026 The OpenXLA Authors.

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

#ifndef TENSORFLOW_TSL_PROFILER_LIB_TRACEME_GLOBAL_FLAGS_H_
#define TENSORFLOW_TSL_PROFILER_LIB_TRACEME_GLOBAL_FLAGS_H_

#include <stddef.h>
#include <stdint.h>

#include <atomic>

#include "xla/tsl/platform/macros.h"

// Flags that are global to the TraceMe implementation, which may be shared by
// TraceMe and TraceMeRecorder.
namespace tsl {
namespace profiler {

TF_EXPORT extern std::atomic<bool> g_enable_source_location;

class TraceMeGlobalFlags {
 public:
  // Returns whether source location is enabled.
  static bool IsSourceLocationEnabled() {
    return g_enable_source_location.load(std::memory_order_relaxed);
  }
};

}  // namespace profiler
}  // namespace tsl

#endif  // TENSORFLOW_TSL_PROFILER_LIB_TRACEME_GLOBAL_FLAGS_H_
