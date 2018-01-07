// Editor.h : EDITOR �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#include "resource.h"       // ���C�� �V���{��

#if !defined(EDITORAPP_H)		//�Q�d�C���N���[�h�h�~
#define EDITORAPP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <ras.h>
#include <raserror.h>

//�q�`�r�c�k�k�֐��A�h���X
typedef DWORD (CALLBACK* LPFNRASFUNC1)();								//rasGetEntryNeedSize
typedef LONG  (CALLBACK* LPFNRASFUNC2)(LPRASENTRYNAME,LPDWORD);			//rasGetRasEntry
typedef LONG  (CALLBACK* LPFNRASFUNC3)(char*,char*,char*,char*,char*);	//rasConnect
typedef LONG  (CALLBACK* LPFNRASFUNC4)(LPDWORD,char*);					//rasIsConnected
typedef void  (CALLBACK* LPFNRASFUNC5)();								//rasDisConnect

//�\�P�b�g�c�k�k�֐��A�h���X
typedef LONG  (CALLBACK* LPFNSCKFUNC1)(char*,char*);						//GetLocalHostInfo
typedef LONG  (CALLBACK* LPFNSCKFUNC2)(WORD);								//SockServerOpen
typedef LONG  (CALLBACK* LPFNSCKFUNC3)(LONG,LONG,LPVOID,DWORD,LPDWORD);		//SockServerSend
typedef LONG  (CALLBACK* LPFNSCKFUNC4)(LONG,LONG,LPVOID,LPDWORD);			//SockServerRecv
typedef LONG  (CALLBACK* LPFNSCKFUNC5)(LONG);								//SockServerClose
typedef LONG  (CALLBACK* LPFNSCKFUNC6)(char*,WORD,BOOL);					//SockClientOpen
typedef LONG  (CALLBACK* LPFNSCKFUNC7)(LONG,LPVOID,DWORD,LPDWORD);			//SockClientSend
typedef LONG  (CALLBACK* LPFNSCKFUNC8)(LONG,LPVOID,LPDWORD);				//SockClientRecv
typedef LONG  (CALLBACK* LPFNSCKFUNC9)(LONG);								//SockClientClose
typedef LONG  (CALLBACK* LPFNSCKFUNC10)(LONG,LPVOID,DWORD,DWORD,LPDWORD);	//SockClientRecv2

//���K�\���c�k�k�֐��A�h���X
typedef int  (CALLBACK* LPFNREGFUNC1)(const char*,int);						//myRegcomp
typedef int  (CALLBACK* LPFNREGFUNC2)(const char*,int,int*,int*);			//myRegexec
typedef void (CALLBACK* LPFNREGFUNC3)(int,char*,size_t);					//myRegerror
typedef void (CALLBACK* LPFNREGFUNC4)(void);								//myRegfree
typedef BOOL (CALLBACK* LPFNREGFUNC5)(const char*,int);						//myIsNeedRegcomp

class CDdeServer;
class CEditorDoc;
class CKeyBind;
class CKeyBindManager;

class CEditorApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEditorApp();											// �R���X�g���N�^

	void	MySetForegroundWindow(HWND);
	void	SetFlag2AllWindow(CString&,BOOL&,BOOL&,BOOL&,BOOL&,BOOL&,BOOL&);
	void	SetFontSettings2AllWindow(CEditorDoc*);
	void	SetColorSettings2AllWindow();
	BOOL	MakeXerxesShortcut(BOOL);
	BOOL	CreateShortCut(const char*,char*,char*);		// �V���[�g�J�b�g�쐬����
	LONG	GetRegKey(HKEY,LPCTSTR,LPTSTR);					// ���W�X�g���L�[�擾����
	void	OpenHelpTopix(ULONG);							// �w���v�g�s�b�N�I�[�v������
	void	DoFileAllSave();								// �S�t�@�C���ۑ����s����
	void	MessagePump();									// �E�C���h�E�Y���b�Z�[�W����
	int		GetMaxMRU();									// �l�q�t�t�@�C�����擾
	void	SetMaxMRU(int);									// �l�q�t�t�@�C�����ݒ�
	BOOL	IsShowSplash();									// �X�v���b�V���X�N���[���\������
	void	SetShowSplash(BOOL);							// �X�v���b�V���X�N���[���\���ݒ�
	BOOL	IsOpenNewFile();								// �N�����V�K�q�E�C���h�E�쐬����
	void	SetOpenNewFile(BOOL);							// �N�����V�K�q�E�C���h�E�쐬�ݒ�
	BOOL	IsChdirToOpenFile();							// �I�[�v���t�@�C���f�B���N�g���ړ�����
	void	SetChdirToOpenFile(BOOL);						// �I�[�v���t�@�C���f�B���N�g���ړ��ݒ�
	void	PrepareMyReadString();							// ����A�[�J�C�u���͏���
	int		MyReadString(CArchive&,CString&,long*,long*);	// ����A�[�J�C�u����
	BOOL	CreateNewMDIChdWnd(CDocument**,CFrameWnd**);	// �V�K�q�E�C���h�E�쐬
	void	SendJumpMsg(const char*,long);					// �w��s�W�����v���b�Z�[�W���M
	void	CheckFileChange();								// �t�@�C���ύX�`�F�b�N
	void	GetFileErrMessage(CString&, CFileException&);	// �t�@�C���A�N�Z�X�G���[���b�Z�[�W�擾
	BOOL	IsDriveRoot();									// �h���C�u���[�g�f�B���N�g���`�F�b�N
	BOOL	IsDriveRoot(const char*);						// �h���C�u���[�g�f�B���N�g���`�F�b�N
	void	ExecuteLogging(const char*);					// ���O�o��
	int		GetPrintOrientation();							// ����p�������擾
	void	SetPrintOrientation(int);						// ����p�������ݒ�
	void	GetPrintMargin(CRect&);							// ����]���}�[�W���擾
	void	SetPrintMargin(CRect&);							// ����]���}�[�W���ݒ�
	void	OpenFile(CString&);								// �w��t�@�C���I�[�v��
	void	RegisterShellExtDll();							// �V�F���G�N�X�e���V�����c�k�k�o�^
	void	UnRegisterShellExtDll();						// �V�F���G�N�X�e���V�����c�k�k�o�^����
	void	ChangeDirectory(const char*);					// �f�B���N�g���ړ�
	BOOL	BrowseUrl(CString&);							// �w��t�q�k���u���E�Y
	BOOL	ExecMailer(CString&);							// ���[���[���N��
	CKeyBind* FindKeyBind(UINT,BOOL,BOOL,BOOL);				// �L�[�o�C���h����
	CKeyBind* FindKeyBind(DWORD);							// �L�[�o�C���h����
	void	addKeyBind(CKeyBind&);							// �L�[�o�C���h�o�^
	void	resetKeyBind();									// �L�[�o�C���h���Z�b�g
	void	writeKeyBind();									// �L�[�o�C���h��������
	void	readKeyBind();									// �L�[�o�C���h�ǂݍ���
	BOOL	MyPlaySound(UINT,BOOL);
	BOOL	MyPlaySound(const char*,BOOL);
	int		MyMessageBox(LPCTSTR,UINT,UINT nIDHelp=0);
	void	RemoveRecentFileList(const char*);
	BOOL	SetMenuOfIE(BOOL);								// �h�d��Xerxes�ҏW���j���[�ǉ��^�폜
	BOOL	TryDDE(const char*);							// DDE�ʐM�g���C

	CString	m_strOpenFileDirectory;			// �I�[�v�������t�@�C���̃f�B���N�g��
	CString	m_strModuleFileName;			// ���W���[���t�@�C����
	CString	m_strExePath;					// �N���f�B���N�g���p�X
	CString	m_strRecvMailPath;				// ��M�ς݃��[���ۑ���p�X
	CString	m_strRecvMailAttachPath;		// ��M�ς݃��[���Y�t�t�@�C���ۑ���p�X
	CString	m_strMediaDirPath;
	CString	m_strCashePath;					// HTML�L���b�V���p�p�X
	CString	m_strShelExtDllPath;			// �V�F���G�N�X�e���V�����c�k�k��
	UINT	m_unOrgCaretBlinkTime;							// �L�����b�g�_�ŊԊu�I���W�i���l
	HINSTANCE m_hRasDllInstance;							// ras_dll.dll�C���X�^���X�n���h��
	HINSTANCE m_hSockDllInstance;							// sock_dll.dll�C���X�^���X�n���h��
	HINSTANCE m_hRegexDllInstance;							// regex_dll.dll�C���X�^���X�n���h��
	LPFNRASFUNC1	m_lpfnRasFunc1;							// ras_dll.dll�֐��A�h���X
	LPFNRASFUNC2	m_lpfnRasFunc2;
	LPFNRASFUNC3	m_lpfnRasFunc3;
	LPFNRASFUNC4	m_lpfnRasFunc4;
	LPFNRASFUNC5	m_lpfnRasFunc5;
	LPFNSCKFUNC1	m_lpfnSckFunc1;							// sock_dll.dll �֐��A�h���X
	LPFNSCKFUNC2	m_lpfnSckFunc2;
	LPFNSCKFUNC3	m_lpfnSckFunc3;
	LPFNSCKFUNC4	m_lpfnSckFunc4;
	LPFNSCKFUNC5	m_lpfnSckFunc5;
	LPFNSCKFUNC6	m_lpfnSckFunc6;
	LPFNSCKFUNC7	m_lpfnSckFunc7;
	LPFNSCKFUNC8	m_lpfnSckFunc8;
	LPFNSCKFUNC9	m_lpfnSckFunc9;
	LPFNSCKFUNC10	m_lpfnSckFunc10;
	LPFNREGFUNC1	m_lpfnRegFunc1;							// regex_dll.dll �֐��A�h���X
	LPFNREGFUNC2	m_lpfnRegFunc2;
	LPFNREGFUNC3	m_lpfnRegFunc3;
	LPFNREGFUNC4	m_lpfnRegFunc4;
	LPFNREGFUNC5	m_lpfnRegFunc5;

protected:
	void	getMyDirectory();								// �f�B���N�g����͏���
	void	paramAnalyze(CCommandLineInfo&);				// �p�����[�^���
	BOOL	IsLicenseRegistered();							// ���C�Z���X�o�^�ς݃`�F�b�N����
	BOOL	IsExpired(BOOL);								// �]���������`�F�b�N����
	void	GenerateLisenceKeys();							// ���C�Z���X�L�[�쐬

private:
	BOOL	m_bIsDriveRoot;
	int		m_nPrintOrientation;							// ����̗p������
	CRect	m_objPrintMarginRect;							// ����]���̃}�[�W��
	int		m_nMaxMRU;										// ���O�Ɏg�p�����t�@�C����ێ����鐔
	BOOL	m_bIsOpenNewFile;								// �N�����ɐV�K�q�E�C���h�E���쐬���邩
	BOOL	m_bIsChdirToOpenFile;							// �I�[�v�������t�@�C���̃f�B���N�g���ֈړ����邩
	BOOL	m_bShowSplash;									// �X�v���b�V���X�N���[����\�����邩
	FILE*	m_fpLogging;									// ���M���O�p�t�@�C���|�C���^
	char	m_cBuf4MyReadString;							// ����A�[�J�C�u���͗p�����ꎞ�o�b�t�@
	CDdeServer*	m_pobjDdeServer;							// �c�c�d�T�[�o�[
	HANDLE	m_hMutex;
	CKeyBindManager* m_pobjKeyBindManager;

	//{{AFX_VIRTUAL(CEditorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEditorApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileAllsave();
	afx_msg void OnUpdateFileAllsave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpenonexit(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenonexit();
	afx_msg void OnHelpLicense();
	afx_msg void OnHelpContent();
	afx_msg void OnUpdateHelpContent(CCmdUI* pCmdUI);
	afx_msg void OnAllsaveExit();
	afx_msg void OnUpdateAllsaveExit(CCmdUI* pCmdUI);
	afx_msg void OnHelpContent2();
	afx_msg void OnUpdateHelpContent2(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(EDITORAPP_H)

