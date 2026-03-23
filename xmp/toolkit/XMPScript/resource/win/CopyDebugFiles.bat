xcopy /y ..\..\third-party\extendscript\sccore\public\libraries\windows\release\ScCore.dll ..\..\..\target\windows\debug\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows\release\ExtendScript.dll ..\..\..\target\windows\debug\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows\release\Shell.exe ..\..\..\target\windows\debug\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows\release\Shell.pdb ..\..\..\target\windows\debug\bin

mkdir ..\..\..\target\windows\debug\bin\scripts

xcopy /yS ..\..\samples ..\..\..\target\windows\debug\bin\scripts


xcopy /y ..\..\third-party\extendscript\sccore\public\libraries\windows_x64\release\ScCore.dll ..\..\..\target\windows_x64\debug\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows_x64\release\ExtendScript.dll ..\..\..\target\windows_x64\debug\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows_x64\release\Shell.exe ..\..\..\target\windows_x64\debug\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows_x64\release\Shell.pdb ..\..\..\target\windows_x64\debug\bin

mkdir ..\..\..\target\windows_x64\debug\bin\scripts

xcopy /yS ..\..\samples ..\..\..\target\windows_x64\debug\bin\scripts

