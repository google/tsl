/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

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

#ifndef TENSORFLOW_TSL_PLATFORM_ERRORS_H_
#define TENSORFLOW_TSL_PLATFORM_ERRORS_H_

#include <sstream>
#include <string>
#include <utility>

#include "absl/base/attributes.h"
#include "absl/strings/cord.h"
#include "absl/strings/str_join.h"
#include "tsl/platform/logging.h"
#include "tsl/platform/macros.h"
#include "tsl/platform/status.h"
#include "tsl/platform/str_util.h"
#include "tsl/platform/strcat.h"

namespace tsl {
namespace error {
// NOLINTBEGIN(misc-unused-using-decls)
// TODO(aminim): figure out the protobuf migration story.
using tensorflow::error::ABORTED;
using tensorflow::error::ALREADY_EXISTS;
using tensorflow::error::CANCELLED;
using tensorflow::error::Code;
using tensorflow::error::DATA_LOSS;
using tensorflow::error::DEADLINE_EXCEEDED;
using tensorflow::error::FAILED_PRECONDITION;
using tensorflow::error::INTERNAL;
using tensorflow::error::INVALID_ARGUMENT;
using tensorflow::error::NOT_FOUND;
using tensorflow::error::OK;
using tensorflow::error::OUT_OF_RANGE;
using tensorflow::error::PERMISSION_DENIED;
using tensorflow::error::RESOURCE_EXHAUSTED;
using tensorflow::error::UNAUTHENTICATED;
using tensorflow::error::UNAVAILABLE;
using tensorflow::error::UNIMPLEMENTED;
using tensorflow::error::UNKNOWN;
// NOLINTEND(misc-unused-using-decls)
}  // namespace error

namespace errors {

namespace internal {

// The DECLARE_ERROR macro below only supports types that can be converted
// into StrCat's AlphaNum. For the other types we rely on a slower path
// through std::stringstream. To add support of a new type, it is enough to
// make sure there is an operator<<() for it:
//
//   std::ostream& operator<<(std::ostream& os, const MyType& foo) {
//     os << foo.ToString();
//     return os;
//   }
// Eventually absl::strings will have native support for this and we will be
// able to completely remove PrepareForStrCat().
template <typename T>
typename std::enable_if<!std::is_convertible<T, strings::AlphaNum>::value,
                        std::string>::type
PrepareForStrCat(const T& t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}
inline const strings::AlphaNum& PrepareForStrCat(const strings::AlphaNum& a) {
  return a;
}

}  // namespace internal

// Maps UNIX errors into a Status.
Status IOError(const string& context, int err_number);

// Returns all payloads from a Status as a key-value map.
inline std::unordered_map<std::string, std::string> GetPayloads(
    const ::tsl::Status& status) {
  std::unordered_map<std::string, std::string> payloads;
  status.ForEachPayload(
      [&payloads](tsl::StringPiece key, tsl::StringPiece value) {
        payloads[std::string(key)] = std::string(value);
      });
  return payloads;
}

// Inserts all given payloads into the given status. Will overwrite existing
// payloads if they exist with the same key.
inline void InsertPayloads(
    ::tsl::Status& status,
    const std::unordered_map<std::string, std::string>& payloads) {
  for (const auto& payload : payloads) {
    status.SetPayload(payload.first, absl::Cord(payload.second));
  }
}

// Copies all payloads from one Status to another. Will overwrite existing
// payloads in the destination if they exist with the same key.
inline void CopyPayloads(const ::tsl::Status& from, ::tsl::Status& to) {
  from.ForEachPayload([&to](tsl::StringPiece key, tsl::StringPiece value) {
    to.SetPayload(key, absl::Cord(value));
  });
}

// Creates a new status with the given code, message and payloads.
inline ::tsl::Status Create(
    Code code, ::tsl::StringPiece message,
    const std::unordered_map<std::string, std::string>& payloads,
    SourceLocation loc = SourceLocation::current()) {
  Status status(code, message, loc);
  InsertPayloads(status, payloads);
  return status;
}

// Returns a new Status, replacing its message with the given.
inline ::tsl::Status CreateWithUpdatedMessage(const ::tsl::Status& status,
                                              ::tsl::StringPiece message) {
  return Create(status.code(), message, GetPayloads(status));
}

// Append some context to an error message.  Each time we append
// context put it on a new line, since it is possible for there
// to be several layers of additional context.
template <typename... Args>
void AppendToMessage(::tsl::Status* status, Args... args) {
  auto new_status = ::tsl::Status(
      status->code(),
      ::tsl::strings::StrCat(status->error_message(), "\n\t", args...));
  CopyPayloads(*status, new_status);
  *status = std::move(new_status);
}

// For propagating errors when calling a function.
#define TF_RETURN_IF_ERROR(...)                          \
  do {                                                   \
    ::tsl::Status _status = (__VA_ARGS__);               \
    if (TF_PREDICT_FALSE(!_status.ok())) return _status; \
  } while (0)

#define TF_RETURN_WITH_CONTEXT_IF_ERROR(expr, ...)           \
  do {                                                       \
    ::tsl::Status _status = (expr);                          \
    if (TF_PREDICT_FALSE(!_status.ok())) {                   \
      ::tsl::errors::AppendToMessage(&_status, __VA_ARGS__); \
      return _status;                                        \
    }                                                        \
  } while (0)

// Convenience functions for generating and using error status.
// Example usage:
//   status.Update(errors::InvalidArgument("The ", foo, " isn't right."));
//   if (errors::IsInvalidArgument(status)) { ... }
//   switch (status.code()) { case error::INVALID_ARGUMENT: ... }

// CANCELLED
template <typename... Args>
::tsl::Status Cancelled(Args... args) {
  return ::tsl::Status(::tsl::error::Code::CANCELLED,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status CancelledWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::CANCELLED, message, payloads);
}

// InvalidArgument
template <typename... Args>
::tsl::Status InvalidArgument(Args... args) {
  return ::tsl::Status(::tsl::error::Code::INVALID_ARGUMENT,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
// Specialized overloads to capture source location for up to three arguments.
template <typename Arg1, typename Arg2, typename Arg3>
::tsl::Status InvalidArgument(Arg1 arg1, Arg2 arg2, Arg3 arg3,
                              SourceLocation loc = SourceLocation::current()) {
  return ::tsl::Status(
      ::tsl::error::Code::INVALID_ARGUMENT,
      ::tsl::strings::StrCat(::tsl::errors::internal::PrepareForStrCat(arg1),
                             ::tsl::errors::internal::PrepareForStrCat(arg2),
                             ::tsl::errors::internal::PrepareForStrCat(arg3)),
      loc);
}
template <typename Arg1, typename Arg2>
::tsl::Status InvalidArgument(Arg1 arg1, Arg2 arg2,
                              SourceLocation loc = SourceLocation::current()) {
  return ::tsl::Status(
      ::tsl::error::Code::INVALID_ARGUMENT,
      ::tsl::strings::StrCat(::tsl::errors::internal::PrepareForStrCat(arg1),
                             ::tsl::errors::internal::PrepareForStrCat(arg2)),
      loc);
}
template <typename Arg1>
::tsl::Status InvalidArgument(Arg1 arg1,
                              SourceLocation loc = SourceLocation::current()) {
  return ::tsl::Status(
      ::tsl::error::Code::INVALID_ARGUMENT,
      ::tsl::strings::StrCat(::tsl::errors::internal::PrepareForStrCat(arg1)),
      loc);
}
template <typename... Args>
::tsl::Status InvalidArgumentWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads,
    SourceLocation loc = SourceLocation::current()) {
  return errors::Create(::tsl::error::Code::INVALID_ARGUMENT, message, payloads,
                        loc);
}

// NotFound
template <typename... Args>
::tsl::Status NotFound(Args... args) {
  return ::tsl::Status(::tsl::error::Code::NOT_FOUND,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
// Specialized overloads to capture source location for up to three arguments.
template <typename Arg1, typename Arg2, typename Arg3>
::tsl::Status NotFound(Arg1 arg1, Arg2 arg2, Arg3 arg3,
                       SourceLocation loc = SourceLocation::current()) {
  return ::tsl::Status(
      ::tsl::error::Code::NOT_FOUND,
      ::tsl::strings::StrCat(::tsl::errors::internal::PrepareForStrCat(arg1),
                             ::tsl::errors::internal::PrepareForStrCat(arg2),
                             ::tsl::errors::internal::PrepareForStrCat(arg3)),
      loc);
}
template <typename Arg1, typename Arg2>
::tsl::Status NotFound(Arg1 arg1, Arg2 arg2,
                       SourceLocation loc = SourceLocation::current()) {
  return ::tsl::Status(
      ::tsl::error::Code::NOT_FOUND,
      ::tsl::strings::StrCat(::tsl::errors::internal::PrepareForStrCat(arg1),
                             ::tsl::errors::internal::PrepareForStrCat(arg2)),
      loc);
}
template <typename Arg1>
::tsl::Status NotFound(Arg1 arg1,
                       SourceLocation loc = SourceLocation::current()) {
  return ::tsl::Status(
      ::tsl::error::Code::NOT_FOUND,
      ::tsl::strings::StrCat(::tsl::errors::internal::PrepareForStrCat(arg1)),
      loc);
}
template <typename... Args>
::tsl::Status NotFoundWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads,
    SourceLocation loc = SourceLocation::current()) {
  return errors::Create(::tsl::error::Code::NOT_FOUND, message, payloads, loc);
}

// AlreadyExists
template <typename... Args>
::tsl::Status AlreadyExists(Args... args) {
  return ::tsl::Status(::tsl::error::Code::ALREADY_EXISTS,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status AlreadyExistsWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::ALREADY_EXISTS, message, payloads);
}

// ResourceExhausted
template <typename... Args>
::tsl::Status ResourceExhausted(Args... args) {
  return ::tsl::Status(::tsl::error::Code::RESOURCE_EXHAUSTED,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status ResourceExhaustedWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::RESOURCE_EXHAUSTED, message,
                        payloads);
}

// Unavailable
template <typename... Args>
::tsl::Status Unavailable(Args... args) {
  return ::tsl::Status(::tsl::error::Code::UNAVAILABLE,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status UnavailableWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::UNAVAILABLE, message, payloads);
}

// FailedPrecondition
template <typename... Args>
::tsl::Status FailedPrecondition(Args... args) {
  return ::tsl::Status(::tsl::error::Code::FAILED_PRECONDITION,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status FailedPreconditionWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::FAILED_PRECONDITION, message,
                        payloads);
}

// OutOfRange
template <typename... Args>
::tsl::Status OutOfRange(Args... args) {
  return ::tsl::Status(::tsl::error::Code::OUT_OF_RANGE,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status OutOfRangeWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::OUT_OF_RANGE, message, payloads);
}

// Unimplemented
template <typename... Args>
::tsl::Status Unimplemented(Args... args) {
  return ::tsl::Status(::tsl::error::Code::UNIMPLEMENTED,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status UnimplementedWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::UNIMPLEMENTED, message, payloads);
}

// Internal
template <typename... Args>
::tsl::Status Internal(Args... args) {
  return ::tsl::Status(::tsl::error::Code::INTERNAL,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status InternalWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::INTERNAL, message, payloads);
}

// Aborted
template <typename... Args>
::tsl::Status Aborted(Args... args) {
  return ::tsl::Status(::tsl::error::Code::ABORTED,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status AbortedWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::ABORTED, message, payloads);
}

// DeadlineExceeded
template <typename... Args>
::tsl::Status DeadlineExceeded(Args... args) {
  return ::tsl::Status(::tsl::error::Code::DEADLINE_EXCEEDED,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status DeadlineExceededWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::DEADLINE_EXCEEDED, message,
                        payloads);
}

// DataLoss
template <typename... Args>
::tsl::Status DataLoss(Args... args) {
  return ::tsl::Status(::tsl::error::Code::DATA_LOSS,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status DataLossWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::DATA_LOSS, message, payloads);
}

// Unknown
template <typename... Args>
::tsl::Status Unknown(Args... args) {
  return ::tsl::Status(::tsl::error::Code::UNKNOWN,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status UnknownPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::UNKNOWN, message, payloads);
}
// PermissionDenied
template <typename... Args>
::tsl::Status PermissionDenied(Args... args) {
  return ::tsl::Status(::tsl::error::Code::PERMISSION_DENIED,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status PermissionDeniedWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::PERMISSION_DENIED, message,
                        payloads);
}

// Unauthenticated
template <typename... Args>
::tsl::Status Unauthenticated(Args... args) {
  return ::tsl::Status(::tsl::error::Code::UNAUTHENTICATED,
                       ::tsl::strings::StrCat(
                           ::tsl::errors::internal::PrepareForStrCat(args)...));
}
template <typename... Args>
::tsl::Status UnauthenticatedWithPayloads(
    const ::tsl::StringPiece& message,
    const std::unordered_map<std::string, std::string>& payloads) {
  return errors::Create(::tsl::error::Code::UNAUTHENTICATED, message, payloads);
}

bool IsAborted(const Status& status);
bool IsAlreadyExists(const Status& status);
bool IsCancelled(const Status& status);
bool IsDataLoss(const Status& status);
bool IsDeadlineExceeded(const Status& status);
bool IsFailedPrecondition(const Status& status);
bool IsInternal(const Status& status);
bool IsInvalidArgument(const Status& status);
bool IsNotFound(const Status& status);
bool IsOutOfRange(const Status& status);
bool IsPermissionDenied(const Status& status);
bool IsResourceExhausted(const Status& status);
bool IsUnauthenticated(const Status& status);
bool IsUnavailable(const Status& status);
bool IsUnimplemented(const Status& status);
bool IsUnknown(const Status& status);

// Produces a formatted string pattern from the name which can uniquely identify
// this node upstream to produce an informative error message. The pattern
// followed is: {{node <name>}}
// Note: The pattern below determines the regex _NODEDEF_NAME_RE in the file
// tensorflow/python/client/session.py
// LINT.IfChange
inline std::string FormatNodeNameForError(const std::string& name) {
  return strings::StrCat("{{node ", name, "}}");
}
// LINT.ThenChange(//tensorflow/python/client/session.py)
template <typename T>
std::string FormatNodeNamesForError(const T& names) {
  return absl::StrJoin(
      names, ", ", [](std::string* output, const std::string& s) {
        ::tsl::strings::StrAppend(output, FormatNodeNameForError(s));
      });
}
// LINT.IfChange
inline std::string FormatColocationNodeForError(const std::string& name) {
  return strings::StrCat("{{colocation_node ", name, "}}");
}
// LINT.ThenChange(//tensorflow/python/framework/error_interpolation.py)
template <typename T>
std::string FormatColocationNodeForError(const T& names) {
  return absl::StrJoin(
      names, ", ", [](std::string* output, const std::string& s) {
        ::tsl::strings::StrAppend(output, FormatColocationNodeForError(s));
      });
}

inline std::string FormatFunctionForError(const std::string& name) {
  return strings::StrCat("{{function_node ", name, "}}");
}

inline Status ReplaceErrorFromNonCommunicationOps(const Status s,
                                                  const std::string& op_name) {
  assert(::tsl::errors::IsUnavailable(s));
  return Status(
      error::Code::INTERNAL,
      strings::StrCat(
          s.error_message(), "\nExecuting non-communication op <", op_name,
          "> originally returned UnavailableError, and was replaced by "
          "InternalError to avoid invoking TF network error handling logic."));
}

template <typename T>
std::string FormatOriginalNodeLocationForError(const T& node_names,
                                               const T& func_names) {
  std::vector<std::string> error_message;
  for (int i = 0; i != node_names.size(); ++i) {
    if (i != 0) {
      error_message.push_back(", ");
    }
    if (i < func_names.size()) {
      error_message.push_back(FormatFunctionForError(func_names[i]));
    }
    error_message.push_back(FormatNodeNameForError(node_names[i]));
  }
  return absl::StrJoin(error_message, "");
}

// The CanonicalCode() for non-errors.
using ::tsl::error::OK;  // NOLINT

}  // namespace errors
}  // namespace tsl

#endif  // TENSORFLOW_TSL_PLATFORM_ERRORS_H_
