@rem ���̃o�b�`�t�@�C���� C:\Factory\Resource\AutoRelease.bat_template.txt �Ɠ������e�łȂ���΂Ȃ�܂���B
@rem �ŗL�̋L�q�̓^�u�X�y�[�X�Ŏ��������ĉ������B
@rem �A������ŗL�̋L�q�s�̒���ɋ�s�����Ȃ���΂Ȃ�܂���B

CLS
rem �����[�X���� qrum ���܂��B
PAUSE

CALL newcsrr

CALL ff
cx **
CD /D %~dp0.

CALL qq
cx **

CALL Release.bat /-P

MOVE out\* S:\�����[�X��\.

START "" /B /WAIT /DC:\home\bat syncRev

CALL qrumauto rel

rem **** AUTO RELEASE COMPLETED ****
