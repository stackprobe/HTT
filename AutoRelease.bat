CLS
rem �����[�X���� qrum ���܂��B
PAUSE

CALL qq
cx **

CALL _Release.bat /-P

MOVE out\* S:\�����[�X��\.

START "" /B /WAIT /DC:\home\bat syncRev

CALL qrumauto rel

rem **** AUTO RELEASE COMPLETED ****

:END
