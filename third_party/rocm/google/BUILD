# Internal stub version of rocm macros.

load("//third_party/tensorflow/core/platform:rules_cc.bzl", "cc_library")
load("@bazel_skylib//:bzl_library.bzl", "bzl_library")

package(
    # copybara:uncomment default_applicable_licenses = ["//third_party/tensorflow:license"],
    default_visibility = [
        "//third_party/tensorflow:__subpackages__",
    ],
    licenses = ["notice"],
)

# ROCm is not currently supported in google3, but we want a config setting to
# use in open source. Specify one that never matches.
config_setting(
    name = "using_hipcc",
    define_values = {"something": "that will never match"},
)

cc_library(
    name = "rocm_headers",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "hipfft",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "hipsparse",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "hipsolver",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "hiprand",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "miopen",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "rccl",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "rocblas",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "rocprim",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "rocm_root",
    srcs = [],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "rocsolver",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "roctracer",
    srcs = [],
    hdrs = [],
    visibility = ["//visibility:public"],
)

bzl_library(
    name = "build_defs_bzl",
    srcs = ["build_defs.bzl"],
)
