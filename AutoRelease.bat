CLS
rem �����[�X���� qrum ���܂��B
PAUSE

CALL newcsrr

CALL ff
cx **
CD /D %~dp0.

CALL qq
cx **

CALL _Release.bat /-P

MOVE out\* S:\�����[�X��\.

START "" /B /WAIT /DC:\home\bat syncRev

CALL qrumauto rel

rem **** AUTO RELEASE COMPLETED ****
