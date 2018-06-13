@echo off
echo Setting up environment for Qt usage...
set PATH=D:\Software\QT\Qt5.9.5\5.9.5\mingw53_32\bin;D:/Software/Qt/Qt5.9.5/Tools/mingw530_32\bin;%PATH%
cd ../Win32/BjutNetLogin
windelopqt BjutNetLogin.exe
pause