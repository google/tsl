/* Copyright 2016 The TensorFlow Authors. All Rights Reserved.

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

#include "tsl/platform/test.h"

#include <cstdlib>

#include "tsl/platform/logging.h"
#include "tsl/platform/net.h"
#include "tsl/platform/path.h"
#include "tsl/platform/strcat.h"
#include "tsl/platform/types.h"

namespace tsl {
namespace testing {

string TmpDir() {
  // 'bazel test' sets TEST_TMPDIR
  const char* env = getenv("TEST_TMPDIR");
  if (env && env[0] != '\0') {
    return env;
  }
  env = getenv("TMPDIR");
  if (env && env[0] != '\0') {
    return env;
  }
  return "/tmp";
}

string SrcDir() {
  // Bazel makes data dependencies available via a relative path.
  return "";
}

int RandomSeed() {
  const char* env = getenv("TEST_RANDOM_SEED");
  int result;
  if (env && sscanf(env, "%d", &result) == 1) {
    return result;
  }
  return 301;
}

int PickUnusedPortOrDie() { return internal::PickUnusedPortOrDie(); }

string TensorFlowSrcRoot() {
  // 'bazel test' sets TEST_SRCDIR, and also TEST_WORKSPACE if a new
  // enough version of bazel is used.
  const char* env = getenv("TEST_SRCDIR");
  const char* workspace = getenv("TEST_WORKSPACE");
  if (env && env[0] != '\0') {
    if (workspace && workspace[0] != '\0') {
      return io::JoinPath(env, workspace, "tensorflow");
    }
    return io::JoinPath(env, "tensorflow");
  }
  LOG(WARNING) << "TEST_SRCDIR environment variable not set: "
               << "using $PWD/tensorflow as TensorFlowSrcRoot() for tests.";
  return "tensorflow";
}

// Returns the path to XLA in the directory containing data
// dependencies.
string XlaSrcRoot() {
  // 'bazel test' sets TEST_SRCDIR, and also TEST_WORKSPACE if a new
  // enough version of bazel is used.
  const char* env = getenv("TEST_SRCDIR");
  const char* workspace = getenv("TEST_WORKSPACE");
  const char* xla_path = "tensorflow/compiler/xla";
  if (env && env[0] != '\0') {
    if (workspace && workspace[0] != '\0') {
      return io::JoinPath(env, workspace, xla_path);
    }
    return io::JoinPath(env, xla_path);
  }
  LOG(WARNING) << "TEST_SRCDIR environment variable not set: "
               << "using $PWD/xla as XlaSrcRoot() for tests.";
  return io::JoinPath(env, xla_path);
}

}  // namespace testing
}  // namespace tsl
