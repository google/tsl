load(":dependencies.bzl", "dependencies")

def workspace():
    dependencies()

    # TODO(csigg): Add http_archive()s for MODULE.bazel bazel_dep()s here.
    fail("Please build with --experimental_enable_bzlmod")
