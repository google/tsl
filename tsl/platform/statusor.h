/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

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

// StatusOr<T> is the union of a Status object and a T object. StatusOr models
// the concept of an object that is either a value, or an error Status
// explaining why such a value is not present. To this end, StatusOr<T> does not
// allow its Status value to be Status::OK.
//
// The primary use-case for StatusOr<T> is as the return value of a
// function which may fail.
//
// Example client usage for a StatusOr<T>, where T is not a pointer:
//
//  StatusOr<float> result = DoBigCalculationThatCouldFail();
//  if (result.ok()) {
//    float answer = result.value();
//    printf("Big calculation yielded: %f", answer);
//  } else {
//    LOG(ERROR) << result.status();
//  }
//
// Example client usage for a StatusOr<T*>:
//
//  StatusOr<Foo*> result = FooFactory::MakeNewFoo(arg);
//  if (result.ok()) {
//    std::unique_ptr<Foo> foo(result.value());
//    foo->DoSomethingCool();
//  } else {
//    LOG(ERROR) << result.status();
//  }
//
// Example client usage for a StatusOr<std::unique_ptr<T>>:
//
//  StatusOr<std::unique_ptr<Foo>> result = FooFactory::MakeNewFoo(arg);
//  if (result.ok()) {
//    std::unique_ptr<Foo> foo = std::move(result.value());
//    foo->DoSomethingCool();
//  } else {
//    LOG(ERROR) << result.status();
//  }
//
// Example factory implementation returning StatusOr<T*>:
//
//  StatusOr<Foo*> FooFactory::MakeNewFoo(int arg) {
//    if (arg <= 0) {
//      return tsl::InvalidArgument("Arg must be positive");
//    } else {
//      return new Foo(arg);
//    }
//  }
//
// Note that the assignment operators require that destroying the currently
// stored value cannot invalidate the argument; in other words, the argument
// cannot be an alias for the current value, or anything owned by the current
// value.
#ifndef TENSORFLOW_TSL_PLATFORM_STATUSOR_H_
#define TENSORFLOW_TSL_PLATFORM_STATUSOR_H_

#include "absl/base/attributes.h"
#include "tsl/platform/errors.h"
#include "tsl/platform/macros.h"
#include "tsl/platform/status.h"
#include "tsl/platform/statusor_internals.h"

namespace tsl {

#if TF_HAS_CPP_ATTRIBUTE(nodiscard)
template <typename T>
class [[nodiscard]] StatusOr;
#endif

template <typename T>
class StatusOr : private internal_statusor::StatusOrData<T>,
                 private internal_statusor::TraitsBase<
                     std::is_copy_constructible<T>::value,
                     std::is_move_constructible<T>::value> {
  template <typename U>
  friend class StatusOr;

  typedef internal_statusor::StatusOrData<T> Base;

 public:
  typedef T element_type;  // DEPRECATED: use `value_type`.
  typedef T value_type;

  // Constructs a new StatusOr with Status::UNKNOWN status.  This is marked
  // 'explicit' to try to catch cases like 'return {};', where people think
  // StatusOr<std::vector<int>> will be initialized with an empty vector,
  // instead of a Status::UNKNOWN status.
  explicit StatusOr();

  // StatusOr<T> will be copy constructible/assignable if T is copy
  // constructible.
  StatusOr(const StatusOr&) = default;
  StatusOr& operator=(const StatusOr&) = default;

  // StatusOr<T> will be move constructible/assignable if T is move
  // constructible.
  StatusOr(StatusOr&&) = default;
  StatusOr& operator=(StatusOr&&) = default;

  // Conversion copy/move constructor, T must be convertible from U.
  template <typename U, typename std::enable_if<
                            std::is_convertible<U, T>::value>::type* = nullptr>
  StatusOr(const StatusOr<U>& other);
  template <typename U, typename std::enable_if<
                            std::is_convertible<U, T>::value>::type* = nullptr>
  StatusOr(StatusOr<U>&& other);

  // Conversion copy/move assignment operator, T must be convertible from U.
  template <typename U, typename std::enable_if<
                            std::is_convertible<U, T>::value>::type* = nullptr>
  StatusOr& operator=(const StatusOr<U>& other);
  template <typename U, typename std::enable_if<
                            std::is_convertible<U, T>::value>::type* = nullptr>
  StatusOr& operator=(StatusOr<U>&& other);

  // Constructs the inner value `T` in-place using the provided args, using the
  // `T(args...)` constructor.
  template <typename... Args>
  explicit StatusOr(absl::in_place_t, Args&&... args);

  // Constructs a new StatusOr with the given value. After calling this
  // constructor, calls to value() will succeed, and calls to status() will
  // return OK.
  //
  // NOTE: Not explicit - we want to use StatusOr<T> as a return type
  // so it is convenient and sensible to be able to do 'return T()'
  // when the return type is StatusOr<T>.
  //
  // REQUIRES: T is copy constructible.
  StatusOr(const T& value);

  // Constructs a new StatusOr with the given non-ok status. After calling
  // this constructor, calls to value() will CHECK-fail.
  //
  // NOTE: Not explicit - we want to use StatusOr<T> as a return
  // value, so it is convenient and sensible to be able to do 'return
  // Status()' when the return type is StatusOr<T>.
  //
  // REQUIRES: !status.ok(). This requirement is DCHECKed.
  // In optimized builds, passing OkStatus() here will have the effect
  // of passing tsl::error::INTERNAL as a fallback.
  StatusOr(const Status& status);
  StatusOr& operator=(const Status& status);

  // TODO(b/62186997): Add operator=(T) overloads.

  // Similar to the `const T&` overload.
  //
  // REQUIRES: T is move constructible.
  StatusOr(T&& value);

  // RValue versions of the operations declared above.
  StatusOr(Status&& status);
  StatusOr& operator=(Status&& status);

  // Returns this->status().ok()
  bool ok() const { return this->status_.ok(); }

  // Returns a reference to our status. If this contains a T, then
  // returns OkStatus().
  const Status& status() const&;
  Status status() &&;

  // Returns a reference to our current value, or CHECK-fails if !this->ok().
  //
  // DEPRECATED: Prefer accessing the value using `operator*` or `operator->`
  // after testing that the StatusOr is OK. If program termination is desired in
  // the case of an error status, consider `CHECK_OK(status_or);`.
  // Note: for value types that are cheap to copy, prefer simple code:
  //
  //   T value = statusor.value();
  //
  // Otherwise, if the value type is expensive to copy, but can be left
  // in the StatusOr, simply assign to a reference:
  //
  //   T& value = statusor.value();  // or `const T&`
  //
  // Otherwise, if the value type supports an efficient move, it can be
  // used as follows:
  //
  //   T value = std::move(statusor).value();
  //
  // The std::move on statusor instead of on the whole expression enables
  // warnings about possible uses of the statusor object after the move.
  // C++ style guide waiver for ref-qualified overloads granted in cl/143176389
  // See go/ref-qualifiers for more details on such overloads.
  const T& value() const&;
  T& value() &;
  const T&& value() const&&;
  T&& value() &&;

  // Deprecated, use `value()` instead.
  ABSL_DEPRECATED("Use `value()` instead.") T& ValueOrDie() &;
  ABSL_DEPRECATED("Use `value()` instead.") T&& ValueOrDie() &&;

  // Returns a reference to the current value.
  //
  // REQUIRES: this->ok() == true, otherwise the behavior is undefined.
  //
  // Use this->ok() or `operator bool()` to verify that there is a current
  // value. Alternatively, see value() for a similar API that guarantees
  // CHECK-failing if there is no current value.
  const T& operator*() const&;
  T& operator*() &;
  const T&& operator*() const&&;
  T&& operator*() &&;

  // Returns a pointer to the current value.
  //
  // REQUIRES: this->ok() == true, otherwise the behavior is undefined.
  //
  // Use this->ok() or `operator bool()` to verify that there is a current
  // value.
  const T* operator->() const;
  T* operator->();

  // Ignores any errors. This method does nothing except potentially suppress
  // complaints from any tools that are checking that errors are not dropped on
  // the floor.
  void IgnoreError() const;
};

////////////////////////////////////////////////////////////////////////////////
// Implementation details for StatusOr<T>

template <typename T>
StatusOr<T>::StatusOr() : Base(Status(tsl::error::UNKNOWN, "")) {}

template <typename T>
StatusOr<T>::StatusOr(const T& value) : Base(value) {}

template <typename T>
StatusOr<T>::StatusOr(const Status& status) : Base(status) {}

template <typename T>
StatusOr<T>& StatusOr<T>::operator=(const Status& status) {
  this->Assign(status);
  return *this;
}

template <typename T>
StatusOr<T>::StatusOr(T&& value) : Base(std::move(value)) {}

template <typename T>
template <typename... Args>
StatusOr<T>::StatusOr(absl::in_place_t, Args&&... args)
    : Base(absl::in_place, std::forward<Args>(args)...) {}

template <typename T>
StatusOr<T>::StatusOr(Status&& status) : Base(std::move(status)) {}

template <typename T>
StatusOr<T>& StatusOr<T>::operator=(Status&& status) {
  this->Assign(std::move(status));
  return *this;
}

template <typename T>
template <typename U,
          typename std::enable_if<std::is_convertible<U, T>::value>::type*>
inline StatusOr<T>::StatusOr(const StatusOr<U>& other)
    : Base(static_cast<const typename StatusOr<U>::Base&>(other)) {}

template <typename T>
template <typename U,
          typename std::enable_if<std::is_convertible<U, T>::value>::type*>
inline StatusOr<T>& StatusOr<T>::operator=(const StatusOr<U>& other) {
  if (other.ok())
    this->Assign(other.value());
  else
    this->Assign(other.status());
  return *this;
}

template <typename T>
template <typename U,
          typename std::enable_if<std::is_convertible<U, T>::value>::type*>
inline StatusOr<T>::StatusOr(StatusOr<U>&& other)
    : Base(static_cast<typename StatusOr<U>::Base&&>(other)) {}

template <typename T>
template <typename U,
          typename std::enable_if<std::is_convertible<U, T>::value>::type*>
inline StatusOr<T>& StatusOr<T>::operator=(StatusOr<U>&& other) {
  if (other.ok()) {
    this->Assign(std::move(other).value());
  } else {
    this->Assign(std::move(other).status());
  }
  return *this;
}

template <typename T>
const Status& StatusOr<T>::status() const& {
  return this->status_;
}
template <typename T>
Status StatusOr<T>::status() && {
  // Note that we copy instead of moving the status here so that
  // ~StatusOrData() can call ok() without invoking UB.
  return ok() ? OkStatus() : this->status_;
}

template <typename T>
const T& StatusOr<T>::value() const& {
  this->EnsureOk();
  return this->data_;
}

template <typename T>
T& StatusOr<T>::value() & {
  this->EnsureOk();
  return this->data_;
}

template <typename T>
const T&& StatusOr<T>::value() const&& {
  this->EnsureOk();
  return std::move(this->data_);
}

template <typename T>
T&& StatusOr<T>::value() && {
  this->EnsureOk();
  return std::move(this->data_);
}


template <typename T>
T& StatusOr<T>::ValueOrDie() & {
  this->EnsureOk();
  return this->data_;
}


template <typename T>
T&& StatusOr<T>::ValueOrDie() && {
  this->EnsureOk();
  return std::move(this->data_);
}

template <typename T>
const T* StatusOr<T>::operator->() const {
  this->EnsureOk();
  return &this->data_;
}

template <typename T>
T* StatusOr<T>::operator->() {
  this->EnsureOk();
  return &this->data_;
}

template <typename T>
const T& StatusOr<T>::operator*() const& {
  this->EnsureOk();
  return this->data_;
}

template <typename T>
T& StatusOr<T>::operator*() & {
  this->EnsureOk();
  return this->data_;
}

template <typename T>
const T&& StatusOr<T>::operator*() const&& {
  this->EnsureOk();
  return std::move(this->data_);
}

template <typename T>
T&& StatusOr<T>::operator*() && {
  this->EnsureOk();
  return std::move(this->data_);
}

template <typename T>
void StatusOr<T>::IgnoreError() const {
  // no-op
}

#define TF_ASSERT_OK_AND_ASSIGN(lhs, rexpr)                             \
  TF_ASSERT_OK_AND_ASSIGN_IMPL(                                         \
      TF_STATUS_MACROS_CONCAT_NAME(_status_or_value, __COUNTER__), lhs, \
      rexpr);

#define TF_ASSERT_OK_AND_ASSIGN_IMPL(statusor, lhs, rexpr)  \
  auto statusor = (rexpr);                                  \
  ASSERT_TRUE(statusor.status().ok()) << statusor.status(); \
  lhs = std::move(statusor).value()

#define TF_STATUS_MACROS_CONCAT_NAME(x, y) TF_STATUS_MACROS_CONCAT_IMPL(x, y)
#define TF_STATUS_MACROS_CONCAT_IMPL(x, y) x##y

#define TF_ASSIGN_OR_RETURN(lhs, rexpr) \
  TF_ASSIGN_OR_RETURN_IMPL(             \
      TF_STATUS_MACROS_CONCAT_NAME(_status_or_value, __COUNTER__), lhs, rexpr)

#define TF_ASSIGN_OR_RETURN_IMPL(statusor, lhs, rexpr) \
  auto statusor = (rexpr);                             \
  if (TF_PREDICT_FALSE(!statusor.ok())) {              \
    return statusor.status();                          \
  }                                                    \
  lhs = std::move(statusor).value()

}  // namespace tsl

#endif  // TENSORFLOW_TSL_PLATFORM_STATUSOR_H_
