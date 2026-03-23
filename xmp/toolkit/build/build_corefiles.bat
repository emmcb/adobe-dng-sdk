@echo off
rem parameters:
rem %1: path to VS compiler
rem %2: sub-folder that contains the solution (vsnet8/vc9)
rem %3: directory for log-files

set COMPILER=%1
set SOLUTION="./%2/XMPToolkit.sln"
set LOGFILE="%3/1_build_corefiles_vs.log"
set cov=%4

set DEBUG-CONF="Debug|Win32"
set RELEASE-CONF="Release|Win32"

set DEBUG-CONF-X64="Debug|x64"
set RELEASE-CONF-X64="Release|x64"

REM echo %SOLUTION%
REM echo %LOGFILE%
REM echo %COMPILER%

:buildDebug
echo Building %DEBUG-CONF% ...
echo Building %DEBUG-CONF% ... >> %LOGFILE%
rem %COMPILER% %SOLUTION% /build %DEBUG-CONF% /project %PROJECT% /out %LOGFILE%
%COMPILER% %SOLUTION% /project XMPCore /build %DEBUG-CONF% /out %LOGFILE%
if errorlevel 1 goto error
rem %COMPILER% %SOLUTION% /project "XMPCore Static" /build %DEBUG-CONF% /out %LOGFILE%
rem if errorlevel 1 goto error
%COMPILER% %SOLUTION% /project XMPFiles /build %DEBUG-CONF% /out %LOGFILE%
if errorlevel 1 goto error
rem %COMPILER% %SOLUTION% /project "XMPFiles Static" /build %DEBUG-CONF% /out %LOGFILE%
rem if errorlevel 1 goto error

:buildRelease
rem echo Success.
rem echo Building %RELEASE-CONF% ...
rem echo Building %RELEASE-CONF% ... >> %LOGFILE%
rem if not "%cov%"=="true" goto nocov1
rem activate instrumenting
rem cov01 -1 >> %LOGFILE%
rem set COVFILE=G:\CRUISER_AGENTS\agent_winXP32_1\checkout\main\xmp\toolkit\test\coverage\toolkit.cov
rem :nocov1
rem %COMPILER% %SOLUTION% /build %RELEASE-CONF% /project %PROJECT% /out %LOGFILE%
rem %COMPILER% %SOLUTION% /project XMPCore /build %RELEASE-CONF% /out %LOGFILE%
rem if errorlevel 1 goto error
rem %COMPILER% %SOLUTION% /project "XMPCore Static" /build %RELEASE-CONF% /out %LOGFILE%
rem if errorlevel 1 goto error
rem %COMPILER% %SOLUTION% /project XMPFiles /build %RELEASE-CONF% /out %LOGFILE%
rem if errorlevel 1 goto error
rem %COMPILER% %SOLUTION% /project "XMPFiles Static" /build %RELEASE-CONF% /out %LOGFILE%
rem cov01 -0
rem if errorlevel 1 goto error

:buildDebugx64
rem echo Building %DEBUG-CONF-X64% ...
rem echo Building %DEBUG-CONF-X64% ... >> %LOGFILE%
rem %COMPILER% %SOLUTION% /build %DEBUG-CONF-X64% /project %PROJECT% /out %LOGFILE%
rem %COMPILER% %SOLUTION% /project XMPCore /build %DEBUG-CONF-X64% /out %LOGFILE%
rem if errorlevel 1 goto error
rem %COMPILER% %SOLUTION% /project "XMPCore Static" /build %DEBUG-CONF-X64% /out %LOGFILE%
rem if errorlevel 1 goto error
rem %COMPILER% %SOLUTION% /project XMPFiles /build %DEBUG-CONF-X64% /out %LOGFILE%
rem if errorlevel 1 goto error
rem %COMPILER% %SOLUTION% /project "XMPFiles Static" /build %DEBUG-CONF-X64% /out %LOGFILE%
rem if errorlevel 1 goto error

:buildReleasex64
echo Success.
echo Building %RELEASE-CONF-X64% ...
echo Building %RELEASE-CONF-X64% ... >> %LOGFILE%
rem %COMPILER% %SOLUTION% /build %RELEASE-CONF-X64% /project %PROJECT% /out %LOGFILE%
%COMPILER% %SOLUTION% /project XMPCore /build %RELEASE-CONF-X64% /out %LOGFILE%
if errorlevel 1 goto error
%COMPILER% %SOLUTION% /project "XMPCore Static" /build %RELEASE-CONF-X64% /out %LOGFILE%
if errorlevel 1 goto error
%COMPILER% %SOLUTION% /project XMPFiles /build %RELEASE-CONF-X64% /out %LOGFILE%
if errorlevel 1 goto error
%COMPILER% %SOLUTION% /project "XMPFiles Static" /build %RELEASE-CONF-X64% /out %LOGFILE%
if errorlevel 1 goto error

goto ok

:error
echo Failed.
rem cov01 -0
exit 1

:ok
echo Success.
rem cov01 -0
exit 0
