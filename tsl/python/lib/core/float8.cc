/* Copyright 2022 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
// Must be included first
// clang-format off
#include "tsl/python/lib/core/numpy.h" //NOLINT
// clang-format on

#include "tsl/python/lib/core/float8.h"

#include <array>   // NOLINT
#include <cmath>   // NOLINT
#include <limits>  // NOLINT
#include <locale>  // NOLINT

// Place `<locale>` before <Python.h> to avoid a build failure in macOS.
#include <Python.h>

#include "Eigen/Core"  // from @eigen_archive
#include "tsl/platform/types.h"
#include "tsl/python/lib/core/bfloat16.h"
#include "tsl/python/lib/core/bfloat16_typedescriptor.h"
#include "tsl/python/lib/core/custom_float.h"
#include "tsl/python/lib/core/float8_typedescriptor.h"

namespace tsl {
namespace {

// Initializes the module.
bool Initialize() {
  tsl::ImportNumpy();
  import_umath1(false);

  tsl::custom_float_internal::Safe_PyObjectPtr numpy_str =
      tsl::custom_float_internal::make_safe(PyUnicode_FromString("numpy"));
  if (!numpy_str) {
    return false;
  }
  tsl::custom_float_internal::Safe_PyObjectPtr numpy =
      tsl::custom_float_internal::make_safe(PyImport_Import(numpy_str.get()));
  if (!numpy) {
    return false;
  }

  bool float8_already_registered;
  if (!tsl::custom_float_internal::RegisterNumpyDtype<float8_e4m3fn>(
          numpy.get(), &float8_already_registered)) {
    return false;
  }
  if (!tsl::custom_float_internal::RegisterNumpyDtype<float8_e4m3b11>(
          numpy.get())) {
    return false;
  }
  if (!tsl::custom_float_internal::RegisterNumpyDtype<float8_e5m2>(
          numpy.get())) {
    return false;
  }

  // Register casts between float8 types. Only perform the cast if
  // float8_e4m3fn hasn't been previously registered, presumably by a different
  // library. In this case, we assume the cast has also already been registered,
  // and registering it again can cause segfaults due to accessing an
  // uninitialized type descriptor in this library.
  if (!float8_already_registered &&
      !tsl::custom_float_internal::RegisterCustomFloatCast<float8_e4m3fn,
                                                           float8_e5m2>()) {
    return false;
  }

  return true;
}

}  // namespace

bool RegisterNumpyFloat8e4m3fn() {
  if (tsl::custom_float_internal::TypeDescriptor<float8_e4m3fn>::Dtype() !=
      NPY_NOTYPE) {
    // Already initialized.
    return true;
  }
  if (!Initialize()) {
    if (!PyErr_Occurred()) {
      PyErr_SetString(PyExc_RuntimeError, "cannot load float8_e4m3fn module.");
    }
    PyErr_Print();
    return false;
  }
  return true;
}

PyObject* Float8e4m3fnDtype() {
  return reinterpret_cast<PyObject*>(
      tsl::custom_float_internal::TypeDescriptor<float8_e4m3fn>::type_ptr);
}

int Float8e4m3fnNumpyType() {
  return tsl::custom_float_internal::TypeDescriptor<float8_e4m3fn>::Dtype();
}

bool RegisterNumpyFloat8e4m3b11() {
  if (tsl::custom_float_internal::TypeDescriptor<float8_e4m3b11>::Dtype() !=
      NPY_NOTYPE) {
    // Already initialized.
    return true;
  }
  if (!Initialize()) {
    if (!PyErr_Occurred()) {
      PyErr_SetString(PyExc_RuntimeError, "cannot load float8_e4m3b11 module.");
    }
    PyErr_Print();
    return false;
  }
  return true;
}

PyObject* Float8e4m3b11Dtype() {
  return reinterpret_cast<PyObject*>(
      tsl::custom_float_internal::TypeDescriptor<float8_e4m3b11>::type_ptr);
}

int Float8e4m3b11NumpyType() {
  return tsl::custom_float_internal::TypeDescriptor<float8_e4m3b11>::Dtype();
}

bool RegisterNumpyFloat8e5m2() {
  if (tsl::custom_float_internal::TypeDescriptor<float8_e5m2>::Dtype() !=
      NPY_NOTYPE) {
    // Already initialized.
    return true;
  }
  if (!Initialize()) {
    if (!PyErr_Occurred()) {
      PyErr_SetString(PyExc_RuntimeError, "cannot load float8_e5m2 module.");
    }
    PyErr_Print();
    return false;
  }
  return true;
}

PyObject* Float8e5m2Dtype() {
  return reinterpret_cast<PyObject*>(
      tsl::custom_float_internal::TypeDescriptor<float8_e5m2>::type_ptr);
}

int Float8e5m2NumpyType() {
  return tsl::custom_float_internal::TypeDescriptor<float8_e5m2>::Dtype();
}

}  // namespace tsl
