// This is a source code template for callbacks used by the dynamic loader code.
#include <dlfcn.h>

#include <filesystem>

#include "header.h"  // NOLINT(build/include)

extern "C" {

void* prefixDlOpen(const char* lib_name) {
  // Check if the library has already been loaded.
  if (void* handle = dlopen(lib_name, RTLD_NOLOAD | RTLD_LAZY)) return handle;

  // Try to load the library from the bazel runfiles directory.
  if (std::filesystem::path path("rootpath"); !path.empty()) {
    // Use absolute path if runfiles directory is provided by bazel.
    if (const char *test_srcdir = std::getenv("TEST_SRCDIR"),
        *test_workspace = std::getenv("TEST_WORKSPACE");
        test_srcdir && test_workspace) {
      path = std::filesystem::path(test_srcdir) / test_workspace / path;
    }
    if (void* handle = dlopen(path.c_str(), RTLD_LAZY)) return handle;
  }

  // Try to load the library from the Python package path.
  if (std::filesystem::path path("packagepath"); !path.empty()) {
    std::error_code ec;
    auto root = std::filesystem::canonical("/proc/self/exe", ec);
    while (!ec && root.has_relative_path()) {
      root = root.parent_path();
      if (void* handle = dlopen((root / path).c_str(), RTLD_LAZY))
        return handle;
    }
  }

  // Try to load the library using the dlopen search paths.
  if (void* handle = dlopen(lib_name, RTLD_LAZY)) return handle;

  // Failed to load library, return handle to self.
  return dlopen(nullptr, RTLD_LAZY);
}

void* prefixDlSym(void* handle, const char* sym_name) {
  if (void* address = dlsym(handle, sym_name)) return address;
  return reinterpret_cast<void*>(+[] { return ERROR_VALUE; });
}

}  // extern "C"
