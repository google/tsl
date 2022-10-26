load(
    "//tsl/platform:rules_cc.bzl",
    "cc_library",
)
load("//third_party/tensorflow:tensorflow.bzl", "tf_cc_test")
load("//tsl/profiler/builds:build_config.bzl", "tf_profiler_copts")

package(
    default_visibility = ["//third_party/tensorflow/core/profiler:internal"],
    licenses = ["notice"],
)

cc_library(
    name = "xla_op_utils",
    hdrs = ["xla_op_utils.h"],
    deps = ["@com_google_absl//absl/strings"],
)

tf_cc_test(
    name = "xla_op_utils_test",
    size = "small",
    srcs = ["xla_op_utils_test.cc"],
    deps = [
        ":xla_op_utils",
        "//tsl/platform:test",
        "//tsl/platform:test_main",
    ],
)

cc_library(
    name = "post_process_single_host_xplane",
    srcs = ["post_process_single_host_xplane.cc"],
    hdrs = ["post_process_single_host_xplane.h"],
    copts = tf_profiler_copts(),
    deps = [
        "//third_party/tensorflow/core/profiler/protobuf:xplane_proto_cc",
        "//tsl/platform:types",
        "//tsl/profiler/utils:xplane_schema",
        "//tsl/profiler/utils:xplane_utils",
    ],
)
