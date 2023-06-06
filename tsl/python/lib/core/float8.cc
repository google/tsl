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
#include "tsl/python/lib/core/custom_float.h"

namespace tsl {
namespace custom_float_internal {

template <>
struct TypeDescriptor<float8_e4m3fn>
    : custom_float_internal::CustomFloatTypeDescriptor<float8_e4m3fn> {
  using T = float8_e4m3fn;
  static constexpr const char* kTypeName = "float8_e4m3fn";
  static constexpr const char* kTpDoc = "float8_e4m3fn floating-point values";
  // We must register float8_e4m3fn with a unique kind, because numpy
  // considers two types with the same kind and size to be equal.
  // The downside of this is that NumPy scalar promotion does not work with
  // float8 values.  Using 'V' to mirror bfloat16 vs float16.
  static constexpr char kNpyDescrKind = 'V';
  // TODO(phawkins): there doesn't seem to be a way of guaranteeing a type
  // character is unique.
  static constexpr char kNpyDescrType = '4';
  static constexpr char kNpyDescrByteorder = '=';
};

template <>
struct TypeDescriptor<float8_e4m3b11>
    : custom_float_internal::CustomFloatTypeDescriptor<float8_e4m3b11> {
  using T = float8_e4m3b11;
  static constexpr const char* kTypeName = "float8_e4m3b11";
  static constexpr const char* kTpDoc = "float8_e4m3b11 floating-point values";
  // We must register float8_e4m3b11 with a unique kind, because numpy
  // considers two types with the same kind and size to be equal.
  // The downside of this is that NumPy scalar promotion does not work with
  // float8 values.
  static constexpr char kNpyDescrKind = 'V';
  // TODO(phawkins): there doesn't seem to be a way of guaranteeing a type
  // character is unique.
  static constexpr char kNpyDescrType = 'L';
  static constexpr char kNpyDescrByteorder = '=';
};

template <>
struct TypeDescriptor<float8_e5m2>
    : custom_float_internal::CustomFloatTypeDescriptor<float8_e5m2> {
  using T = float8_e5m2;
  static constexpr const char* kTypeName = "float8_e5m2";
  static constexpr const char* kTpDoc = "float8_e5m2 floating-point values";
  // Treating e5m2 as the natural "float" type since it is IEEE-754 compliant.
  static constexpr char kNpyDescrKind = 'f';
  // TODO(phawkins): there doesn't seem to be a way of guaranteeing a type
  // character is unique.
  static constexpr char kNpyDescrType = '5';
  static constexpr char kNpyDescrByteorder = '=';
};

}  // namespace custom_float_internal

namespace {

// Initializes the module.
bool Initialize() {
  tsl::ImportNumpy();
  import_umath1(false);

  custom_float_internal::Safe_PyObjectPtr numpy_str =
      custom_float_internal::make_safe(PyUnicode_FromString("numpy"));
  if (!numpy_str) {
    return false;
  }
  custom_float_internal::Safe_PyObjectPtr numpy =
      custom_float_internal::make_safe(PyImport_Import(numpy_str.get()));
  if (!numpy) {
    return false;
  }

  if (!custom_float_internal::RegisterNumpyDtype<float8_e4m3fn>(numpy.get())) {
    return false;
  }
  if (!custom_float_internal::RegisterNumpyDtype<float8_e4m3b11>(numpy.get())) {
    return false;
  }
  if (!custom_float_internal::RegisterNumpyDtype<float8_e5m2>(numpy.get())) {
    return false;
  }

  return true;
}

}  // namespace

bool RegisterNumpyFloat8e4m3fn() {
  if (custom_float_internal::TypeDescriptor<float8_e4m3fn>::Dtype() !=
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
      custom_float_internal::TypeDescriptor<float8_e4m3fn>::type_ptr);
}

int Float8e4m3fnNumpyType() {
  return custom_float_internal::TypeDescriptor<float8_e4m3fn>::Dtype();
}

bool RegisterNumpyFloat8e4m3b11() {
  if (custom_float_internal::TypeDescriptor<float8_e4m3b11>::Dtype() !=
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
      custom_float_internal::TypeDescriptor<float8_e4m3b11>::type_ptr);
}

int Float8e4m3b11NumpyType() {
  return custom_float_internal::TypeDescriptor<float8_e4m3b11>::Dtype();
}

bool RegisterNumpyFloat8e5m2() {
  if (custom_float_internal::TypeDescriptor<float8_e5m2>::Dtype() !=
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
      custom_float_internal::TypeDescriptor<float8_e5m2>::type_ptr);
}

int Float8e5m2NumpyType() {
  return custom_float_internal::TypeDescriptor<float8_e5m2>::Dtype();
}

}  // namespace tsl
