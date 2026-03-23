echo  off
cls

REM list of all projects to build

echo "Update path environment that cmake can be found"
REM set PATH=%CD%\..\..\resources\tools\CMakeApp\win\bin;%PATH%
set PATH=%CD%\..\testtools\third-party\cmake\bin;%PATH%


echo "================= Generate project for XMP build ================="
call cmake.bat %1 %2 %3 %4 %5
if errorlevel 1 goto error
popd

goto ok


:error
cd ..
echo Failed.
exit 1

:ok
echo Success.
exit 0
