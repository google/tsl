// This is a source code template for callbacks used by the dynamic loader code.
#include <dlfcn.h>

#include "header.h"  // NOLINT(build/include)

extern "C" {

void* prefixDlOpen(const char* lib_name) {
  // Check if the library has already been loaded.
  if (void* handle = dlopen(lib_name, RTLD_NOLOAD | RTLD_LAZY)) return handle;
  // Try to load the library.
  if (void* handle = dlopen(lib_name, RTLD_LAZY)) return handle;
  // Failed to load library, return handle to self.
  return dlopen("", RTLD_LAZY);
}

void* prefixDlSym(void* handle, const char* sym_name) {
  if (void* address = dlsym(handle, sym_name)) return address;
  return reinterpret_cast<void*>(+[] { return ERROR_VALUE; });
}

}  // extern "C"
