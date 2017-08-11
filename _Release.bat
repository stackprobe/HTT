C:\Factory\Tools\RDMD.exe /RC out

COPY /B Get\Release\Get.exe out
COPY /B HTT\Release\HTT.exe out
COPY /B WHTT\WHTT\bin\Release\WHTT.exe out

C:\Factory\Tools\xcp.exe doc out
C:\Factory\Tools\xcp.exe /C Sample out\Sample

C:\Factory\SubTools\zip.exe /O out HTT

PAUSE
