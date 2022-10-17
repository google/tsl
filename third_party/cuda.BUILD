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
    linkopts = [
        "-ldl",
        "-lrt",
        "-lpthread",
    ],
    strip_include_prefix = "include",
    deps = [":cuda"],
)

[
    (
        # These targets dynamically load the actual shared library.
        cc_library(
            name = name,
            srcs = [":{}_implib_gen".format(name)],
            hdrs = glob(["include/**"]),
            data = glob(["lib/*.so"]),
            linkopts = ["-ldl"],
            linkstatic = True,
            strip_include_prefix = "include",
        ),
    )
    for name in [
        "cuda",
        "cublas",
        "cufft",
        "curand",
        "cusolver",
    ]
]

implib_gen(
    name = "cuda_implib_gen",
    error_value = "CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND",
    header = "cuda.h",
    shared_library = "lib/stubs/libcuda.so",
    # Load the driver's libcuda.so.1, not the stub from runfiles or package.
    use_runfile = False,
    visibility = ["//visibility:private"],
)

implib_gen(
    name = "cublas_implib_gen",
    error_value = "CUBLAS_STATUS_NOT_INITIALIZED",
    header = "cublas.h",
    package = "nvidia.cublas.lib",
    shared_library = "lib/libcublas.so",
    visibility = ["//visibility:private"],
)

implib_gen(
    name = "cufft_implib_gen",
    error_value = "CUFFT_SETUP_FAILED",
    header = "cufft.h",
    package = "nvidia.cufft.lib",
    shared_library = "lib/libcufft.so",
    visibility = ["//visibility:private"],
)

implib_gen(
    name = "curand_implib_gen",
    error_value = "CURAND_STATUS_INITIALIZATION_FAILED",
    header = "curand.h",
    package = "nvidia.curand.lib",
    shared_library = "lib/libcurand.so",
    visibility = ["//visibility:private"],
)

implib_gen(
    name = "cusolver_implib_gen",
    error_value = "CUSOLVER_STATUS_NOT_INITIALIZED",
    header = "cusolver_common.h",
    package = "nvidia.cusolver.lib",
    shared_library = "lib/libcusolver.so",
    visibility = ["//visibility:private"],
)
