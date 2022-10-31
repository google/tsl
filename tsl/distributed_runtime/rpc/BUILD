# Description:
#   RPC communication interfaces and implementations for TensorFlow.

load("//tsl/platform:rules_cc.bzl", "cc_library")
load("//tsl/platform:build_config.bzl", "tsl_cc_test")
load("//tsl:tsl.default.bzl", "tsl_grpc_cc_dependencies")

package(
    default_visibility = [
        "//tsl:internal",
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

cc_library(
    name = "grpc_channel_common",
    hdrs = ["grpc_channel_common.h"],
    deps = [
        ":grpc_util",
        "//tsl/platform:logging",
        "//tsl/platform:mutex",
        "@com_google_absl//absl/container:flat_hash_map",
    ],
)

cc_library(
    name = "grpc_channel",
    srcs = ["grpc_channel.cc"],
    hdrs = ["grpc_channel.h"],
    deps = [
        ":grpc_channel_common",
        ":grpc_util",
        "//base",
        "@com_google_absl//absl/strings",
        "//tsl/lib/gtl:map_util",
        "//tsl/util:device_name_utils",
        "//tsl/platform:numbers",
        "//tsl/platform:str_util",
        "//tsl/platform:strcat",
        "//tsl/platform:errors",
        "//tsl/platform:logging",
        "//tsl/platform:macros",
        "//tsl/platform:mutex",
        "//tsl/platform:status",
        "//tsl/platform:thread_annotations",
        "//tsl/platform:types",
        "//tsl/protobuf:rpc_options_proto_cc",
    ] + tsl_grpc_cc_dependencies(),
)

tsl_cc_test(
    name = "grpc_channel_test",
    size = "small",
    srcs = [
        "grpc_channel_test.cc",
    ],
    deps = [
        ":grpc_channel",
        "//tsl/lib/core:status_test_util",
        "//tsl/platform:strcat",
        "//tsl/platform:test",
        "//tsl/platform:test_main",
        "//tsl/protobuf:rpc_options_proto_cc_impl",
        "//tsl/util:device_name_utils",
    ],
)
