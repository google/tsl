load(":dependencies.bzl", "dependencies")

extensions = module_extension(
    implementation = lambda ctx: dependencies(),
)
