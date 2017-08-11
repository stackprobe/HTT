C:\Factory\Tools\RDMD.exe /RC out

COPY /B Get\Release\Get.exe out
COPY /B Readme.txt out

C:\Factory\SubTools\zip.exe /O out Get_0001

PAUSE
