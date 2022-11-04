load("//tsl/platform:build_config.bzl", "tf_proto_library")

package_group(
    name = "friends",
    includes = [
        "//tsl/profiler:friends",
    ],
)

tf_proto_library(
    name = "xplane_proto",
    srcs = ["xplane.proto"],
    cc_api_version = 2,
    make_default_target_header_only = True,
    visibility = [":friends"],
)

# This is needed because of how tf_android_core_proto_sources parses proto paths.
exports_files(
    srcs = ["xplane.proto"],
    visibility = [
        "//third_party/tensorflow/core:__pkg__",
        "//tsl:__pkg__",
    ],
)

tf_proto_library(
    name = "profile_proto",
    srcs = ["profile.proto"],
    cc_api_version = 2,
    visibility = [
        "//third_party/tensorflow/compiler/xla/python:__pkg__",
        "//third_party/tensorflow/core/profiler:__pkg__",
    ],
)

tf_proto_library(
    name = "protos_all",
    create_go_proto = False,
    make_default_target_header_only = True,
    protodeps = [
        ":xplane_proto",
    ],
    visibility = ["//visibility:public"],
)

# copybara:uncomment_begin(google-only)
# py_proto_library(
#     name = "xplane_py_pb2",
#     api_version = 2,
#     visibility = [":friends"],
#     deps = [":xplane_proto"],
# )
#
# copybara:uncomment_end
