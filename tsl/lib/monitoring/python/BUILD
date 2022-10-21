# Python bindings for tsl/lib/monitoring.
load("//third_party/tensorflow:tensorflow.google.bzl", "pybind_extension")

package(
    licenses = ["notice"],
)

pybind_extension(
    name = "monitoring",
    srcs = [
        "monitoring.cc",
    ],
    pytype_srcs = ["__init__.pyi"],
    visibility = [
        "//third_party/py/jax:__subpackages__",
        "//third_party/py/paxml:__subpackages__",
    ],
    deps = [
        "//third_party/pybind11",
        "//tsl/lib/monitoring:sampler",
        "//tsl/platform:env_impl",
        "//tsl/protobuf:histogram_proto_cc_impl",
        "@com_google_absl//absl/memory",
    ],
)

py_test(
    name = "monitoring_test",
    srcs = ["monitoring_test.py"],
    deps = [
        "//third_party/py/absl/testing:absltest",
        "//tsl/lib/monitoring/python:monitoring",
    ],
)
