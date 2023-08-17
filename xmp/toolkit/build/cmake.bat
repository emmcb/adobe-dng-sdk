echo OFF

rem Change relPath to point to the folder with the shared configs,
set relPath=../..

set workingDir=%~dp0%
set buildSharedLoc=%workingDir%%relPath%/build/shared
call %buildSharedLoc%/CMakeUtils.bat %*
if errorlevel 1 goto error
goto ok

:error
cd ..
echo Failed %PROJECT% build cmake.
exit /B 1

:ok
echo Success %PROJECT% build cmake.
cd ..
exit /B 0