exports_files(["dlopen.cc"])

py_binary(
    name = "implib-gen",
    srcs = ["implib-gen.py"],
    data = glob(["arch/**"]),
    visibility = ["//visibility:public"],
)
