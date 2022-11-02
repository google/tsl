load("//tsl/platform:rules_cc.bzl", "cc_library")
load("//tsl/platform:build_config.bzl", "tsl_cc_test")

package(
    default_visibility = [
        "//tsl:internal",
    ],
    licenses = ["notice"],
)

cc_library(
    name = "preemption_notifier",
    srcs = ["preemption_notifier.cc"],
    hdrs = ["preemption_notifier.h"],
    # copybara:uncomment compatible_with = ["//buildenv/target:gce"],
    deps = [
        "//tsl/platform:env",
        "//tsl/platform:errors",
        "//tsl/platform:mutex",
        "//tsl/platform:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/time",
    ],
)

tsl_cc_test(
    name = "preemption_notifier_test",
    size = "small",
    srcs = ["preemption_notifier_test.cc"],
    deps = [
        ":preemption_notifier",
        "//tsl/platform:env",
        "//tsl/platform:env_impl",
        "//tsl/platform:errors",
        "//tsl/platform:mutex",
        "//tsl/platform:status",
        "//tsl/platform:statusor",
        "//tsl/platform:test",
        "//tsl/platform:test_main",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/time",
    ],
)

cc_library(
    name = "preemption_sync_manager",
    srcs = ["preemption_sync_manager.cc"],
    hdrs = ["preemption_sync_manager.h"],
    deps = [
        ":preemption_notifier",
        "//tsl/distributed_runtime:call_options",
        "//tsl/distributed_runtime/coordination:coordination_service_agent",
        "//tsl/platform:env",
        "//tsl/platform:mutex",
        "//tsl/platform:status",
        "//tsl/platform:statusor",
        "//tsl/protobuf:coordination_service_proto_cc",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/time",
    ],
)
