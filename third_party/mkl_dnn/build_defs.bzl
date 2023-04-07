def if_mkldnn_openmp(if_true, if_false = []):
    """Returns `if_true` if OpenMP is used with oneDNN.

    Shorthand for select()'ing on whether we're building with
    oneDNN open source library only with openmp

    Returns a select statement which evaluates to if_true if we're building
    with oneDNN open source library only with OpenMP. Otherwise, the
    select statement evaluates to if_false.

    """
    return select({
        "@tsl//third_party/mkl_dnn:build_with_mkldnn_openmp": if_true,
        "//conditions:default": if_false,
    })

def if_onednn_v3(if_true, if_false = []):
    """Returns `if_true` if oneDNN v3.x is used.

    Returns a select statement which evaluates to if_true if we're building
    with oneDNN v3.x open source library only. Otherwise, the select statement
    evaluates to if_false.

    """
    return select({
        "@tsl//third_party/mkl_dnn:build_with_onednn_v3": if_true,
        "//conditions:default": if_false,
    })

def if_mkldnn_aarch64_acl(if_true, if_false = []):
    return select({
        "@tsl//third_party/mkl:build_with_mkl_aarch64": if_true,
        "//conditions:default": if_false,
    })

def if_mkldnn_aarch64_acl_openmp(if_true, if_false = []):
    return select({
        "@tsl//third_party/mkl_dnn:build_with_mkl_aarch64_openmp": if_true,
        "//conditions:default": if_false,
    })
