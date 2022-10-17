load(
    "//tsl/platform:rules_cc.bzl",
    "cc_library",
)
load("//tsl:tsl.default.bzl", "filegroup")

package(
    default_visibility = [
        "//visibility:public",
    ],
    licenses = ["notice"],
)

cc_library(
    name = "byte_swap_array",
    srcs = ["byte_swap_array.cc"],
    hdrs = ["byte_swap_array.h"],
    deps = [
        "//tsl/platform:byte_order",
        "//tsl/platform:errors",
        "//tsl/platform:status",
    ],
)

filegroup(
    name = "mobile_srcs",
    srcs = [
        "byte_swap_array.cc",
        "byte_swap_array.h",
    ],
)
