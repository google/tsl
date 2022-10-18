"""Macro to create bazel repositories for external dependencies."""

def dependencies():
    # When adding or updating a dependency, please also update the mirror:
    # /google/bin/releases/tensorflow-devinfra-team/cli_tools/tf_mirror <url>

    http_archive(
        name = "benchmark",
        urls = mirror_urls("https://github.com/google/benchmark/archive/0baacde3618ca617da95375e0af13ce1baadea47.tar.gz"),
        integrity = "sha256-C5IaO8OeNfQnXI3MZYryORwVD7lmECNBKHsEAf8ubyE=",
        strip_prefix = "benchmark-0baacde3618ca617da95375e0af13ce1baadea47",
        symlinks = {"//third_party:benchmark.BUILD": "BUILD"},
    )

    http_archive(
        name = "farmhash",
        urls = mirror_urls("https://github.com/google/farmhash/archive/0d859a811870d10f53a594927d0d0b97573ad06d.tar.gz"),
        integrity = "sha256-GDks8HNuHWLsu41pXDFJa2UHhZ6MdVQdetC6CS3FIRU=",
        strip_prefix = "farmhash-0d859a811870d10f53a594927d0d0b97573ad06d",
        symlinks = {"//third_party:farmhash.BUILD": "BUILD"},
    )

    http_archive(
        name = "fft2d",
        urls = mirror_urls("https://github.com/petewarden/OouraFFT/archive/v1.0.tar.gz"),
        integrity = "sha256-X02rwq4h4fU3Ql1YpJzcocSeoR2w1iceKksn6Wl1SOs=",
        strip_prefix = "OouraFFT-1.0",
        symlinks = {"//third_party:fft2d.BUILD": "BUILD"},
    )

    http_archive(
        name = "gif",
        urls = mirror_urls("http://pilotfiber.dl.sourceforge.net/project/giflib/giflib-5.2.1.tar.gz"),
        integrity = "sha256-MdpVYvRMXxXWM0Cgmk/WK0jEViDNMC93ptms8Ad4eb0=",
        strip_prefix = "giflib-5.2.1",
        symlinks = {"//third_party:gif.BUILD": "BUILD"},
        patches = ["//third_party:gif.patch"],
    )

    http_archive(
        name = "highwayhash",
        urls = mirror_urls("https://github.com/google/highwayhash/archive/c13d28517a4db259d738ea4886b1f00352a3cc33.tar.gz"),
        integrity = "sha256-wOK5kx+8zjv7zXmZw8EU9ASsD4uJd1pbvMvKpQGGjlg=",
        strip_prefix = "highwayhash-c13d28517a4db259d738ea4886b1f00352a3cc33",
        symlinks = {"//third_party:highwayhash.BUILD": "BUILD"},
    )

    http_archive(
        name = "hwloc",
        urls = mirror_urls("https://download.open-mpi.org/release/hwloc/v2.7/hwloc-2.7.1.tar.gz"),
        integrity = "sha256-TLCnge2YCwOtjEi+tXQHqmfEuQjkVyKVS5cwN5vH9tU=",
        strip_prefix = "hwloc-2.7.1",
        symlinks = {
            "//third_party:hwloc.BUILD": "BUILD",
            "//third_party:hwloc.build_defs.bzl": "build_defs.bzl",
            "//third_party:hwloc.static-components.h": "hwloc/static-components.h",
        },
    )

    http_archive(
        name = "implib_so",
        urls = mirror_urls("https://github.com/yugr/Implib.so/archive/1c65670f2ffba77d1985a8b32e3f4529fe70291e.tar.gz"),
        integrity = "sha256-0MqCHgjKUcBBhfQtWabFztKCIJcDAcgQ46iWqsXNXRo=",
        strip_prefix = "Implib.so-1c65670f2ffba77d1985a8b32e3f4529fe70291e",
        symlinks = {
            "//third_party:implib_so.BUILD": "BUILD",
            "//third_party:implib_so.dlopen.cc": "dlopen.cc",
            "//third_party:implib_so.implib_gen.bzl": "implib_gen.bzl",
        },
    )

    http_archive(
        name = "libjpeg_turbo",
        urls = mirror_urls("https://github.com/libjpeg-turbo/libjpeg-turbo/archive/2.1.0.tar.gz"),
        integrity = "sha256-1rd5CSfWWBCN/TvuLwxmopJMUe5/nckw9ixFL0pjjFI=",
        strip_prefix = "libjpeg-turbo-2.1.0",
        symlinks = {"//third_party:libjpeg_turbo.BUILD": "BUILD"},
    )

    http_archive(
        name = "nasm",
        urls = mirror_urls("https://www.nasm.us/pub/nasm/releasebuilds/2.14.02/nasm-2.14.02.tar.bz2"),
        integrity = "sha256-NP0mxwonep/dVMtezzibre2vSAR7Jp0QCPvIGbJOgLw=",
        strip_prefix = "nasm-2.14.02",
        symlinks = {
            "//third_party:nasm.BUILD": "BUILD",
            "//third_party:nasm.config.h": "config/config.h",
        },
    )

    http_archive(
        name = "nsync",
        urls = mirror_urls("https://github.com/google/nsync/archive/1.25.0.tar.gz"),
        integrity = "sha256-K+nb/M5BfHq8wqpv7jUc1NKSUY1pJXfnSixsBbBJ5EI=",
        strip_prefix = "nsync-1.25.0",
        patches = ["//third_party:nsync.patch"],
    )

    http_archive(
        name = "png",
        urls = mirror_urls("https://github.com/glennrp/libpng/archive/v1.6.37.tar.gz"),
        integrity = "sha256-ynSg2s4XmoQiGHZxrul904krU+FoYnFFJxytW1rIEwc=",
        strip_prefix = "libpng-1.6.37",
        symlinks = {"//third_party:png.BUILD": "BUILD"},
        patches = ["//third_party:png.patch"],
    )

    http_archive(
        name = "snappy",
        urls = mirror_urls("https://github.com/google/snappy/archive/1.1.8.tar.gz"),
        integrity = "sha256-FrZ38HgyphKwg2F42383TkFPlGV8E45pk8v8XcxYZR8=",
        strip_prefix = "snappy-1.1.8",
        symlinks = {"//third_party:snappy.BUILD": "BUILD"},
    )

    http_archive(
        name = "cudnn",
        urls = mirror_urls("https://developer.download.nvidia.com/compute/redist/cudnn/v8.6.0/local_installers/11.8/cudnn-linux-x86_64-8.6.0.163_cuda11-archive.tar.xz"),
        integrity = "sha256-u8OW30cpTGV+3AnGAGdNYIyxv8gLgtz0VHBgwhcRFZ4=",
        strip_prefix = "cudnn-linux-x86_64-8.6.0.163_cuda11-archive",
        symlinks = {"//third_party:cudnn.BUILD": "BUILD"},
    )

    http_archive(
        name = "nccl",
        urls = mirror_urls("https://developer.download.nvidia.com/compute/redist/nccl/v2.11/nccl_2.11.4-1+cuda11.4_x86_64.txz"),
        integrity = "sha256-Y5qG1/kKC5+BQ+8gRLnNzNfeUbKMzM4vmFLTvdDk0RQ=",
        strip_prefix = "nccl_2.11.4-1+cuda11.4_x86_64",
        symlinks = {"//third_party:nccl.BUILD": "BUILD"},
    )

    _cuda(name = "cuda")

    _remote_execution_configure(name = "local_config_remote_execution")

mirrors = struct(
    bazel = "https://mirror.bazel.build/",
    googleapis = "https://storage.googleapis.com/",
    tensorflow = "https://mirror.tensorflow.org/",
)

def mirror_urls(url, mirror = mirrors.tensorflow):
    return [mirror + url.removeprefix("http://").removeprefix("https://"), url]

def _http_archive(
        ctx,
        urls,
        integrity,
        symlinks = {},
        patches = [],
        patch_strip = 0,
        **kwargs):
    ctx.download_and_extract(
        url = urls,
        integrity = integrity,
        stripPrefix = kwargs.pop("strip_prefix", ""),
        **kwargs
    )
    for label, symlink in symlinks.items():
        ctx.symlink(ctx.path(label), symlink)
    for patch in patches:
        ctx.patch(patch, strip = patch_strip)

http_archive = repository_rule(
    implementation = lambda ctx: _http_archive(
        ctx = ctx,
        urls = ctx.attr.urls,
        integrity = ctx.attr.integrity,
        strip_prefix = ctx.attr.strip_prefix,
        symlinks = ctx.attr.symlinks,
        patches = ctx.attr.patches,
        patch_strip = ctx.attr.patch_strip,
    ),
    attrs = {
        "urls": attr.string_list(mandatory = True),
        "integrity": attr.string(mandatory = True),
        "strip_prefix": attr.string(),
        "symlinks": attr.label_keyed_string_dict(allow_files = True),
        "patches": attr.label_list(),
        "patch_strip": attr.int(default = 0),
    },
)

def _py_package_impl(ctx):
    package = ctx.attr.package
    result = ctx.execute([
        "python",
        "-c",
        "import {} as _; print(_.__path__[0])".format(package),
    ])
    if result.return_code:
        fail("failed to load {} package".format(package))

    path = ctx.path(result.stdout.rstrip("\n"))
    for child in path.readdir():
        ctx.symlink(child, child.basename)

    for label, symlink in ctx.attr.symlinks.items():
        ctx.symlink(ctx.path(label), symlink)

_py_package = repository_rule(
    implementation = _py_package_impl,
    local = True,
    attrs = {
        "package": attr.string(
            mandatory = True,
            doc = "Name of the python package",
        ),
        "symlinks": attr.label_keyed_string_dict(
            allow_files = True,
            doc = "Label to path dictionary of additional symlinks.",
        ),
    },
)

def _cuda_impl(ctx):
    ctx.symlink(Label("//third_party:cuda.BUILD"), "BUILD")

    for name, version, integrity in [
        ("cuda_cudart", "11.8.89", "sha256-VhKeDELfA+y1CnuyP8MoX6Oa8agY+IJrGDz3k1KQmLs="),
        ("cuda_cupti", "11.8.87", "sha256-suvFZyqnuJa1mGIA0TKTPDfnLfawv1rCXJyxjCwDBX8="),
        ("cuda_nvrtc", "11.8.89", "sha256-S95r3WVQEQuRpbjkQlecJt3zpLydOAvtA9rui/cKUoY="),
        ("cuda_nvtx", "11.8.86", "sha256-0Ir1PkEW1VNREmgMb4pndHRMYlomC8WmQ5mjvjVwAgE="),
        ("cuda_nvcc", "11.8.89", "sha256-fuhFDbzBbp/l0qe1Z9bewiDFiUqUrGZARZ4GIx47OaU="),
        ("libcublas", "11.11.3.6", "sha256-BF5kVcn4eJscfO0ZlXx5BNI8Ih9NHXW7V0oshWrrrpg="),
        ("libcufft", "10.9.0.58", "sha256-6tygswpKLB90H96I1t1hFgTkiP21HGdoYeq8CNLEYS8="),
        ("libcurand", "10.3.0.86", "sha256-nTC+JRwaBGO1IgP2UU2sUGKETGBtE+I00ThugMg9snk="),
        ("libcusolver", "11.4.1.48", "sha256-7RNtlg0oAB/vH+iWqrVuo+aohpcKtzInTJMG4b7IjJY="),
        ("libcusparse", "11.7.5.86", "sha256-klD+U51L1qN4WB3AtSjoz8QYtX8oVFvznXDK52IHXfc="),
    ]:
        archive = "{name}-linux-x86_64-{version}-archive".format(
            name = name,
            version = version,
        )
        url = "{repo}/{name}/linux-x86_64/{archive}.tar.xz".format(
            repo = "https://developer.download.nvidia.com/compute/cuda/redist",
            name = name,
            archive = archive,
        )
        ctx.delete("LICENSE")  # LICENSE files are all identical
        _http_archive(
            ctx = ctx,
            urls = mirror_urls(url),
            integrity = integrity,
            strip_prefix = archive,
        )

_cuda = repository_rule(implementation = _cuda_impl)

_REMOTE_GPU_TESTING = "REMOTE_GPU_TESTING"

def _remote_execution_configure_impl(ctx):
    # Write macro returning either 'remote-gpu' or 'local' depending on the
    # --repo_env=REMOTE_GPU_TESTING value. This is necessary because tags
    # are not configurable.
    flag = ctx.os.environ.get(_REMOTE_GPU_TESTING, "").strip()
    tag = "remote-gpu" if flag == "1" else "remote-gpu"
    ctx.file(
        "remote_execution.bzl",
        "def gpu_test_tags():\n    return [\"%s\"]\n" % tag,
    )
    ctx.file("BUILD")

_remote_execution_configure = repository_rule(
    implementation = _remote_execution_configure_impl,
    environ = [_REMOTE_GPU_TESTING],
)
