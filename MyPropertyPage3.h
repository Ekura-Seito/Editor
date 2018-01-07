// MyPropertyPage3.h : CMyPropertyPage3 �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MYPROPERTYPAGE3_H)		//�Q�d�C���N���[�h�h�~
#define MYPROPERTYPAGE3_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CColorButton;

class CMyPropertyPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage3)

public:
	CMyPropertyPage3();					//�R���X�g���N�^
	~CMyPropertyPage3();				//�f�X�g���N�^

	BOOL IsChanged();					// �ύX��������
	void ClearChangeFlag();				// 

	CString		m_objFileExt;			// �t�@�C���g���q
	COLORREF	m_FontColor;			// �ʏ�̕����F
	COLORREF	m_BackColor;			// �ʏ�̔w�i�F
	COLORREF	m_InvertFontColor;		// ���]���̕����F
	COLORREF	m_InvertBackColor;		// ���]���̔w�i�F
	COLORREF	m_KeyNo1Color;			// �L�[���[�h�m���P�̕����F
	COLORREF	m_KeyNo2Color;			// �L�[���[�h�m���Q�̕����F
	COLORREF	m_KeyNo3Color;			// �L�[���[�h�m���R�̕����F
	COLORREF	m_KeyNo4Color;			// �L�[���[�h�m���S�̕����F
	COLORREF	m_MarkLineColor;		// �}�[�N�s�F
	COLORREF	m_CommentColor;			// �R�����g�̕����F�i�b�^�b�{�{�p�j
	COLORREF	m_CommentColor2;		// �R�����g�̕����F�i�u�a�p�j
	COLORREF	m_CommentColor3;		// �R�����g�̕����F�i�A�Z���u���p�j
	COLORREF	m_CommentColor4;		// �R�����g�̕����F�i���p�����p�j
	COLORREF	m_LineNoColor;			// �s�ԍ��̐F
	COLORREF	m_LineNoBackColor;		// �s�ԍ��̔w�i�̐F
	COLORREF	m_RulerColor;			// ���[���̐F
	COLORREF	m_RulerBackColor;		// ���[���̔w�i�̐F
	COLORREF	m_CRLFColor;			// ���s�}�[�N�̐F
	COLORREF	m_TabColor;				// �^�u�}�[�N�̐F
	COLORREF	m_ZenkakuSpaceColor;	// �S�p�X�y�[�X�̐F
	COLORREF	m_UnderLineColor;		// �A���_�[���C���̐F
	COLORREF	m_UrlColor;				// �t�q�k�F
	COLORREF	m_EMailColor;			// ���|���������F
	COLORREF	m_MDIBkgndColor;		// �l�c�h�w�i�F
	COLORREF	m_DiffDelLineColor;		// �t�@�C����r�p�A�폜�s�F
	COLORREF	m_DiffAddLineColor;		// �t�@�C����r�p�A�ǉ��s�F
	COLORREF	m_DirDiffLineColor;		// �f�B���N�g����r�p�A����t�@�C���F

	//{{AFX_DATA(CMyPropertyPage3)
	enum { IDD = IDD_PROPPAGE3 };
	CComboBox	m_objFileExtCombo;
	CColorButton	m_objTxtClrBtn;
	CColorButton	m_objInvTxtClrBtn;
	CColorButton	m_objInvBkClrBtn;
	CColorButton	m_objBkClrBtn;
	CColorButton	m_objKeyNo1ClrBtn;
	CColorButton	m_objKeyNo2ClrBtn;
	CColorButton	m_objKeyNo3ClrBtn;
	CColorButton	m_objKeyNo4ClrBtn;
	CColorButton	m_objMarkLineClrBtn;
	BOOL			m_bComment1Clr;
	CColorButton	m_objCommentClrBtn;
	BOOL			m_bComment2Clr;
	CColorButton	m_objCommentClrBtn2;
	BOOL			m_bComment3Clr;
	CColorButton	m_objCommentClrBtn3;
	BOOL			m_bComment4Clr;
	CColorButton	m_objCommentClrBtn4;
	BOOL			m_bDispLineNo;
	CColorButton	m_objLineNoClrBtn;
	CColorButton	m_objLineNoBackClrBtn;
	BOOL			m_bDispRuler;
	CColorButton	m_objRulerClrBtn;
	CColorButton	m_objRulerBackClrBtn;
	BOOL			m_bDispCRLF;
	CColorButton	m_objCRLFClrBtn;
	BOOL			m_bDispTab;
	CColorButton	m_objTabClrBtn;
	BOOL			m_bDispZenkakuSpace;
	CColorButton	m_objZenkakuSpaceClrBtn;
	BOOL			m_bDispUnderLine;
	CColorButton	m_objUnderLineClrBtn;
	BOOL			m_bDispUrl;
	CColorButton	m_objUrlClrBtn;
	BOOL			m_bDispEMail;
	CColorButton	m_objEMailClrBtn;
	CColorButton	m_objMDIBkgndClrBtn;
	CColorButton	m_objDiffDelLineClrBtn;
	CColorButton	m_objDiffAddLineClrBtn;
	CColorButton	m_objDirDiffLineClrBtn;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage3)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTxtclr();
	afx_msg void OnButtonBkclr();
	afx_msg void OnButtonInvbkclr();
	afx_msg void OnButtonInvtxtclr();
	afx_msg void OnButtonKeyword1clr();
	afx_msg void OnButtonKeyword2clr();
	afx_msg void OnButtonKeyword3clr();
	afx_msg void OnButtonKeyword4clr();
	afx_msg void OnButtonMarklineclr();
	afx_msg void OnChkcppcomment();
	afx_msg void OnButtonCommentclr();
	afx_msg void OnChkvbcomment();
	afx_msg void OnButtonCommentclr2();
	afx_msg void OnChkasmcomment();
	afx_msg void OnButtonCommentclr3();
	afx_msg void OnChkDisplineno();
	afx_msg void OnButtonlinenoclr();
	afx_msg void OnButtonlinenobackclr();
	afx_msg void OnChkDispruler();
	afx_msg void OnButtonrulerclr();
	afx_msg void OnButtonrulerbackclr();
	afx_msg void OnChkDispcrlf();
	afx_msg void OnButtonCrlfclr();
	afx_msg void OnChkDisptab();
	afx_msg void OnButtonTabclr();
	afx_msg void OnChkDispzenkakuspace();
	afx_msg void OnButtonZenkakuSpaceclr();
	afx_msg void OnChkDispunderline();
	afx_msg void OnButtonUnderLineclr();
	afx_msg void OnChkDispurl();
	afx_msg void OnButtonUrlclr();
	afx_msg void OnChkDispemail();
	afx_msg void OnButtonEmailclr();
	afx_msg void OnButtonMDIBkgndclr();
	afx_msg void OnButtonDiffDelLineclr();
	afx_msg void OnButtonDiffAddLineclr();
	afx_msg void OnButtonDirDifflineclr();
	afx_msg void OnSelchangeComboFileext();
	afx_msg void OnCheckQuote();
	afx_msg void OnButtonCommentclr4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;									//�A�v���P�[�V�����N���X�ւ̃|�C���^
	BOOL		m_bIsChanged;							//�ύX�����t���O
};

#endif // !defined(MYPROPERTYPAGE3_H)

