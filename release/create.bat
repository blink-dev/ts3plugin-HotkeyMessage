@echo off
copy ..\build64\Release\HotkeyMessage.dll plugins\HotkeyMessage_win64.dll
mkdir releases\
"C:\Program Files\7-Zip\7z.exe" a releases\HotkeyMessage.ts3_plugin -tzip -mx=9 -mm=Deflate ./package.ini plugins/*
pause