@ECHO OFF
@rem Copy the needed PDFL to the public folder.
@rem Usage: CopyPDFL <mode> <platform>
@rem        <mode> is debug or release
@rem        <platform> is windows or windows_x64. defaults to "windows" (32-bit)

set mode=%1
if "%1" == "" set mode=release
set platform=%2
if "%2" == "" set platform=windows

mkdir ..\..\..\public\%platform%\%mode%\PDF_Handler
xcopy /y /r ..\..\third-party\MiniPDFL\libraries\%platform%\release\MiniPDFL.dll ..\..\..\public\%platform%\%mode%\PDF_Handler
