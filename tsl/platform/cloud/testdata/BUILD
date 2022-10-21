# Cloud test data files.

load("//third_party/tensorflow:tensorflow.google.bzl", "filegroup")

package(
    licenses = ["notice"],
)

filegroup(
    name = "application_default_credentials",
    srcs = [
        "application_default_credentials.json",
    ],
    visibility = ["//tsl/platform/cloud:__pkg__"],
)

filegroup(
    name = "service_account_credentials",
    srcs = [
        "service_account_credentials.json",
    ],
    visibility = ["//tsl/platform/cloud:__pkg__"],
)

filegroup(
    name = "service_account_public_key",
    srcs = [
        "service_account_public_key.txt",
    ],
    visibility = ["//tsl/platform/cloud:__pkg__"],
)
