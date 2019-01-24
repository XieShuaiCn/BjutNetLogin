@echo off
echo Setting up environment for Qt usage...
set PATH=D:/QT/Qt5.9.6/5.9.6/mingw54_64/bin;D:/QT/Qt5.9.6/Tools/mingw530_32/bin;%PATH%
cd ../Win32/BjutNetLogin
windelopqt BjutNetLogin.exe
pause