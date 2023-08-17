@echo off

set LOGFILE="4_testSuite.log"
set CWD=%CD%
set TARGETDIR="..\..\target\windows_x64\Release\bin"

rem copy files into working directory
xcopy /Y ..\samples\JSUnit.js %TARGETDIR%
rmdir /S /Q %TARGETDIR%\tests
mkdir %TARGETDIR%\tests
xcopy /Y /E ..\samples\tests %TARGETDIR%\tests

xcopy /Y ..\third-party\extendscript\extendscript\public\libraries\windows_x64\release\ExtendScript.dll %TARGETDIR%
xcopy /Y ..\third-party\extendscript\extendscript\public\libraries\windows_x64\release\ScCore.dll %TARGETDIR%
xcopy /Y ..\third-party\extendscript\extendscript\public\libraries\windows_x64\release\Shell.exe %TARGETDIR%

cd %TARGETDIR%
Shell.exe -u -s -l -B -c "load('JSUnit.js');runTests('tests','%LOGFILE%');"
if errorlevel 1 goto error

goto ok

:error
echo Failed.
xcopy /Y %LOGFILE% "%1"
cd %CWD%
exit 1

:ok
echo Success.
xcopy /Y %LOGFILE% "%1"
cd %CWD%
exit 0
