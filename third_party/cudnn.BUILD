load("@implib_so//:implib_gen.bzl", "implib_gen")
load("@rules_license//rules:license.bzl", "license")

package(
    default_applicable_licenses = [":license"],
    default_visibility = ["//visibility:public"],
)

# See https://docs.nvidia.com/deeplearning/cudnn/sla
licenses(["by_exception_only"])

license(
    name = "license",
    package_name = "cudnn",
    # The license is 'by_exception_only' since it's a custom NVIDIA SLA.
    license_kinds = ["@rules_license//licenses/generic:by_exception_only"],
)

# cuDNN headers-only target.
# There is no need to depend on this and :cudnn.
cc_library(
    name = "cudnn_header",
    hdrs = glob(["include/**"]),
    strip_include_prefix = "include",
    deps = ["@cuda//:cuda_headers"],
)

# This target dynamically loads the actual libcudnn.so.
cc_library(
    name = "cudnn",
    srcs = [":implib_gen"],
    hdrs = glob(["include/**"]),
    data = glob(["lib/*.so*"]),
    linkopts = ["-ldl"],
    linkstatic = True,
    strip_include_prefix = "include",
    deps = ["@cuda//:cuda_headers"],
)

implib_gen(
    name = "implib_gen",
    error_value = "CUDNN_STATUS_NOT_INITIALIZED",
    header = "cudnn.h",
    package = "nvidia.cudnn.lib",
    shared_library = "lib/libcudnn.so.8",
    visibility = ["//visibility:private"],
)
