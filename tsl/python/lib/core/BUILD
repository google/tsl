# TODO(zacmustin): describe this package.
# TODO(zacmustin): branch this file properly from tf/python/lib/core/BUILD. This is just to avoid
# lots of annoying merge conflicts.
load("//tsl:tsl.default.bzl", "tsl_pybind_extension")
load("//tsl/platform:build_config_root.bzl", "if_static")

package(
    default_visibility = [
        "//visibility:public",
    ],
    licenses = ["notice"],
)

filegroup(
    name = "numpy_hdr",
    srcs = ["numpy.h"],
)

filegroup(
    name = "basic_hdrs",
    srcs = [
        "numpy.h",
    ],
)

cc_library(
    name = "numpy",
    hdrs = ["numpy.h"],
    deps = [
        "//third_party/py/numpy:headers",
        "//third_party/python_runtime:headers",
    ] + if_static([":numpy_impl"]),
)

cc_library(
    name = "numpy_impl",
    srcs = ["numpy.cc"],
    hdrs = ["numpy.h"],
    deps = [
        "//third_party/py/numpy:headers",
        "//third_party/python_runtime:headers",
    ],
)

cc_library(
    name = "custom_float",
    hdrs = [
        "custom_float.h",
    ],
    deps = [
        ":numpy",
        "//third_party/eigen3",
        "//third_party/python_runtime:headers",  # build_cleaner: keep; DNR: b/35864863
        "//tsl/platform:logging",
        "//tsl/platform:types",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "bfloat16_lib",
    srcs = [
        "bfloat16.cc",
        "float8_e4m3b11.cc",
    ],
    hdrs = [
        "bfloat16.h",
        "float8_e4m3b11.h",
    ],
    deps = [
        ":custom_float",
        "//third_party/eigen3",
        "//third_party/python_runtime:headers",  # build_cleaner: keep; DNR: b/35864863
        "//tsl/platform:logging",
        "//tsl/platform:types",
        "@com_google_absl//absl/strings",
    ],
)

tsl_pybind_extension(
    name = "_pywrap_bfloat16",
    srcs = ["bfloat16_wrapper.cc"],
    deps = [
        ":bfloat16_lib",
        ":numpy_impl",
        "//third_party/pybind11",
        "//third_party/python_runtime:headers",
    ],
)

filegroup(
    name = "bfloat16_hdr",
    srcs = ["bfloat16.h"],
)
