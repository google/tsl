/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

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

#include "third_party/tensorflow/core/framework/metrics.h"

#include "tsl/lib/monitoring/cell_reader.h"
#include "tsl/platform/test.h"

namespace tsl {
namespace metrics {
TEST(RecordTPUXlaCompileCountTest, StreamzIncrementsOnCall) {
  tsl::monitoring::testing::CellReader<int64_t> counter_reader(
      "/platforms/xla/tpu/compile_count");

  EXPECT_EQ(counter_reader.Delta("foo", "bar"), 0);
  tensorflow::metrics::RecordTPUXlaCompileCount("foo", "bar");
  EXPECT_EQ(counter_reader.Delta("foo", "bar"), 1);
  tensorflow::metrics::RecordTPUXlaCompileCount("foo", "bar");
  tensorflow::metrics::RecordTPUXlaCompileCount("foo", "bar");
  EXPECT_EQ(counter_reader.Delta("foo", "bar"), 2);
}
}  // namespace metrics
}  // namespace tsl
