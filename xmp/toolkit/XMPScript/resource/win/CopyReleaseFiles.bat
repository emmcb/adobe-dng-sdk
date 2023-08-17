xcopy /y ..\..\third-party\extendscript\sccore\public\libraries\windows\release\ScCore.dll ..\..\..\target\windows\release\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows\release\ExtendScript.dll ..\..\..\target\windows\release\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows\release\Shell.exe ..\..\..\target\windows\release\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows\release\Shell.pdb ..\..\..\target\windows\release\bin

mkdir ..\..\..\target\windows\release\bin\scripts

xcopy /yS ..\..\samples ..\..\..\target\windows\release\bin\scripts



xcopy /y ..\..\third-party\extendscript\sccore\public\libraries\windows_x64\release\ScCore.dll ..\..\..\target\windows_x64\release\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows_x64\release\ExtendScript.dll ..\..\..\target\windows_x64\release\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows_x64\release\Shell.exe ..\..\..\target\windows_x64\release\bin

xcopy /y ..\..\third-party\extendscript\extendscript\public\libraries\windows_x64\release\Shell.pdb ..\..\..\target\windows_x64\release\bin

mkdir ..\..\..\target\windows_x64\release\bin\scripts

xcopy /yS ..\..\samples ..\..\..\target\windows_x64\release\bin\scripts


