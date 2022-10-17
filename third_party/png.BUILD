# Description:
#   libpng is the official PNG reference library.

licenses(["notice"])  # BSD/MIT-like license

exports_files(["LICENSE"])

cc_library(
    name = "png",
    srcs = [
        "png.c",
        "pngdebug.h",
        "pngerror.c",
        "pngget.c",
        "pnginfo.h",
        "pnglibconf.h",
        "pngmem.c",
        "pngpread.c",
        "pngpriv.h",
        "pngread.c",
        "pngrio.c",
        "pngrtran.c",
        "pngrutil.c",
        "pngset.c",
        "pngstruct.h",
        "pngtrans.c",
        "pngwio.c",
        "pngwrite.c",
        "pngwtran.c",
        "pngwutil.c",
    ] + select({
        "@platforms//os:windows": [
            "intel/intel_init.c",
            "intel/filter_sse2_intrinsics.c",
        ],
        "@platforms//cpu:ppc": [
            "powerpc/powerpc_init.c",
            "powerpc/filter_vsx_intrinsics.c",
        ],
        "//conditions:default": [
        ],
    }),
    hdrs = [
        "png.h",
        "pngconf.h",
    ],
    copts = select({
        "@platforms//os:windows": ["-DPNG_INTEL_SSE_OPT=1"],
        "//conditions:default": [],
    }),
    includes = ["."],
    linkopts = select({
        "@platforms//os:windows": [],
        "//conditions:default": ["-lm"],
    }),
    visibility = ["//visibility:public"],
    deps = ["@zlib"],
)

genrule(
    name = "snappy_stubs_public_h",
    srcs = ["scripts/pnglibconf.h.prebuilt"],
    outs = ["pnglibconf.h"],
    cmd = "sed -e 's/PNG_ZLIB_VERNUM 0/PNG_ZLIB_VERNUM 0x12c0/' $< >$@",
)
