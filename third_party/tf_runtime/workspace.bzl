"""Provides the repository macro to import TFRT."""

load("//third_party:repo.bzl", "tf_http_archive", "tf_mirror_urls")

def repo():
    """Imports TFRT."""

    # DO_NOT_SUBMIT
    tf_http_archive(
        name = "platforms",
        sha256 = "5308fc1d8865406a49427ba24a9ab53087f17f5266a7aabbfc28823f3916e1ca",
        urls = tf_mirror_urls("https://github.com/bazelbuild/platforms/releases/download/0.0.6/platforms-0.0.6.tar.gz"),
    )

    # Attention: tools parse and update these lines.
    TFRT_COMMIT = "5a3ff2087ab590e6ac9c839c9dc43e520891b7de"
    TFRT_SHA256 = "70884650bb2fc4440c3a2db8cccf175a1895daedf5fd7f0f29d4767c4f0cb124"

    tf_http_archive(
        name = "tf_runtime",
        sha256 = TFRT_SHA256,
        strip_prefix = "runtime-{commit}".format(commit = TFRT_COMMIT),
        urls = tf_mirror_urls("https://github.com/tensorflow/runtime/archive/{commit}.tar.gz".format(commit = TFRT_COMMIT)),
        # A patch file can be provided for atomic commits to both TF and TFRT.
        # The job that bumps the TFRT_COMMIT also resets patch_file to 'None'.
        patch_file = None,
    )
