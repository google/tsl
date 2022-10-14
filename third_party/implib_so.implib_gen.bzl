"""Rule to generate source files which dynamically load a shared object."""

def _implib_gen_impl(ctx):
    input = ctx.file.shared_library
    dlopen, init, tramp = [
        ctx.actions.declare_file(input.basename + suffix)
        for suffix in [".dlopen.cc", ".init.c", ".tramp.S"]
    ]
    prefix = input.basename.split(".", 1)[0]
    ctx.actions.expand_template(
        template = ctx.file._template,
        substitutions = {
            "header.h": ctx.attr.header,
            "prefix": prefix,
            "ERROR_VALUE": ctx.attr.error_value,
        },
        output = dlopen,
    )
    ctx.actions.run(
        executable = ctx.executable._implib_gen,
        arguments = [
            "--target=x86_64",
            "--dlopen-callback=%sDlOpen" % prefix,
            "--dlsym-callback=%sDlSym" % prefix,
            "--outdir=" + init.dirname,
            "--quiet",
            input.path,
        ],
        inputs = [input],
        outputs = [init, tramp],
    )

    return [DefaultInfo(files = depset([dlopen, init, tramp]))]

implib_gen = rule(
    implementation = _implib_gen_impl,
    attrs = {
        "shared_library": attr.label(
            allow_single_file = True,
            mandatory = True,
            doc = "Shared library to generate dynamic loading code for.",
        ),
        "error_value": attr.string(
            mandatory = True,
            doc = "Error enum to return when dlsym fails.",
        ),
        "header": attr.string(
            mandatory = True,
            doc = "Header which provides the 'error_value'.",
        ),
        "_implib_gen": attr.label(
            executable = True,
            cfg = "exec",
            default = "@implib_so//:implib-gen",
        ),
        "_template": attr.label(
            allow_single_file = True,
            cfg = "exec",
            default = "@implib_so//:dlopen.cc",
        ),
    },
)
