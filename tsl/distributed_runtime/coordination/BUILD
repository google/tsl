load("//tsl/platform:rules_cc.bzl", "cc_library")
load("//tsl/platform:build_config.bzl", "tsl_cc_test")

package(
    default_visibility = [
        "//third_party/tensorflow:internal",
    ],
    licenses = ["notice"],
)

cc_library(
    name = "coordination_service_error_util",
    hdrs = ["coordination_service_error_util.h"],
    deps = [
        "//tsl/platform:errors",
        "//tsl/platform:status",
        "//tsl/protobuf:coordination_service_proto_cc",
        "@com_google_absl//absl/strings",
    ],
)

tsl_cc_test(
    name = "coordination_service_error_util_test",
    srcs = ["coordination_service_error_util_test.cc"],
    deps = [
        ":coordination_service_error_util",
        "//tsl/platform:errors",
        "//tsl/platform:status",
        "//tsl/platform:test",
        "//tsl/platform:test_main",
        "//tsl/protobuf:coordination_service_proto_cc",
    ],
)

cc_library(
    name = "coordination_client",
    hdrs = ["coordination_client.h"],
    deps = [
        "//tsl/distributed_runtime:call_options",
        "//tsl/platform:status",
        "//tsl/protobuf:coordination_service_proto_cc",
    ],
)

filegroup(
    name = "pywrap_required_hdrs",
    srcs = [
        "coordination_client.h",
    ],
)
