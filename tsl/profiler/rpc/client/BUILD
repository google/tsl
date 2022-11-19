load("//tsl/platform:rules_cc.bzl", "cc_library")
load("//third_party/tensorflow:tensorflow.google.bzl", "tf_grpc_cc_dependencies")
load("//third_party/tensorflow:tensorflow.bzl", "tf_cc_test")
load(
    "//third_party/tensorflow/core/profiler/builds:build_config.bzl",
    "tf_profiler_copts",
    "tf_profiler_pybind_cc_library_wrapper",
)

# For platform specific build config
load(
    "//tsl/platform:build_config.bzl",
    "tf_protos_profiler_service",
)

package(
    # copybara:uncomment default_applicable_licenses = ["//third_party/tensorflow:license"],
    default_visibility = [
        "//third_party/tensorflow/core/profiler/rpc/client:__pkg__",
    ],
    licenses = ["notice"],
)

cc_library(
    name = "capture_profile",
    srcs = ["capture_profile.cc"],
    hdrs = ["capture_profile.h"],
    copts = tf_profiler_copts(),
    visibility = [
        "//third_party/tensorflow/compiler/xla/python:__pkg__",
        "//third_party/tensorflow/core/profiler/rpc/client:__pkg__",
        "//third_party/tensorflow/python/profiler/internal:__pkg__",
    ],
    deps = [
        ":profiler_client_for_pybind",
        ":remote_profiler_session_manager",
        ":save_profile",
        "//tsl/platform:errors",
        "//tsl/platform:platform_port",
        "//tsl/platform:status",
        "//tsl/platform:types",
        "//tsl/profiler/protobuf:profiler_analysis_proto_cc",
        "//tsl/profiler/protobuf:profiler_options_proto_cc",
        "//tsl/profiler/protobuf:profiler_service_proto_cc",
        "//tsl/profiler/protobuf:xplane_proto_cc",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ],
)

cc_library(
    name = "save_profile",
    srcs = ["save_profile.cc"],
    hdrs = ["save_profile.h"],
    copts = tf_profiler_copts(),
    visibility = [
        "//third_party/tensorflow/core/profiler:internal",
        "//third_party/tensorflow/core/profiler/rpc/client:__pkg__",
        "//tsl/profiler/rpc:__pkg__",
    ],
    deps = [
        "//third_party/tensorflow/core:framework",
        "//tsl/lib/io:zlib_compression_options",
        "//tsl/lib/io:zlib_outputbuffer",
        "//tsl/platform:env",
        "//tsl/platform:errors",
        "//tsl/platform:logging",
        "//tsl/platform:status",
        "//tsl/platform:types",
        "//tsl/profiler/protobuf:profiler_service_proto_cc",
        "//tsl/profiler/protobuf:xplane_proto_cc",
        "//tsl/profiler/utils:file_system_utils",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ],
)

tf_profiler_pybind_cc_library_wrapper(
    name = "profiler_client_for_pybind",
    actual = ":profiler_client",
    visibility = [
        "//third_party/tensorflow/core/profiler/rpc/client:__pkg__",
        "//third_party/tensorflow/python/profiler/internal:__pkg__",
    ],
)

cc_library(
    name = "profiler_client",
    hdrs = ["profiler_client.h"],
    visibility = [
        "//third_party/tensorflow/compiler/xla:__subpackages__",
        "//third_party/tensorflow/core/profiler/rpc/client:__pkg__",
        "//third_party/tensorflow/python/profiler/internal:__pkg__",
    ],
    deps = [
        ":profiler_client_impl",
        "//tsl/platform:status",
        "//tsl/profiler/protobuf:profiler_analysis_cc_grpc_proto",
        "//tsl/profiler/protobuf:profiler_service_cc_grpc_proto",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ],
)

# Linked to pywrap_tensorflow to avoid ODR violation due to tf_grpc_cc_dependencies().
cc_library(
    name = "profiler_client_impl",
    srcs = [
        "profiler_client.cc",
        "profiler_client.h",
    ],
    copts = tf_profiler_copts(),
    visibility = [
        "//third_party/tensorflow/core/profiler/rpc/client:__pkg__",
        "//third_party/tensorflow/python:__pkg__",
        "//third_party/tensorflow/python/profiler/internal:__pkg__",
    ],
    deps = [
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "//tsl/platform:errors",
        "//tsl/platform:logging",
        "//tsl/platform:status",
        "//tsl/platform:types",
        "//tsl/profiler/protobuf:profiler_analysis_cc_grpc_proto",
        "//tsl/profiler/protobuf:profiler_service_cc_grpc_proto",
        "//tsl/protobuf:error_codes_proto_impl_cc",
    ] + tf_grpc_cc_dependencies(),
    alwayslink = True,
)

cc_library(
    name = "profiler_client_test_util",
    testonly = 1,
    hdrs = ["profiler_client_test_util.h"],
    deps = [
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "//third_party/tensorflow/core/profiler/lib:profiler_session",
        "//third_party/tensorflow/core/profiler/rpc:profiler_server_impl",
        "//tsl/platform:test",
        "//tsl/platform:logging",
        "//tsl/platform:types",
        "//tsl/profiler/protobuf:profiler_options_proto_cc",
    ] + tf_protos_profiler_service(),
)

tf_cc_test(
    name = "profiler_client_test",
    srcs = ["profiler_client_test.cc"],
    deps = [
        ":profiler_client",
        ":profiler_client_impl",  # for oss
        ":profiler_client_test_util",
        "@com_google_absl//absl/time",
        "//tsl/platform:errors",
        "//tsl/platform:status",
        "//tsl/platform:test",
        "//tsl/platform:test_main",
        "//tsl/platform:types",
    ] + tf_protos_profiler_service(),
)

cc_library(
    name = "remote_profiler_session_manager",
    srcs = ["remote_profiler_session_manager.cc"],
    hdrs = ["remote_profiler_session_manager.h"],
    copts = tf_profiler_copts(),
    deps = [
        ":profiler_client_for_pybind",
        "//tsl/platform:env_time",
        "//tsl/platform:errors",
        "//tsl/platform:logging",
        "//tsl/platform:macros",
        "//tsl/platform:mutex",
        "//tsl/platform:status",
        "//tsl/platform:thread_annotations",
        "//tsl/platform:types",
        "//tsl/profiler/utils:time_utils",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ],
)

tf_cc_test(
    name = "remote_profiler_session_manager_test",
    srcs = ["remote_profiler_session_manager_test.cc"],
    deps = [
        ":profiler_client_impl",  # for oss
        ":profiler_client_test_util",
        ":remote_profiler_session_manager",
        "@com_google_absl//absl/time",
        "//third_party/tensorflow/core/profiler/rpc:profiler_server_impl",
        "//tsl/platform:errors",
        "//tsl/platform:status",
        "//tsl/platform:test",
        "//tsl/platform:test_main",
        "//tsl/platform:types",
        "//tsl/profiler/protobuf:profiler_options_proto_cc",
    ] + tf_protos_profiler_service(),
)
