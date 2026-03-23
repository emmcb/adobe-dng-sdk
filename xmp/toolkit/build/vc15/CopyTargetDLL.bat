@rem Copy the just build DLL to the target tree.
@rem Usage: CopyTargetDLL <lib> <mode> <platform>
@rem        <lib> is AdobeXMP, AdobeXMPFiles, etc.
@rem        <mode> is debug or release
@rem        <platform> is windows or windows_x64. defaults to "windows" (32-bit)

set library=%1
set mode=%2
set platform=%3
if "%3" == "" set platform=windows

mkdir ..\..\..\target\%platform%\%mode%\bin
xcopy /y /r ..\..\..\public\libraries\%platform%\%mode%\%library%.* ..\..\..\target\%platform%\%mode%\bin\
