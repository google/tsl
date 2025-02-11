# NVIDIA NCCL 2
# A package of optimized primitives for collective multi-GPU communication.

load("@bazel_skylib//rules:expand_template.bzl", "expand_template")
load("@bazel_skylib//rules:write_file.bzl", "write_file")
load(
    "@local_config_cuda//cuda:build_defs.bzl",
    "cuda_library",
)
load(
    "@local_config_nccl//:build_defs.bzl",
    "cuda_rdc_library",
)
load(
    "@local_config_nccl//:generated_names.bzl",
    "GENERATED_SOURCES",
)

licenses(["notice"])

exports_files(["LICENSE.txt"])

NCCL_MAJOR = 2

NCCL_MINOR = 25

NCCL_PATCH = 1

NCCL_VERSION = NCCL_MAJOR * 10000 + NCCL_MINOR * 100 + NCCL_PATCH  # e.g., 21605

expand_template(
    name = "nccl_header_version",
    out = "src/nccl.h",
    substitutions = {
        "${nccl:Major}": str(NCCL_MAJOR),
        "${nccl:Minor}": str(NCCL_MINOR),
        "${nccl:Patch}": str(NCCL_PATCH),
        "${nccl:Suffix}": "\"\"",
        "${nccl:Version}": str(NCCL_VERSION),
    },
    template = "src/nccl.h.in",
)

# This additional header allows us to determine the configured NCCL version
# without including the rest of NCCL.
write_file(
    name = "nccl_config_header",
    out = "nccl_config.h",
    content = [
        "#define TF_NCCL_VERSION \"{}\"".format(NCCL_MAJOR),
    ],
)

cc_library(
    name = "nccl_config",
    hdrs = ["nccl_config.h"],
    include_prefix = "third_party/nccl",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "src_hdrs",
    hdrs = [
        "src/include/collectives.h",
        "src/nccl.h",
    ],
    strip_include_prefix = "src",
)

cc_library(
    name = "include_hdrs",
    hdrs = glob(["src/include/**"]),
    strip_include_prefix = "src/include",
    deps = ["@local_config_cuda//cuda:cuda_headers"],
)

cc_library(
    name = "device_hdrs",
    hdrs = glob(["src/device/**/*.h"]),
    strip_include_prefix = "src/device",
)

py_binary(
    name = "generate",
    srcs = ["src/device/generate.py"],
    python_version = "PY3",
)

genrule(
    name = "generated_srcs",
    srcs = [],
    outs = ["result.tar"],
    cmd = """
    mkdir -p src/device/generated
    $(location :generate) src/device/generated
    tar -cf $@ src
    """,
    tools = [":generate"],
)

genrule(
    name = "generated_sources",
    srcs = ["generated_srcs"],
    outs = ["generated_names.bzl"],
    cmd = """
    echo '"List of sources generated by :generate_nccl_kernels"' > $@
    echo "GENERATED_SOURCES = [" >> $@
    tar --list -f $< | grep '.cc' |  sort | sed -e 's/\\(.*\\)/    "\\1",/' >> $@
    echo "]" >> $@
    """,
)

EXTRACT_CMD = """
    set -x
    OUTDIR=$$(mktemp -d)
    tar -C $$OUTDIR -xf $(location :generated_srcs)
    for outf in $(OUTS); do
      F=$$(echo $$outf | sed -e 's@.*/src/device/generated/@@')
      mv $$OUTDIR/src/device/generated/$$F $$outf
    done
"""

genrule(
    name = "generated_files",
    srcs = [":generated_srcs"],
    outs = GENERATED_SOURCES,
    cmd = EXTRACT_CMD,
)

cuda_rdc_library(
    name = "device",
    srcs = [
        ":generated_files",
    ] + glob(include = [
        "src/device/**/*.cu.cc",
    ]),
    deps = [
        ":device_hdrs",
        ":include_hdrs",
        ":src_hdrs",
        "@local_config_cuda//cuda:cuda_headers",
    ],
)

cc_library(
    name = "net",
    srcs = [
        "src/transport/coll_net.cc",
        "src/transport/net.cc",
    ],
    linkopts = ["-lrt"],
    deps = [
        ":include_hdrs",
        ":src_hdrs",
    ],
)

cc_library(
    name = "nccl_via_stub",
    hdrs = ["src/nccl.h"],
    include_prefix = "third_party/nccl",
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
    deps = [
        "@local_config_cuda//cuda:cuda_headers",
        "@xla//xla/tsl/cuda:nccl_stub",
    ],
)

cc_library(
    name = "nccl_headers",
    hdrs = ["src/nccl.h"],
    include_prefix = "third_party/nccl",
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
    deps = [
        "@local_config_cuda//cuda:cuda_headers",
    ],
)

cc_library(
    name = "nccl",
    srcs = glob(
        include = [
            "src/**/*.cc",
            # Required for header inclusion checking, see below for details.
            "src/graph/*.h",
        ],
        # Exclude device-library code.
        exclude = [
            "src/device/**",
            "src/transport/coll_net.cc",
            "src/transport/net.cc",
            "src/enqueue.cc",
        ],
    ) + [
        # Required for header inclusion checking (see
        # http://docs.bazel.build/versions/master/be/c-cpp.html#hdrs).
        # Files in src/ which #include "nccl.h" load it from there rather than
        # from the virtual includes directory.
        "src/include/collectives.h",
        "src/nccl.h",
    ],
    hdrs = ["src/nccl.h"],
    include_prefix = "third_party/nccl",
    linkopts = ["-lrt"],
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
    deps = [
        ":device",
        ":enqueue",
        ":include_hdrs",
        ":net",
        ":src_hdrs",
    ],
)

alias(
    name = "enqueue",
    actual = select({
        "@local_config_cuda//cuda:using_clang": ":enqueue_clang",
        "@local_config_cuda//cuda:using_nvcc": ":enqueue_nvcc",
    }),
)

# Kernels and their names have special treatment in CUDA compilation.
# Specifically, the host-side kernel launch stub (host-side representation of
# the kernel) ends up having the name which does not match the actual kernel
# name. In order to correctly refer to the kernel the referring code must be
# compiled as CUDA.
cuda_library(
    name = "enqueue_clang",
    srcs = [
        "src/enqueue.cc",
    ],
    hdrs = ["src/nccl.h"],
    copts = [
        "--cuda-host-only",
    ],
    include_prefix = "third_party/nccl",
    linkopts = ["-lrt"],
    # The following definition is needed to enable placeholder literals such as
    # PRIx64 defined at the inttypes.h since Tensorflow docker image uses
    # an old version of glibc.
    local_defines = ["__STDC_FORMAT_MACROS"],
    strip_include_prefix = "src",
    target_compatible_with = select({
        "@local_config_cuda//cuda:using_clang": [],
        "//conditions:default": ["@platforms//:incompatible"],
    }),
    visibility = ["//visibility:public"],
    deps = [
        ":device",
        ":include_hdrs",
        ":src_hdrs",
    ],
)

cc_library(
    name = "enqueue_nvcc",
    srcs = [
        "src/enqueue.cc",
    ],
    hdrs = ["src/nccl.h"],
    include_prefix = "third_party/nccl",
    linkopts = ["-lrt"],
    strip_include_prefix = "src",
    target_compatible_with = select({
        "@local_config_cuda//cuda:using_nvcc": [],
        "//conditions:default": ["@platforms//:incompatible"],
    }),
    visibility = ["//visibility:public"],
    deps = [
        ":device",
        ":include_hdrs",
        ":src_hdrs",
    ],
)
