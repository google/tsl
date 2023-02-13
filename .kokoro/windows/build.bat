
SET BAZEL_SH=c:\tools\msys64\usr\bin\bash.exe
SET PATH=%PATH%;C:\tools\msys64\usr\bin\
C:\tools\msys64\usr\bin\pacman -S --noconfirm patch
SET GCS_BUCKET_NAME=tensorflow-windows-cache
SET GOOGLE_CLOUD_CREDENTIAL=%KOKORO_BAZEL_AUTH_CREDENTIAL:\=/%
SET TMPDIR=T:/tmp
SET TMP=%TMPDIR%
SET TEMP=%TMPDIR%


@REM
@REM Set Environment Variables
@REM
IF NOT DEFINED PYTHON_DIRECTORY (
  SET PYTHON_DIRECTORY=Python39
)
SET PY_EXE=C:\%PYTHON_DIRECTORY%\python.exe
SET PATH=%PATH%;C:\%PYTHON_DIRECTORY%

@REM First, upgrade pypi wheels
%PY_EXE% -m pip install --upgrade "setuptools" pip wheel

@REM NOTE: Windows doesn't have any additional requirements from the common ones.
%PY_EXE% -m pip install -r tensorflow/tools/ci_build/release/requirements_common.txt

@REM
@REM Setup Bazelisk
@REM
:: Download Bazelisk from GitHub and make sure its found in PATH.
md C:\tools\bazel\
wget -q https://github.com/bazelbuild/bazelisk/releases/download/v1.11.0/bazelisk-windows-amd64.exe -O C:/tools/bazel/bazel.exe
SET PATH=C:\tools\bazel;%PATH%
bazel version


bash -l %0/../windows_build.sh %*
exit /b %ERRORLEVEL%
