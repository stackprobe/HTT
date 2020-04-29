CLS
rem リリースして qrum します。
PAUSE

CALL qq
cx **

CALL _Release.bat /-P

MOVE out\* S:\リリース物\.

START "" /B /WAIT /DC:\home\bat syncRev

CALL qrumauto rel

rem **** AUTO RELEASE COMPLETED ****

:END
