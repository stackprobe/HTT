@rem このバッチファイルは C:\Factory\Resource\AutoRelease.bat_template.txt と同じ内容でなければなりません。
@rem 固有の記述はタブスペースで字下げして下さい。
@rem 固有の記述行の直後にある１つ以上連続する空行は無視されます。

CLS
rem リリースして qrum します。
IF NOT "%1" == "/-P" PAUSE

CALL newcsrr

CALL ff
cx **
CD /D %~dp0.

CALL qq
cx **

CALL Release.bat /-P

MOVE out\* S:\リリース物\.

START "" /B /WAIT /DC:\home\bat syncRev

CALL qrumauto rel

rem **** AUTO RELEASE COMPLETED ****
