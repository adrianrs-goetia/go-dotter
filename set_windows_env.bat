@echo off
set INCLUDE=C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.38.33130/include;C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/ucrt
set LIB=C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/;C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.38.33130/lib/x64;C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/ucrt/x64
%*

@REM                    EXAMPLE VSCODE BUILD TASK
@REM {
@REM     "version": "2.0.0",
@REM     "tasks": [
@REM         {
@REM             "label": "echo",
@REM             "type": "shell",
@REM             "command": "${workspaceFolder}/set_windows_env.bat",
@REM             "args": [
@REM                 "cmake",
@REM                 "--build", "${workspaceFolder}/build",
@REM                 "--config", "Debug",
@REM                 "--target", "all"
@REM             ],
@REM             "group": {
@REM                 "kind": "build",
@REM                 "isDefault": true
@REM             },
@REM             "dependsOn": "myenv",
@REM         },
@REM     ]
@REM }