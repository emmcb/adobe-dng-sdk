@echo off
rem aborn 01/17/2006

set COMPILER=%1
set SOLUTION="%2/XMPToolkit.sln"
set LOGFILE="%3/2_vs.log"
set cov=%4

set DEBUG-CONF="Debug|Win32"
set RELEASE-CONF="Release|Win32"

set DEBUG-CONF-X64="Debug|x64"
set RELEASE-CONF-X64="Release|x64"
rem echo %SOLUTION%
rem del %LOGFILE%

:buildDebug
echo Building %DEBUG-CONF% ...
echo Building %DEBUG-CONF% ... >> %LOGFILE%
rem %COMPILER% %SOLUTION% /build %DEBUG-CONF% /project %PROJECT% /out %LOGFILE%
%COMPILER% %SOLUTION% /project XMPScript /build %DEBUG-CONF% /out %LOGFILE%
rem %COMPILER% %SOLUTION% /project "XMPScript Static" /build %DEBUG-CONF% /out %LOGFILE%
if errorlevel 1 goto error

REM :buildRelease
REM echo Success.
REM echo Building %RELEASE-CONF% ...
REM echo Building %RELEASE-CONF% ... >> %LOGFILE%
REM if not "%cov%"=="true" goto nocov1
REM rem activate instrumenting
REM cov01 -1 >> %LOGFILE%
REM set COVFILE=G:\CRUISER_AGENTS\agent_winXP32_1\checkout\main\xmp\toolkit\test\coverage\toolkit.cov
REM :nocov1
REM rem %COMPILER% %SOLUTION% /build %RELEASE-CONF% /project %PROJECT% /out %LOGFILE%
REM %COMPILER% %SOLUTION% /project XMPScript /build %RELEASE-CONF% /out %LOGFILE%
REM %COMPILER% %SOLUTION% /project "XMPScript Static" /build %RELEASE-CONF% /out %LOGFILE%
REM cov01 -0
REM if errorlevel 1 goto error

REM :buildDebugx64
REM echo Building %DEBUG-CONF-X64% ...
REM echo Building %DEBUG-CONF-X64% ... >> %LOGFILE%
REM rem %COMPILER% %SOLUTION% /build %DEBUG-CONF-X64% /project %PROJECT% /out %LOGFILE%
REM %COMPILER% %SOLUTION% /project XMPScript /build %DEBUG-CONF-X64% /out %LOGFILE%
REM %COMPILER% %SOLUTION% /project "XMPScript Static" /build %DEBUG-CONF-X64% /out %LOGFILE%
REM if errorlevel 1 goto error

:buildReleasex64
echo Success.
echo Building %RELEASE-CONF-X64% ...
echo Building %RELEASE-CONF-X64% ... >> %LOGFILE%
rem %COMPILER% %SOLUTION% /build %RELEASE-CONF-X64% /project %PROJECT% /out %LOGFILE%
%COMPILER% %SOLUTION% /project XMPScript /build %RELEASE-CONF-X64% /out %LOGFILE%
%COMPILER% %SOLUTION% /project "XMPScript Static" /build %RELEASE-CONF-X64% /out %LOGFILE%
if errorlevel 1 goto error

goto ok

:error
echo Failed.
exit 1

:ok
echo Success.
exit 0
