# Description:
#   RPC communication interfaces and implementations for TensorFlow.

load("//tsl/platform:rules_cc.bzl", "cc_library")
load("//tsl/platform:build_config.bzl", "tsl_cc_test")
load("//tsl:tsl.default.bzl", "tsl_grpc_cc_dependencies")

package(
    default_visibility = [
        "//third_party/tensorflow:internal",
    ],
    licenses = ["notice"],
)

cc_library(
    name = "async_service_interface",
    srcs = [],
    hdrs = ["async_service_interface.h"],
    deps = [],
)

cc_library(
    name = "grpc_call",
    srcs = [],
    hdrs = ["grpc_call.h"],
    deps = [
        "//tsl/platform:mutex",
        "//tsl/platform:refcount",
    ] + tsl_grpc_cc_dependencies(),
)

cc_library(
    name = "grpc_util",
    srcs = ["grpc_util.cc"],
    hdrs = ["grpc_util.h"],
    deps = [
        "//tsl/platform:random",
        "//tsl/platform:status",
        "//tsl/platform:stringpiece",
        "//tsl/platform:stringprintf",
        "//tsl/protobuf:distributed_runtime_payloads_proto_cc",
    ] + tsl_grpc_cc_dependencies(),
)

tsl_cc_test(
    name = "grpc_util_test",
    size = "small",
    srcs = ["grpc_util_test.cc"],
    tags = [
        "no_mac",
    ],
    deps = [
        ":grpc_util",
        "//tsl/platform:errors",
        "//tsl/platform:test",
        "//tsl/platform:test_main",
        "//tsl/protobuf:distributed_runtime_payloads_proto_cc_impl",
    ] + tsl_grpc_cc_dependencies(),
)
