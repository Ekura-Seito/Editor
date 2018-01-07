// MainFrm.h : CMainFrame �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(MAINFRM_H)		//�Q�d�C���N���[�h�h�~
#define MAINFRM_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ToolBarEx.h"
#include "BCMenu.h"

class CEditorApp;
class CEditorDoc;
class CGrepStatusDlg;
class CCodeConvStatusDlg;
class CKaigyoConvStatusDlg;
class CFileSearchStatusDlg;
class CWCntStatusDlg;
class CBitmapClient;
class COperation;
class COperationManager;
class CEncriptStatusDlg;
class CDecriptStatusDlg;
class CFileCompareStatusDlg;
class CDirCompareStatusDlg;
class CFileTouchDlg;
class CMailListStatusDlg;
class CHtmlOpenStatusDlg;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();										// �R���X�g���N�^
	BOOL SetStatusBarDispInfo(LPSTATUSBARDISP);			// �X�e�[�^�X�o�[�\�����ڏ��ݒ�
	BOOL IsDispIndicatorByte();							// �L�����b�g�ʒu�o�C�g��\�����邩�̃`�F�b�N����
	BOOL IsDispIndicatorGyoretu();						// �L�����b�g�ʒu�s�A���\�����邩�̃`�F�b�N����
	BOOL IsDispIndicatorCode();							// �L�����b�g�ʒu�R�[�h��\�����邩�̃`�F�b�N����
	void SetSaveWindowPlace(BOOL);						// �E�C���h�E�ʒu�ۑ��t���O�ݒ�
	void SetStatusMessage(char*);						// �X�e�[�^�X�y�C�����b�Z�[�W�ݒ�
	void SetStatusMessage(CString&);					// �X�e�[�^�X�y�C�����b�Z�[�W�ݒ�
	void SetGyoRetu(long,long,long);					// �s�A��ԍ��̃X�e�[�^�X�y�C���ݒ�
	void ClearGyoRetu();								// �s�A��ԍ��̃X�e�[�^�X�y�C���N���A
	void SetInsMode(BOOL);								// �}���A�㏑�����[�h�X�e�[�^�X�y�C���ݒ�
	void ClearInsMode();								// �}���A�㏑�����[�h�X�e�[�^�X�y�C���N���A
	void SetReadOnlyMode(BOOL);							// �����݋֎~���[�h�X�e�[�^�X�y�C���ݒ�
	void ClearReadOnlyMode();							// �����݋֎~���[�h�X�e�[�^�X�y�C���N���A
	void SetDateTime();									// ���t�A�����X�e�[�^�X�y�C���ݒ�
	void ClearDateTime();								// ���t�A�����X�e�[�^�X�y�C���N���A
	void SetCaretPosCharCode(UCHAR,UCHAR);				// �L�����b�g�ʒu�R�[�h�X�e�[�^�X�y�C���ݒ�
	void ClearCaretPosCharCode();						// �L�����b�g�ʒu�R�[�h�X�e�[�^�X�y�C���N���A
	void SetCaretPosByte(long,long);					// �L�����b�g�ʒu�o�C�g�X�e�[�^�X�y�C���ݒ�
	void ClearCaretPosByte();							// �L�����b�g�ʒu�o�C�g�X�e�[�^�X�y�C���N���A
	void SetStatusBarFont(LOGFONT);						// �X�e�[�^�X�o�[�t�H���g�ݒ�
	void SetFlagAutoSave(BOOL);							// �I�[�g�Z�[�u�t���O�ݒ�
	void SetSaveInterval(long);							// �I�[�g�Z�[�u�Ԋu�ݒ�
	void SetMDIBkgndColor(COLORREF);					// �l�c�h�w�i�F�ݒ�
	void SetFlagDispLogoBmp(BOOL);						// ���S�r�b�g�}�b�v�\���t���O�ݒ�
	void SetLogoBmpKind(int);							// ���S�r�b�g�}�b�v��ʐݒ�
	void SetLogoPosKind(int);							// ���S�\���ʒu��ʐݒ�
	void SetLogoBmpFilePath(CString);					// ���S�r�b�g�}�b�v�t�@�C���p�X�ݒ菈��
	void SetLogoPosX(UINT);								// ���S�\���ʒu�w�ݒ�
	void SetLogoPosY(UINT);								// ���S�\���ʒu�x�ݒ�
	BOOL IsNowOperationRecording();						// �L�[����L�^������
	BOOL IsNowStepDoOperation();						// �L�[����X�e�b�v���s������
	void addOperation(COperation*);						// �L�[����L�^
	long GetStatusDayStyle();							// �X�e�[�^�X�o�[�֕\��������t�\���X�^�C���擾
	void SetStatusDayStyle(long);						// �X�e�[�^�X�o�[�֕\��������t�\���X�^�C���ݒ�
	long GetStatusHourStyle();							// �X�e�[�^�X�o�[�֕\�����鎞���\���X�^�C���擾
	void SetStatusHourStyle(long);						// �X�e�[�^�X�o�[�֕\�����鎞���\���X�^�C���ݒ�
	void SetStatusLineStyle(long);						// �X�e�[�^�X�o�[�֕\������s�A��\���X�^�C���ݒ�
	BOOL IsNowIncrimentalSearching();					// �C���N�������^���T�[�`���s������
	void SetFlagIncrimentalSearching(BOOL);				// �C���N�������^���T�[�`���s���t���O�ݒ�
	void DoFileCompare(CString&,CString&,CEditorDoc*);	// �t�@�C����r���s����
	void SetFindParameter(CString&,BOOL,BOOL,BOOL,BOOL);// �����p�����[�^�ݒ�
	void SetReplaceParameter(CString&,CString&,BOOL,BOOL,BOOL);// �u���p�����[�^�ݒ�
	BOOL IsHaveChildWnd();
	void SetHtmlOpenParameter(BOOL,CString&,BOOL,CString&,CString&,CString&,BOOL);

	//�r�b�g�}�b�v�t�����j���[�p
	HMENU NewMenu();
	HMENU NewDefaultMenu();
	BCMenu m_menu,m_default;

	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CEditorApp*				m_pApp;							// �A�v���P�[�V�����N���X�ւ̃|�C���^

	// �E�C���h�E�ʒu�֌W
	BOOL					m_bIsSaveWindowPlace;			// �E�C���h�E�ʒu��ۑ�����

	// WEB���HTML�t�@�C�����J���֘A
	BOOL					m_bIsUseProxyHtml;				// �v���L�V�T�[�o�o�R���H
	CString					m_objRequestPathHtml;			// �v���p�X
	CString					m_objProxyIPAddressHtml;		// �v���L�V�T�[�o�̂h�o�A�h���X
	BOOL					m_bIsDialupHtml;				// �_�C�A���A�b�v�ڑ����邩�H
	CString					m_objDialupEntryHtml;			// �g�p����_�C�A���A�b�v�G���g��
	CString					m_objRasUserNameHtml;			// ���[�U��
	CString					m_objRasPasswdHtml;				// �p�X���[�h
	BOOL					m_bIsAutoDisconnectHtml;		// �����ؒf���邩�H
	CHtmlOpenStatusDlg*		m_pobjHtmlOpenStatusDlg;		// �󋵃_�C�A���O

	// �����֘A
	CString					m_objFindString;				// ����������
	BOOL					m_bIsMatchCaseFind;				// �啶���A��������ʂ̗L��
	BOOL					m_bIsMatchWholeWordFind;		// �P��S�̂���v���鎖���������H
	BOOL					m_bIsSearchDownFind;			// ��������
	BOOL					m_bIsNowIncrimentalSearching;	// �C���N�������^���T�[�`���s���t���O
	BOOL					m_bIsUseRegularExpressionFind;	// ���K�\�����g�p���邩�H

	// �u���֘A
	CString					m_objReplaceString;				// �u��������
	BOOL					m_bIsMatchCaseReplace;			// �啶���A��������ʂ̗L��
	BOOL					m_bIsMatchWholeWordReplace;		// �P��S�̂���v���鎖���������H
	BOOL					m_bIsUseRegularExpressionReplace;// ���K�\�����g�p���邩�H

	// �f�q�d�o�֘A
	BOOL					m_bIsMatchCaseGrep;				// �啶���A��������ʂ̗L��
	BOOL					m_bIsMatchWholeWordGrep;		// �P��S�̂���v���鎖���������H
	BOOL					m_bIsSearchSubFolderGrep;		// �T�u�t�H���_���������邩�H
	BOOL					m_bIsUseRegularExpressionGrep;	// ���K�\�����g�p���邩�H
	CString					m_objFolderGrep;				// �f�q�d�o���s�t�H���_
	CString					m_objGrepStr;					// ����������
	CString					m_objFileKindGrep;				// �����Ώۂ̃t�@�C�����
	CEditorDoc*				m_pobjGrepDoc;					// �f�q�d�o�p�h�L�������g�N���X
	CFrameWnd*				m_pobjGrepFrameWnd;				// �f�q�d�o�p�q�E�C���h�E�N���X
	CGrepStatusDlg*			m_pobjGrepStatusDlg;			// �f�q�d�o�󋵃_�C�A���O

	// �����R�[�h�ϊ��֘A
	BOOL					m_bIsSearchSubFolderConv;		// �T�u�t�H���_���ϊ����邩�H
	BOOL					m_bIsOutOtherFileConv;			// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	int						m_nConvKind;					// �����R�[�h�ϊ����
	CString					m_objFolderConv;				// �����R�[�h�ϊ����s�t�H���_
	CString					m_objFileKindConv;				// �ϊ��Ώۂ̃t�@�C�����
	int						m_nInCodeConv;					// ���̓t�@�C���̕����R�[�h������
	CEditorDoc*				m_pobjConvDoc;					// �����R�[�h�ϊ��p�h�L�������g�N���X
	CFrameWnd*				m_pobjConvFrameWnd;				// �����R�[�h�ϊ��p�E�C���h�E�N���X
	CCodeConvStatusDlg*		m_pobjConvStatusDlg;			// �����R�[�h�ϊ��󋵃_�C�A���O

	// ���s�R�[�h�ϊ��֘A
	BOOL					m_bIsSearchSubFolderKaigyoConv;	// �T�u�t�H���_���ϊ����邩�H
	BOOL					m_bIsOutOtherFileKaigyoConv;	// �ϊ����ʂ�ʃt�@�C���֏o�͂��邩�H
	CString					m_objKaigyoConvKind;			// ���s�R�[�h�ϊ����
	CString					m_objFolderKaigyoConv;			// ���s�R�[�h�ϊ����s�t�H���_
	CString					m_objFileKindKaigyoConv;		// �ϊ��Ώۂ̃t�@�C�����
	CEditorDoc*				m_pobjKaigyoConvDoc;			// ���s�R�[�h�ϊ��p�h�L�������g�N���X
	CFrameWnd*				m_pobjKaigyoConvFrameWnd;		// ���s�R�[�h�ϊ��p�E�C���h�E�N���X
	CKaigyoConvStatusDlg*	m_pobjKaigyoConvStatusDlg;		// ���s�R�[�h�ϊ��󋵃_�C�A���O

	// �t�@�C���Í����֘A
	BOOL					m_bIsSearchSubFolderEncript;	// �T�u�t�H���_���ϊ����邩�H
	BOOL					m_bIsOutOtherFileEncript;		// �Í����ʂ�ʃt�@�C���֏o�͂��邩�H
	CString					m_objKeyEncript;				// �t�@�C���Í����L�[
	CString					m_objFolderEncript;				// �t�@�C���Í������s�t�H���_
	CString					m_objFileKindEncript;			// �t�@�C���Í����Ώۂ̃t�@�C�����
	CEditorDoc*				m_pobjEncriptDoc;				// �t�@�C���Í����p�h�L�������g�N���X
	CFrameWnd*				m_pobjEncriptFrameWnd;			// �t�@�C���Í����p�E�C���h�E�N���X
	CEncriptStatusDlg*		m_pobjEncriptStatusDlg;			// �t�@�C���Í����󋵃_�C�A���O

	// �t�@�C���������֘A
	BOOL					m_bIsSearchSubFolderDecript;	// �T�u�t�H���_���ϊ����邩�H
	BOOL					m_bIsOutOtherFileDecript;		// �������ʂ�ʃt�@�C���֏o�͂��邩�H
	CString					m_objKeyDecript;				// �t�@�C���������L�[
	CString					m_objFolderDecript;				// �t�@�C�����������s�t�H���_
	CString					m_objFileKindDecript;			// �t�@�C���������Ώۂ̃t�@�C�����
	CEditorDoc*				m_pobjDecriptDoc;				// �t�@�C���������p�h�L�������g�N���X
	CFrameWnd*				m_pobjDecriptFrameWnd;			// �t�@�C���������p�E�C���h�E�N���X
	CDecriptStatusDlg*		m_pobjDecriptStatusDlg;			// �t�@�C���������󋵃_�C�A���O

	// �_���v�֘A
	CEditorDoc*				m_pobjDumpDoc;					// �_���v�p�h�L�������g�N���X
	CFrameWnd*				m_pobjDumpFrameWnd;				// �_���v�p�E�C���h�E�N���X

	// ���[�h�J�E���g�֘A
	long					m_lByteCountSum;				// �o�C�g�����v
	long					m_lWordCountSum;				// ���[�h�����v
	long					m_lLineCountSum;				// �s�����v
	long					m_lEmptyLineCountSum;			// ��s�����v
	long					m_lCPPCommentLineCountSum;		// �b�E�b�{�{�R�����g�s�����v
	long					m_lWCountFileCnt;				// ���[�h�J�E���g�Ώۃt�@�C����
	BOOL					m_bIsSearchSubFolderWCount;		// �T�u�t�H���_���������邩�H
	CString					m_objFolderWCount;				// ���[�h�J�E���g���s�t�H���_
	CString					m_objFileKindWCount;			// ���[�h�J�E���g�Ώۂ̃t�@�C�����
	CEditorDoc*				m_pobjWCountDoc;				// ���[�h�J�E���g�p�h�L�������g�N���X
	CFrameWnd*				m_pobjWCountFrameWnd;			// ���[�h�J�E���g�p�q�E�C���h�E�N���X
	CWCntStatusDlg*			m_pobjWCntStatusDlg;			// ���[�h�J�E���g�󋵃_�C�A���O

	// �t�@�C�������֘A
	BOOL					m_bIsSearchSubFolderFSearch;	// �T�u�t�H���_���ϊ����邩�H
	CString					m_objFolderFSearch;				// �t�@�C�������ϊ����s�t�H���_
	CString					m_objFileKindFSearch;			// �����Ώۂ̃t�@�C�����
	CEditorDoc*				m_pobjFSearchDoc;				// �t�@�C�������p�h�L�������g�N���X
	CFrameWnd*				m_pobjFSearchFrameWnd;			// �t�@�C�������p�E�C���h�E�N���X
	CFileSearchStatusDlg*	m_pobjFileSearchStatusDlg;		// �t�@�C�������󋵃_�C�A���O

	// �t�@�C���^�b�`�֘A
	BOOL					m_bIsSearchSubFolderTouch;		// �T�u�t�H���_���^�b�`���邩�H
	CString					m_objFolderTouch;				// �^�b�`���s�t�H���_
	CString					m_objFileKindTouch;				// �^�b�`�Ώۂ̃t�@�C�����
	CTime					m_objFileTouchTime;				// �^�b�`����

	// �I�[�g�Z�[�u�֘A
	BOOL					m_bIsAutoSave;					// �I�[�g�Z�[�u�����邩�H
	long					m_lSaveInterval;				// �I�[�g�Z�[�u�Ԋu�i���P�ʁj
	long					m_lTimeUpCount;					// �^�C���A�b�v�J�E���^

	// �L�[����̋L�^�^�Đ��֘A
	BOOL					m_bIsNowOperationRecording;		// �L�[����L�^���t���O
	BOOL					m_bIsNowStepDoOperation;		// �L�[����̃X�e�b�v���s���t���O
	COperationManager		m_objOperationManager;			// �L�[����}�l�[�W��

	// �X�e�[�^�X�o�[�֘A
	LOGFONT					m_stStatusBarLogFont;			// �X�e�[�^�X�o�[��LOGFONT�\����
	CFont					m_objStatusBarFont;				// �X�e�[�^�X�o�[�̃t�H���g
	STATUSBARDISP			m_stStatusBarDisp;				// �X�e�[�^�X�o�[�\�����\����
	long					m_lStatusDayStyle;				// �X�e�[�^�X�o�[�֕\��������t�\���X�^�C��
	long					m_lStatusHourStyle;				// �X�e�[�^�X�o�[�֕\�����鎞���\���X�^�C��
	long					m_lStatusLineStyle;				// �X�e�[�^�X�o�[�֕\������s�A��\���X�^�C��
	long					m_lSavedGyo;					// �s�ԍ�
	long					m_lSavedRetu;					// ��ԍ�
	long					m_lSavedMaxGyo;					// �S�s��

	// �t�@�C����r�ԘA
	CFileCompareStatusDlg*	m_pobjFileCompareStatusDlg;		// �t�@�C����r�󋵃_�C�A���O

	// �f�B���N�g����r�֘A
	CDirCompareStatusDlg*	m_pobjDirCompareStatusDlg;		// �f�B���N�g����r�󋵃_�C�A���O
	CString					m_objFolder1DirCmp;				// ����r�t�H���_
	CString					m_objFolder2DirCmp;				// ����r�t�H���_
	CEditorDoc*				m_pobjDirCmpDoc;				// �f�B���N�g����r�p�h�L�������g�N���X
	CFrameWnd*				m_pobjDirCmpFrameWnd;			// �f�B���N�g����r�p�E�C���h�E�N���X

	// ���[�����X�g�֘A
	CMailListStatusDlg*		m_pobjMailListStatusDlg;		// ���[�����X�g�󋵃_�C�A���O

protected:
	CStatusBar				m_wndStatusBar;
	CToolBarEx				m_wndToolBar;					// �W���c�[���o�[
	CToolBarEx				m_wndToolBarEdit;				// �ҏW�֘A�c�[���o�[
	CToolBarEx				m_wndToolBarFind;				// �����֘A�c�[���o�[
	CToolBarEx				m_wndToolBarJmp;				// �W�����v�֘A�c�[���o�[
	CToolBarEx				m_wndToolBarTools;				// �c�[���֘A�c�[���o�[
	CToolBarEx				m_wndToolBarWnd;				// �E�B���h�E�֘A�c�[���o�[
	CBitmapClient			m_wndClient;					// ���S�t���̂l�c�h�N���C�A���g

	//�c�[���o�[�J�X�^�}�C�Y�p
	static CToolBarInfo		m_wndToolBar_Info[];			// �W���c�[���o�[���
	static CToolBarInfo		m_wndToolBarEdit_Info[];		// �ҏW�֘A�c�[���o�[���
	static CToolBarInfo		m_wndToolBarFind_Info[];		// �����֘A�c�[���o�[���
	static CToolBarInfo		m_wndToolBarJmp_Info[];			// �W�����v�֘A�c�[���o�[���
	static CToolBarInfo		m_wndToolBarTools_Info[];		// �c�[���֘A�c�[���o�[���
	static CToolBarInfo		m_wndToolBarWnd_Info[];			// �E�C���h�E�֘A�c�[���o�[���

protected:
	void	WaitForHtmlOpenDlgDisp();						// �g�s�l�k�I�[�v���󋵃_�C�A���O�\���҂�

// 	afx_msg LONG OnFindReplace(WPARAM,LPARAM);				// �����A�u���n���h��
	afx_msg LRESULT OnMSHMouseWheel(WPARAM,LPARAM);			// �C���e���}�E�X�̃z�C�[���X�N���[��
	BOOL	LoadWindowPlacement(LPWINDOWPLACEMENT);			// �E�C���h�E�ʒu���[�h����
	void	SaveWindowPlacement(LPWINDOWPLACEMENT);			// �E�C���h�E�ʒu�Z�[�u����

	BOOL	CreateToolBar();
	void	DockControlBarLeftOf(CToolBar*,CToolBar*);

	void	DoGrep();										// �f�q�d�o���s����
	BOOL	DoGrepInOneDirectory(CString,CString,CString);	// �P�f�B���N�g�����f�q�d�o���s����
	BOOL	DoGrepInOneFile(CString,CString,CString);		// �P�t�@�C�����f�q�d�o���s����
	void	WaitForGrepDlgDisp();							// �f�q�d�o�󋵃_�C�A���O�\���҂�
	BOOL	IsGrepCanceled();								// �f�q�d�o�L�����Z���`�F�b�N

	void	DoCodeConv();									// �����R�[�h�ϊ����s����
	BOOL	DoConvInOneDirectory(CString,CString,int,int);	// �P�f�B���N�g���������R�[�h�ϊ����s����
	BOOL	DoConvInOneFile(CString,CString,CString,int,int);// �P�t�@�C�������R�[�h�ϊ����s����
	void	WaitForConvDlgDisp();							// �����R�[�h�ϊ��󋵃_�C�A���O�\���҂�
	BOOL	IsConvCanceled();								// �����R�[�h�ϊ��L�����Z���`�F�b�N

	void	DoKaigyoCodeConv();								// ���s�R�[�h�ϊ����s����
	BOOL	DoKaigyoConvInOneDirectory(CString,CString,CString);// �P�f�B���N�g�������s�R�[�h�ϊ����s����
	BOOL	DoKaigyoConvInOneFile(CString,CString,CString,CString);// �P�t�@�C�����s�R�[�h�ϊ����s����
	void	WaitForKaigyoConvDlgDisp();						// ���s�R�[�h�ϊ��󋵃_�C�A���O�\���҂�
	BOOL	IsKaigyoConvCanceled();							// ���s�R�[�h�ϊ��L�����Z���`�F�b�N

	void	DoWordCount();									// ���[�h�J�E���g���s����
	BOOL	DoWCountInOneDirectory(CString,CString);		// �P�f�B���N�g�������[�h�J�E���g���s����
	BOOL	DoWCountInOneFile(CString,CString);				// �P�t�@�C�������[�h�J�E���g���s����
	void	WaitForWCntDlgDisp();							// ���[�h�J�E���g�󋵃_�C�A���O�\���҂�
	BOOL	IsWCntCanceled();								// ���[�h�J�E���g�L�����Z���`�F�b�N

	void	DoFileSearch();									// �t�@�C���������s����
	BOOL	DoFSearchInOneDirectory(CString,CString);		// �P�f�B���N�g�����t�@�C���������s����
	void	WaitForFSearchDlgDisp();						// �t�@�C�������󋵃_�C�A���O�\���҂�
	BOOL	IsFSearchCanceled();							// �t�@�C�������L�����Z���`�F�b�N

	void	DoFileEncript();								// �t�@�C���Í������s����
	BOOL	DoEncriptInOneDirectory(CString,CString,CString);// �P�f�B���N�g�����t�@�C���Í������s����
	BOOL	DoEncriptInOneFile(CString,CString,CString,CString);// �P�t�@�C���Í������s����
	void	WaitForEncriptDlgDisp();						// �t�@�C���Í����󋵃_�C�A���O�\���҂�
	BOOL	IsEncriptCanceled();							// �t�@�C���Í����L�����Z���`�F�b�N

	void	DoFileDecript();								// �t�@�C�����������s����
	BOOL	DoDecriptInOneDirectory(CString,CString,CString);// �P�f�B���N�g�����t�@�C�����������s����
	BOOL	DoDecriptInOneFile(CString,CString,CString,CString);// �P�t�@�C�����������s����
	void	WaitForDecriptDlgDisp();						// �t�@�C���������󋵃_�C�A���O�\���҂�
	BOOL	IsDecriptCanceled();							// �t�@�C���������L�����Z���`�F�b�N

	void	WaitForFileCompareStatusDlgDisp();				// �t�@�C����r�󋵃_�C�A���O�\���҂�

	void	DoDirCompare();									// �f�B���N�g����r���s����
	BOOL	DoDirCmpInOneDirectory();						// �P�f�B���N�g����r���s
	void	WaitForDirCompareStatusDlgDisp();				// �f�B���N�g����r�󋵃_�C�A���O�\���҂�

	void	DoFileTouch();									// �t�@�C���^�b�`���s����
	BOOL	DoFileTouchInOneDirectory(CString,CString,CTime);// �P�f�B���N�g�����t�@�C���^�b�`���s

	void	WaitForMailListDlgDisp();						// ���[�����X�g�󋵃_�C�A���O�\���҂�

// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnSettings();
	afx_msg void OnUpdateSettings(CCmdUI* pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateEditReplace(CCmdUI* pCmdUI);
	afx_msg void OnEditFindcontinue();
	afx_msg void OnEditFindrevcontinue();
	afx_msg void OnUpdateEditFindcontinue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFindrevcontinue(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateWindowAllclose(CCmdUI* pCmdUI);
	afx_msg void OnWindowAllclose();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnUpdateEditGrep(CCmdUI* pCmdUI);
	afx_msg void OnEditGrep();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnUpdateEditCodeconv(CCmdUI* pCmdUI);
	afx_msg void OnEditCodeconv();
	afx_msg void OnUpdateEditAsciidump(CCmdUI* pCmdUI);
	afx_msg void OnEditAsciidump();
	afx_msg void OnUpdateEditSjisdump(CCmdUI* pCmdUI);
	afx_msg void OnEditSjisdump();
	afx_msg void OnUpdateFileReread(CCmdUI* pCmdUI);
	afx_msg void OnFileReread();
	afx_msg void OnUpdateFileProperty(CCmdUI* pCmdUI);
	afx_msg void OnFileProperty();
	afx_msg void OnEditSetreadonly();
	afx_msg void OnUpdateEditSetreadonly(CCmdUI* pCmdUI);
	afx_msg void OnEditUnsetreadonly();
	afx_msg void OnUpdateEditUnsetreadonly(CCmdUI* pCmdUI);
	afx_msg void OnEditWordcount();
	afx_msg void OnUpdateEditWordcount(CCmdUI* pCmdUI);
	afx_msg void OnEditFilesearch();
	afx_msg void OnUpdateEditFilesearch(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveclose();
	afx_msg void OnUpdateFileSaveclose(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeStartendrecord();
	afx_msg void OnUpdateToolOpeStartendrecord(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeDo();
	afx_msg void OnUpdateToolOpeDo(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeSave();
	afx_msg void OnUpdateToolOpeSave(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeOpen();
	afx_msg void OnUpdateToolOpeOpen(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeDisp();
	afx_msg void OnUpdateToolOpeDisp(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeStepdoNext();
	afx_msg void OnUpdateToolOpeStepdoNext(CCmdUI* pCmdUI);
	afx_msg void OnToolOpeStepdoEnd();
	afx_msg void OnUpdateToolOpeStepdoEnd(CCmdUI* pCmdUI);
	afx_msg void OnWndNext();
	afx_msg void OnUpdateWndNext(CCmdUI* pCmdUI);
	afx_msg void OnWndPrev();
	afx_msg void OnUpdateWndPrev(CCmdUI* pCmdUI);
	afx_msg void OnEditClipview();
	afx_msg void OnUpdateEditClipview(CCmdUI* pCmdUI);
	afx_msg void OnEditIncrimentalsearch();
	afx_msg void OnUpdateEditIncrimentalsearch(CCmdUI* pCmdUI);
	afx_msg void OnSecurityEncript();
	afx_msg void OnUpdateSecurityEncript(CCmdUI* pCmdUI);
	afx_msg void OnSecurityDecript();
	afx_msg void OnUpdateSecurityDecript(CCmdUI* pCmdUI);
	afx_msg void OnEditUuencode();
	afx_msg void OnUpdateEditUuencode(CCmdUI* pCmdUI);
	afx_msg void OnEditBase64encode();
	afx_msg void OnUpdateEditBase64encode(CCmdUI* pCmdUI);
	afx_msg void OnEditQuotedencode();
	afx_msg void OnUpdateEditQuotedencode(CCmdUI* pCmdUI);
	afx_msg void OnEditFilecompare();
	afx_msg void OnUpdateEditFilecompare(CCmdUI* pCmdUI);
	afx_msg void OnEditDircompare();
	afx_msg void OnUpdateEditDircompare(CCmdUI* pCmdUI);
	afx_msg void OnFileDelete();
	afx_msg void OnUpdateFileDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditFiletouch();
	afx_msg void OnUpdateEditFiletouch(CCmdUI* pCmdUI);
	afx_msg void OnWindowRestore();
	afx_msg void OnUpdateWindowRestore(CCmdUI* pCmdUI);
	afx_msg void OnWindowMinimize();
	afx_msg void OnUpdateWindowMinimize(CCmdUI* pCmdUI);
	afx_msg void OnWindowMaximize();
	afx_msg void OnUpdateWindowMaximize(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarWnd();
	afx_msg void OnUpdateViewToolbarWnd(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarTools();
	afx_msg void OnUpdateViewToolbarTools(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarFind();
	afx_msg void OnUpdateViewToolbarFind(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarEdit();
	afx_msg void OnUpdateViewToolbarEdit(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	afx_msg void OnViewToolbarJmp();
	afx_msg void OnUpdateViewToolbarJmp(CCmdUI* pCmdUI);
	afx_msg void OnFileMaillist();
	afx_msg void OnUpdateFileMaillist(CCmdUI* pCmdUI);
	afx_msg void OnEditKaigyoconv();
	afx_msg void OnUpdateEditKaigyoconv(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbar();
	afx_msg void OnUpdateCustomizeToolbar(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbaredit();
	afx_msg void OnUpdateCustomizeToolbaredit(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbarfind();
	afx_msg void OnUpdateCustomizeToolbarfind(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbarjmp();
	afx_msg void OnUpdateCustomizeToolbarjmp(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbartools();
	afx_msg void OnUpdateCustomizeToolbartools(CCmdUI* pCmdUI);
	afx_msg void OnCustomizeToolbarwnd();
	afx_msg void OnUpdateCustomizeToolbarwnd(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowClose();
	afx_msg void OnUpdateWindowClose(CCmdUI* pCmdUI);
	afx_msg void OnHttpFileOpen();
	afx_msg void OnEditFcompAlphabetbigandsmall();
	afx_msg void OnUpdateEditFcompAlphabetbigandsmall(CCmdUI* pCmdUI);
	afx_msg void OnEditFcompTabequal2space();
	afx_msg void OnUpdateEditFcompTabequal2space(CCmdUI* pCmdUI);
	afx_msg void OnEditFcompZenequal2han();
	afx_msg void OnUpdateEditFcompZenequal2han(CCmdUI* pCmdUI);
	afx_msg void OnEditFilerecompare();
	afx_msg void OnUpdateEditFilerecompare(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(MAINFRM_H)


