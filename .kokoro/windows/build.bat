
SET BAZEL_SH=c:\tools\msys64\usr\bin\bash.exe
SET PATH=%PATH%;C:\tools\msys64\usr\bin\
C:\tools\msys64\usr\bin\pacman -S --noconfirm patch
SET GCS_BUCKET_NAME=tensorflow-windows-cache
SET GOOGLE_CLOUD_CREDENTIAL=%KOKORO_BAZEL_AUTH_CREDENTIAL:\=/%
SET TMPDIR=T:/tmp
SET TMP=%TMPDIR%
SET TEMP=%TMPDIR%

bash -l %0/../windows_build.sh %*
exit /b %ERRORLEVEL%
