# Description:
# Tools for source code management etc.

package(
    default_visibility = ["//third_party/tensorflow:internal"],
    licenses = ["notice"],
)

# Export this file to run sh_tests from other Tensorflow BUILD files.
exports_files(["memcheck_runner.sh"])

sh_binary(
    name = "memcheck_runner",
    srcs = ["memcheck_runner.sh"],
)

sh_test(
    name = "memcheck_runner_test",
    srcs = ["memcheck_runner_test.sh"],
    data = [
        ":memcheck_runner",
        "//util/shell/gbash:unit",
    ],
)
