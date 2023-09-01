# Description:
#   Implementation of custom numpy floats.

package(
    # copybara:uncomment default_applicable_licenses = ["//tensorflow:license"],
    default_visibility = [
        "//visibility:public",
    ],
    features = [
        # For ml_dtypes.so (see b/259896740)
        "windows_export_all_symbols",
    ],
    licenses = ["notice"],
)

filegroup(
    name = "numpy_hdr",
    srcs = ["numpy.h"],
)

filegroup(
    name = "basic_hdrs",
    srcs = ["numpy.h"],
)

cc_library(
    name = "ml_dtypes_lib",
    srcs = ["ml_dtypes.cc"],
    hdrs = ["ml_dtypes.h"],
    copts = ["-fexceptions"],  # Required for pybind11.
    # Requires data dependency in order to load py module from C++.
    data = [
        "@ml_dtypes",
    ],
    features = [
        "-use_header_modules",  # Required for pybind11.
        "-parse_headers",
    ],
    deps = [
        "//third_party/python_runtime:headers",  # build_cleaner: keep; DNR: b/35864863
        "//tsl/python/lib/core:numpy",
        "@pybind11",
    ],
)

cc_library(
    name = "numpy",
    srcs = ["numpy.cc"],
    hdrs = ["numpy.h"],
    deps = [
        "//third_party/py/numpy:headers",
        "//third_party/python_runtime:headers",
    ],
)
