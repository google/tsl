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

#include "tsl/platform/status.h"

#include <stdio.h>

#include <deque>
#include <functional>
#include <memory>
#include <string>

#include "absl/base/call_once.h"
#include "absl/strings/cord.h"
#include "absl/strings/escaping.h"
#include "absl/strings/match.h"
#include "absl/types/optional.h"
#include "tsl/platform/mutex.h"
#include "tsl/platform/stacktrace.h"
#include "tsl/platform/str_util.h"
#include "tsl/platform/strcat.h"
#include "tsl/platform/stringprintf.h"
#include "tsl/protobuf/error_codes.pb.h"

namespace tsl {
namespace error {
// TODO(aminim): figure out the protobuf migration story
using tensorflow::error::ABORTED;
using tensorflow::error::ALREADY_EXISTS;
using tensorflow::error::CANCELLED;
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
}  // namespace error
namespace {

// Log sink is used to collect recent warning and error log messages to be
// attached to the error status.
class StatusLogSink : public TFLogSink {
 public:
  static StatusLogSink* GetInstance() {
    static StatusLogSink* sink = new StatusLogSink();
    return sink;
  }

  void enable() {
    absl::call_once(flag_, [this] {
      num_messages_ = 5;  // default to 5 messages

      if (const char* num_msgs_str =
              getenv("TF_WORKER_NUM_FORWARDED_LOG_MESSAGES")) {
        if (!absl::SimpleAtoi(num_msgs_str, &num_messages_)) {
          LOG(WARNING) << "Failed to parse env variable "
                          "TF_WORKER_NUM_WARNING_ERROR_LOG_IN_STATUS="
                       << num_msgs_str << " as int. Using the default value "
                       << num_messages_ << ".";
        }
      }

      if (num_messages_ > 0) {
        TFAddLogSink(this);
      }
    });
  }

  void GetMessages(std::vector<std::string>* logs) TF_LOCKS_EXCLUDED(mu_) {
    mutex_lock lock(mu_);

    for (auto& msg : messages_) {
      logs->push_back(msg);
    }
  }

  void Send(const TFLogEntry& entry) override TF_LOCKS_EXCLUDED(mu_) {
    if (entry.log_severity() < absl::LogSeverity::kWarning) return;

    mutex_lock lock(mu_);
    messages_.emplace_back(entry.ToString());
    if (messages_.size() > static_cast<size_t>(num_messages_)) {
      messages_.pop_front();
    }
  }

 private:
  mutex mu_;
  // for allowing repeated/concurrent calls to enable()
  absl::once_flag flag_;
  int num_messages_ = 0;
  std::deque<std::string> messages_ TF_GUARDED_BY(mu_);
};

}  // namespace

// TODO(b/197552541) Move this namespace to errors.h after absl migration.
namespace errors {
static constexpr const char kStackTraceProtoUrl[] =
    "type.googleapis.com/tensorflow.StackTracePayload";

void SetStackTrace(::tsl::Status& status, std::vector<StackFrame> stack_trace) {
  status.SetStackTrace(stack_trace);
}

std::vector<StackFrame> GetStackTrace(const ::tsl::Status& status) {
  return status.GetStackTrace();
}

}  // namespace errors

Status::~Status() {}

void Status::SetStackTrace(std::vector<StackFrame> stack_trace) {
  if (state_ != nullptr) {
    state_->stack_trace = stack_trace;
  }
}

std::vector<StackFrame> Status::GetStackTrace() const {
  if (state_ != nullptr) {
    return state_->stack_trace;
  } else {
    return std::vector<StackFrame>();
  }
}

absl::Span<const SourceLocation> Status::GetSourceLocations() const {
  return state_ != nullptr ? state_->source_locations
                           : absl::Span<const SourceLocation>();
}

void Status::MaybeAddSourceLocation(SourceLocation loc) {
  if (state_ == nullptr) {
    return;
  }
  if (loc.line <= 0) {
    return;
  }
  if (loc.file_name == nullptr) {
    return;
  }
  if (loc.file_name[0] == '\0') {
    return;
  }
  state_->source_locations.push_back(loc);
}

Status::Status(tsl::error::Code code, absl::string_view msg,
               SourceLocation loc) {
  assert(code != tsl::error::OK);
  state_ = std::make_unique<State>();
  state_->code = code;
  state_->msg = std::string(msg);
  MaybeAddSourceLocation(loc);
  VLOG(5) << "Generated non-OK status: \"" << *this << "\". "
          << CurrentStackTrace();
}

void Status::Update(const Status& new_status) {
  if (ok()) {
    *this = new_status;
  }
}

void Status::SlowCopyFrom(const State* src) {
  if (src == nullptr) {
    state_ = nullptr;
  } else {
    state_ = std::make_unique<State>(*src);
  }
}

Status::State* Status::NewStateFromNonOKStatus(const Status& s) {
  return new State(*s.state_);
}

const std::string& Status::empty_string() {
  static string* empty = new string;
  return *empty;
}

std::string error_name(error::Code code) {
  switch (code) {
    case tsl::error::OK:
      return "OK";
      break;
    case tsl::error::CANCELLED:
      return "CANCELLED";
      break;
    case tsl::error::UNKNOWN:
      return "UNKNOWN";
      break;
    case tsl::error::INVALID_ARGUMENT:
      return "INVALID_ARGUMENT";
      break;
    case tsl::error::DEADLINE_EXCEEDED:
      return "DEADLINE_EXCEEDED";
      break;
    case tsl::error::NOT_FOUND:
      return "NOT_FOUND";
      break;
    case tsl::error::ALREADY_EXISTS:
      return "ALREADY_EXISTS";
      break;
    case tsl::error::PERMISSION_DENIED:
      return "PERMISSION_DENIED";
      break;
    case tsl::error::UNAUTHENTICATED:
      return "UNAUTHENTICATED";
      break;
    case tsl::error::RESOURCE_EXHAUSTED:
      return "RESOURCE_EXHAUSTED";
      break;
    case tsl::error::FAILED_PRECONDITION:
      return "FAILED_PRECONDITION";
      break;
    case tsl::error::ABORTED:
      return "ABORTED";
      break;
    case tsl::error::OUT_OF_RANGE:
      return "OUT_OF_RANGE";
      break;
    case tsl::error::UNIMPLEMENTED:
      return "UNIMPLEMENTED";
      break;
    case tsl::error::INTERNAL:
      return "INTERNAL";
      break;
    case tsl::error::UNAVAILABLE:
      return "UNAVAILABLE";
      break;
    case tsl::error::DATA_LOSS:
      return "DATA_LOSS";
      break;
    default:
      char tmp[30];
      snprintf(tmp, sizeof(tmp), "UNKNOWN_CODE(%d)", static_cast<int>(code));
      return tmp;
      break;
  }
}

std::string Status::ToString() const {
  if (state_ == nullptr) {
    return "OK";
  } else {
    std::string result(error_name(state_->code));
    result += ": ";
    result += state_->msg;

    for (const std::pair<const std::string, std::string>& element :
         state_->payloads) {
      absl::StrAppend(&result, " [", element.first, "='",
                      absl::CHexEscape(element.second), "']");
    }

    return result;
  }
}

void Status::IgnoreError() const {
  // no-op
}

void Status::SetPayload(absl::string_view type_url, absl::string_view payload) {
  if (ok()) return;
  state_->payloads[std::string(type_url)] = std::string(payload);
}

absl::optional<absl::Cord> Status::GetPayload(
    absl::string_view type_url) const {
  if (ok()) return absl::nullopt;
  auto payload_iter = state_->payloads.find(std::string(type_url));
  if (payload_iter == state_->payloads.end()) return absl::nullopt;
  return absl::Cord(payload_iter->second);
}

bool Status::ErasePayload(absl::string_view type_url) {
  if (ok()) return false;
  auto payload_iter = state_->payloads.find(std::string(type_url));
  if (payload_iter == state_->payloads.end()) return false;
  state_->payloads.erase(payload_iter);
  return true;
}

void Status::ForEachPayload(
    const std::function<void(absl::string_view, absl::string_view)>& visitor)
    const {
  if (ok()) return;
  for (const auto& payload : state_->payloads) {
    visitor(payload.first, payload.second);
  }
}

std::ostream& operator<<(std::ostream& os, const Status& x) {
  os << x.ToString();
  return os;
}

Status OkStatus() { return Status(); }

Status FromAbslStatus(const absl::Status& s) {
  if (s.ok()) {
    return Status();
  }
  Status converted(static_cast<tsl::error::Code>(s.code()), s.message());
  s.ForEachPayload(
      [&converted](absl::string_view key, const absl::Cord& value) {
        converted.SetPayload(key, std::string(value));
      });

  return converted;
}

absl::Status ToAbslStatus(const ::tsl::Status& s) {
  if (s.ok()) {
    return absl::OkStatus();
  }

  absl::Status converted(static_cast<absl::StatusCode>(s.code()),
                         s.error_message());
  s.ForEachPayload([&converted](tsl::StringPiece key, tsl::StringPiece value) {
    converted.SetPayload(key, absl::Cord(value));
  });

  return converted;
}

std::string* TfCheckOpHelperOutOfLine(const ::tsl::Status& v, const char* msg) {
  std::string r("Non-OK-status: ");
  r += msg;
  r += " status: ";
  r += v.ToString();
  // Leaks string but this is only to be used in a fatal error message
  return new std::string(r);
}

StatusGroup::StatusGroup() {}

StatusGroup::StatusGroup(std::initializer_list<Status> statuses) {
  for (const Status& s : statuses) {
    Update(s);
  }
}

static constexpr const char kDerivedStatusProtoUrl[] =
    "type.googleapis.com/tensorflow.DerivedStatus";

Status StatusGroup::MakeDerived(const Status& s) {
  if (IsDerived(s)) {
    return s;
  } else {
    Status derived(s);
    // TODO(b/200167936): Serialize an instance of DerivedStatus proto instead
    // of using the string directly. The string is never used so it is not
    // causing any issues at the moment.
    derived.SetPayload(kDerivedStatusProtoUrl, "");
    return derived;
  }
}

bool StatusGroup::IsDerived(const Status& s) {
  return s.GetPayload(kDerivedStatusProtoUrl).has_value();
}

void StatusGroup::ConfigureLogHistory() {
  StatusLogSink::GetInstance()->enable();
}

void StatusGroup::Update(const Status& s) {
  if (s.ok()) {
    ++num_ok_;
  } else {
    ok_ = false;
    if (IsDerived(s)) {
      derived_.insert(s);
    } else {
      non_derived_.insert(s);
    }
  }
}

static constexpr int kMaxAggregatedStatusMessageSize = 8 * 1024;
static constexpr int kMaxAttachedLogMessageSize = 512;

std::unordered_map<std::string, std::string> StatusGroup::GetPayloads() const {
  std::unordered_map<std::string, std::string> payloads;
  auto capture_payload = [&payloads](absl::string_view key,
                                     absl::string_view value) {
    payloads[std::string(key)] = std::string(value);
  };

  for (const auto& status : derived_) {
    status.ForEachPayload(capture_payload);
  }

  // If a key appears in both derived_ and non_derived_ payloads, then the
  // non_derived_ payload receives priority.
  for (const auto& status : non_derived_) {
    status.ForEachPayload(capture_payload);
  }

  payloads.erase(kDerivedStatusProtoUrl);

  return payloads;
}

Status MakeStatus(
    tsl::error::Code code, absl::string_view message,
    const std::unordered_map<std::string, std::string>& payloads) {
  Status status(code, message);
  for (const auto& payload : payloads) {
    status.SetPayload(payload.first, payload.second);
  }
  return status;
}

std::string MakeString(const Status& status) {
  return absl::StrCat(error_name(status.code()), ": ", status.error_message());
}

// Summarize all the status objects in the StatusGroup. This is used when
// individual Status objects in the StatusGroup are not already summarized.
Status StatusGroup::as_summary_status() const {
  if (ok_) {
    return OkStatus();
  }

  // Gather recent logs as a string
  auto get_recent_logs = [this]() -> std::string {
    if (!recent_logs_.empty()) {
      std::vector<std::string> fmt;
      fmt.push_back("\nRecent warning and error logs:");
      for (auto& log : recent_logs_) {
        // Add an indentation to make it look nicer.
        fmt.push_back("  " + log.substr(0, kMaxAttachedLogMessageSize));
      }
      return absl::StrJoin(fmt, "\n");
    } else {
      return "";
    }
  };

  // If only one root status is found, do not add summary header and footer.
  if (non_derived_.size() == 1) {
    return MakeStatus(non_derived_.begin()->code(),
                      strings::StrCat(non_derived_.begin()->error_message(),
                                      get_recent_logs()),
                      GetPayloads());
  }

  if (!non_derived_.empty()) {
    std::vector<std::string> fmt;

    fmt.push_back(
        strings::Printf("%zu root error(s) found.", non_derived_.size()));

    int index = 0;
    auto code = tsl::error::CANCELLED;
    for (const auto& s : non_derived_) {
      // NOTE: Avoid using CANCELLED as the code of summary status if the group
      // contains other error code.
      if (code == tsl::error::CANCELLED && s.code() != tsl::error::CANCELLED) {
        code = s.code();
      }
      fmt.emplace_back(strings::StrCat("  (", index, ") ", MakeString(s)));
      ++index;
    }

    fmt.push_back(strings::Printf("%zu successful operations.", num_ok_));
    fmt.push_back(
        strings::Printf("%zu derived errors ignored.", derived_.size()));

    std::string error_msg =
        absl::StrJoin(fmt, "\n").substr(0, kMaxAggregatedStatusMessageSize);

    return MakeStatus(code, strings::StrCat(error_msg, get_recent_logs()),
                      GetPayloads());
  } else {
    // All statuses are derived. Pick the first available status to return.
    return MakeDerived(MakeStatus(derived_.begin()->code(),
                                  derived_.begin()->error_message(),
                                  GetPayloads()));
  }
}

// Concatenate all the status objects in the StatusGroup. This is used when
// individual Status objects in the StatusGroup are already summarized Status.
Status StatusGroup::as_concatenated_status() const {
  if (ok_) {
    return OkStatus();
  }

  // If only one root status is found, return it directly.
  if (non_derived_.size() == 1) {
    return MakeStatus(non_derived_.begin()->code(),
                      non_derived_.begin()->error_message(), GetPayloads());
  }

  if (!non_derived_.empty()) {
    std::vector<string> fmt;
    fmt.emplace_back("\n=====================");
    for (const auto& s : non_derived_) {
      fmt.emplace_back(MakeString(s));
    }
    fmt.emplace_back("=====================\n");
    return MakeStatus(
        non_derived_.begin()->code(),
        absl::StrJoin(fmt, "\n").substr(0, kMaxAggregatedStatusMessageSize),
        GetPayloads());
  } else {
    // All statuses are derived. Pick the first available status to return.
    // This should not happen in normal execution.
    return MakeDerived(MakeStatus(derived_.begin()->code(),
                                  derived_.begin()->error_message(),
                                  GetPayloads()));
  }
}

void StatusGroup::AttachLogMessages() {
  recent_logs_.clear();
  StatusLogSink::GetInstance()->GetMessages(&recent_logs_);
}

}  // namespace tsl
