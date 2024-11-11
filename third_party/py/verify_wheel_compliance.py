# Copyright 2024 The Tensorflow Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""Tool to verify wheel manylinux compliance."""

import argparse
import io
import re
import sys
from auditwheel import main_show


def parse_args() -> argparse.Namespace:
  """Arguments parser."""
  parser = argparse.ArgumentParser(
      description="Helper for auditwheel", fromfile_prefix_chars="@"
  )
  parser.add_argument(
      "--wheel_path", required=True, help="Path of the wheel, mandatory"
  )
  parser.add_argument(
      "--compliance-tag", help="ManyLinux compliance tag", required=False
  )
  parser.add_argument(
      "--compliance-verification-log-path",
      help="Path to file with compliance verification results",
      required=False,
  )
  return parser.parse_args()


def get_auditwheel_output(wheel_path: str) -> None:
  """Run "auditwheel show" on the wheel and return the output.

  Args:
    wheel_path: path of the wheel file

  Returns:
    "auditwheel show" output
  """
  stringio = io.StringIO()
  previous_stdout = sys.stdout
  sys.stdout = stringio

  auditwheel_parser = argparse.ArgumentParser(
      description="Cross-distro Python wheels."
  )
  sub_parsers = auditwheel_parser.add_subparsers(metavar="command", dest="cmd")
  main_show.configure_parser(sub_parsers)
  auditwheel_args = argparse.Namespace(
      WHEEL_FILE=wheel_path,
      verbose=1,
  )
  main_show.execute(args=auditwheel_args, p=auditwheel_parser)

  sys.stdout = previous_stdout
  return stringio.getvalue()


def verify_wheel_compliance(
    auditwheel_log: str,
    compliance_tag: str,
    verification_log_path: str,
) -> None:
  """Verify wheel compliance.

  Args:
    auditwheel_log: "auditwheel show" execution results
    compliance_tag: manyLinux compliance tag
    verification_log_path: path to file with compliance verification results

  Raises:
    RuntimeError: if the wheel is not manyLinux compliant.
  """
  regex = 'following platform tag: "{}"'.format(compliance_tag)
  if re.search(regex, auditwheel_log):
    with open(verification_log_path, "w") as verification_log:
      verification_log.write(
          "The wheel is {tag} compliant:\n{log}".format(
              tag=compliance_tag, log=auditwheel_log
          )
      )
  else:
    raise RuntimeError(
        (
            "The wheel is not compliant with tag {tag}."
            + " If you want to disable this check, please provide"
            + " `--//tensorflow/tools/pip_package:wheel_compliance=false`."
            + "\n{result}"
        ).format(tag=compliance_tag, result=auditwheel_log)
    )


if __name__ == "__main__":
  args = parse_args()
  auditwheel_output = get_auditwheel_output(args.wheel_path)
  verify_wheel_compliance(
      auditwheel_output,
      args.compliance_tag,
      args.compliance_verification_log_path,
  )
