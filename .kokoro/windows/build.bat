
SET TMPDIR=T:/tmp
SET TMP=%TMPDIR%
SET TEMP=%TMPDIR%

bash -l %0/../windows_build.sh %*
exit /b %ERRORLEVEL%
