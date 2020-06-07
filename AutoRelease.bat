@rem このバッチファイルは C:\Factory\Resource\AutoRelease.bat_template.txt と同じ内容でなければなりません。
@rem 固有の記述はタブスペースで字下げして下さい。
@rem 連続する固有の記述行の直後に空行を入れなければなりません。

CLS
rem リリースして qrum します。
PAUSE

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
