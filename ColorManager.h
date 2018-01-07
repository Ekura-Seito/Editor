// ColorManager.h: CColorManager �N���X�̃C���^�[�t�F�C�X
//

#if !defined(COLORMANAGER_H)		//�Q�d�C���N���[�h�h�~
#define COLORMANAGER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CLine;
class CEditorApp;

class CColorManager
{
public:
	CColorManager();							// �R���X�g���N�^
	virtual ~CColorManager();					// �f�X�g���N�^
	void SetFontColor(COLORREF);				// �ʏ�̕����F�ݒ菈��
	BOOL GetFlagComment1Clr();					// �b�E�b�{�{�p�R�����g�L���t���O�擾
	void SetFlagComment1Clr(BOOL);				// �b�E�b�{�{�p�R�����g�L���t���O�ݒ�
	COLORREF GetCommentColor();					// �R�����g�F�擾�����i�b�^�b�{�{�p�j
	void SetCommentColor(COLORREF);				// �R�����g�F�ݒ菈���i�b�^�b�{�{�p�j
	BOOL GetFlagComment2Clr();					// �u�a�p�R�����g�L���t���O�擾
	void SetFlagComment2Clr(BOOL);				// �u�a�p�R�����g�L���t���O�ݒ�
	COLORREF GetCommentColor2();				// �R�����g�F�擾�����i�u�a�p�j
	void SetCommentColor2(COLORREF);			// �R�����g�F�ݒ菈���i�u�a�p�j
	BOOL GetFlagComment3Clr();					// �A�Z���u���p�R�����g�L���t���O�擾
	void SetFlagComment3Clr(BOOL);				// �A�Z���u���p�R�����g�L���t���O�ݒ�
	COLORREF GetCommentColor3();				// �R�����g�F�擾�����i�A�Z���u���p�j
	void SetCommentColor3(COLORREF);			// �R�����g�F�ݒ菈���i�A�Z���u���p�j
	BOOL GetFlagComment4Clr();					// ���p�����p�R�����g�L���t���O�擾
	void SetFlagComment4Clr(BOOL);				// ���p�����p�R�����g�L���t���O�ݒ�
	COLORREF GetCommentColor4();				// �R�����g�F�擾�����i���p�����p�j
	void SetCommentColor4(COLORREF);			// �R�����g�F�ݒ菈���i���p�����p�j
	BOOL GetFlagUrlColor();						// �t�q�k�L���t���O�擾
	void SetFlagUrlColor(BOOL);					// �t�q�k�L���t���O�ݒ�
	COLORREF GetUrlColor();						// �t�q�k�F�擾
	void SetUrlColor(COLORREF);					// �t�q�k�F�ݒ�
	BOOL GetFlagEMailColor();					// ���|���������L���t���O�擾
	void SetFlagEMailColor(BOOL);				// ���|���������L���t���O�ݒ�
	COLORREF GetEMailColor();					// ���|���������F�擾
	void SetEMailColor(COLORREF);				// ���|���������F�ݒ�
	void KeyWordColoring(CLine*);				// �L�[���[�h�J���[�����O����
	void DeleteAllKeyWord();
	COLORREF GetKeyNo1Color();					// �L�[���[�h�m���P�̕����F�擾����	
	void SetKeyNo1Color(COLORREF,const char*);	// �L�[���[�h��`�t�@�C���i�m�n�P�j�ǂݍ��݁��F�ݒ菈��
	COLORREF GetKeyNo2Color();					// �L�[���[�h�m���Q�̕����F�擾����	
	void SetKeyNo2Color(COLORREF,const char*);	// �L�[���[�h��`�t�@�C���i�m�n�Q�j�ǂݍ��݁��F�ݒ菈��
	COLORREF GetKeyNo3Color();					// �L�[���[�h�m���R�̕����F�擾����	
	void SetKeyNo3Color(COLORREF,const char*);	// �L�[���[�h��`�t�@�C���i�m�n�R�j�ǂݍ��݁��F�ݒ菈��
	COLORREF GetKeyNo4Color();					// �L�[���[�h�m���S�̕����F�擾����	
	void SetKeyNo4Color(COLORREF,const char*);	// �L�[���[�h��`�t�@�C���i�m�n�S�j�ǂݍ��݁��F�ݒ菈��

private:
	CEditorApp*		m_pApp;						// �A�v���P�[�V�����N���X�ւ̃|�C���^
	COLORREF		m_FontColor;				// �ʏ�̕����F
	BOOL			m_bComment1Clr;				// �b�E�b�{�{�R�����g�L���t���O
	COLORREF		m_CommentColor;				// �R�����g�F�i�b�^�b�{�{�p�j
	BOOL			m_bComment2Clr;				// �u�a�R�����g�L���t���O
	COLORREF		m_CommentColor2;			// �R�����g�F�i�u�a�p�j
	BOOL			m_bComment3Clr;				// �A�Z���u���R�����g�L���t���O
	COLORREF		m_CommentColor3;			// �R�����g�F�i�A�Z���u���p�j
	BOOL			m_bComment4Clr;				// ���p�����R�����g�L���t���O
	COLORREF		m_CommentColor4;			// �R�����g�F�i���p�����p�j
	BOOL			m_bUrlClr;					// �t�q�k�L���t���O
	COLORREF		m_UrlColor;					// �t�q�k�F
	BOOL			m_bEMailClr;				// ���|���������L���t���O
	COLORREF		m_EMailColor;				// ���|���������F
	COLORREF		m_KeyNo1Color;				// �L�[���[�h�m���P�̕����F
	COLORREF		m_KeyNo2Color;				// �L�[���[�h�m���Q�̕����F
	COLORREF		m_KeyNo3Color;				// �L�[���[�h�m���R�̕����F
	COLORREF		m_KeyNo4Color;				// �L�[���[�h�m���S�̕����F

	CMapStringToPtr	m_objMapStrToColor;			// �L�[���[�h���F�̃}�b�v
};

#endif // !defined(COLORMANAGER_H)

