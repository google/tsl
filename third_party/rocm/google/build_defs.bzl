# Macros for building ROCm code.
def if_rocm(if_true, if_false = []):
    """Shorthand for select()'ing on whether we're building with ROCm.

    Always return if_false in internal version.

    """
    return if_false

def rocm_default_copts():
    """Default options for all ROCm compilations."""
    return []

def rocm_copts(opts = []):
    """Gets the appropriate set of copts for (maybe) ROCm compilation.

      If we're doing ROCm compilation, returns copts for our particular ROCm
      compiler.  If we're not doing ROCm compilation, returns an empty list.

      """
    return []

def rocm_gpu_architectures():
    """Returns a list of supported GPU architectures."""
    return []

def if_rocm_is_configured(x):
    """Tests if the ROCm was enabled during the configure process.

    Unlike if_rocm(), this does not require that we are building with
    --config=rocm. Used to allow non-ROCm code to depend on ROCm libraries.
    """
    return []
