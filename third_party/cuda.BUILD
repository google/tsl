load("@implib_so//:implib_gen.bzl", "implib_gen")
load("@rules_license//rules:license.bzl", "license")

package(
    default_applicable_licenses = [":license"],
    default_visibility = ["//visibility:public"],
)

# See https://docs.nvidia.com/cuda/eula/
licenses(["by_exception_only"])

license(
    name = "license",
    package_name = "cuda",
    # The license is 'by_exception_only' since it's a custom NVIDIA EULA.
    license_kinds = ["@rules_license//licenses/generic:by_exception_only"],
)

# CUDA headers-only target.
# There is no need to depend on this and any of the targets below.
cc_library(
    name = "cuda_headers",
    hdrs = glob(["include/**"]),
    strip_include_prefix = "include",
)

cc_library(
    name = "cuda_runtime_static",
    srcs = ["lib/libcudart_static.a"],
    hdrs = glob(["include/**"]),
    strip_include_prefix = "include",
)

[
    (
        # These targets dynamically load the actual shared library.
        cc_library(
            name = name,
            srcs = [":{}_implib_gen".format(name)],
            hdrs = glob(["include/**"]),
            linkopts = [
                "-ldl",
                # Find shared library in the sibling site-package.
                "-Wl,-rpath=../nvidia/{}/lib".format(name),
            ],
            strip_include_prefix = "include",
        ),
    )
    for name in [
        "cuda",
        "cublas",
        "cusolver",
        "cufft",
    ]
]

implib_gen(
    name = "cuda_implib_gen",
    error_value = "CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND",
    header = "cuda.h",
    shared_library = "lib/stubs/libcuda.so",
    visibility = ["//visibility:private"],
)

implib_gen(
    name = "cublas_implib_gen",
    error_value = "CUBLAS_STATUS_NOT_INITIALIZED",
    header = "cublas.h",
    shared_library = "lib/libcublas.so",
    visibility = ["//visibility:private"],
)

implib_gen(
    name = "cusolver_implib_gen",
    error_value = "CUSOLVER_STATUS_NOT_INITIALIZED",
    header = "cusolver_common.h",
    shared_library = "lib/libcusolver.so",
    visibility = ["//visibility:private"],
)

implib_gen(
    name = "cufft_implib_gen",
    error_value = "CUFFT_SETUP_FAILED",
    header = "cufft.h",
    shared_library = "lib/libcufft.so",
    visibility = ["//visibility:private"],
)
