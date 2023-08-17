echo OFF
set ARCH=%1

IF "%2"=="Clean" GOTO CleanCMake
IF "%3"=="Clean" GOTO CleanCMake
IF "%4"=="Clean" GOTO CleanCMake
goto MakeProjects

:CleanCmake
echo "Delete CMake folder PDF_Plugin Build"
if exist CMake rmdir /S /Q CMake
if exist CMake64 rmdir /S /Q CMake64
exit /B 0

:MakeProjects
IF "%2"=="2008" GOTO gen2008

:staticOrDynamic
set CMAKE_BUILDSTATIC=Off

:gen2010
set GENERATOR=Visual Studio 10
goto doneWithGenerator

:gen2008
set GENERATOR=Visual Studio 9 2008
goto doneWithGenerator

:doneWithGenerator
IF "%ARCH%"=="32" GOTO 32
IF "%ARCH%"=="64" GOTO 64
if "%ARCH%" == "" GOTO 32

echo "CMake Generate files for %PROJECT%"

:32

mkdir CMake
cd CMake
cmake ../. -G"%GENERATOR%" -DXMP_CMAKEFOLDER_NAME="CMake" -DXMP_BUILD_STATIC="%CMAKE_BUILDSTATIC%"
if errorlevel 1 goto error
goto ok

:64
mkdir CMake64
cd CMake64
cmake ../. -G"%GENERATOR% Win64" -DCMAKE_CL_64=ON -DXMP_CMAKEFOLDER_NAME="CMake" -DXMP_BUILD_STATIC="%CMAKE_BUILDSTATIC%"
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