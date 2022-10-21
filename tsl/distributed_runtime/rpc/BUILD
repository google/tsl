# Description:
#   RPC communication interfaces and implementations for TensorFlow.

load("//third_party/tensorflow/core/platform:rules_cc.bzl", "cc_library")
load("//third_party/tensorflow:tensorflow.google.bzl", "tf_grpc_cc_dependencies")

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
    ] + tf_grpc_cc_dependencies(),
)
