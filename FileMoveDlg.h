// FileMoveDlg.h : CFileMoveDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(FILEMOVEDLG_H)		//�Q�d�C���N���[�h�h�~
#define FILEMOVEDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;
class CAutoResizer;

class CFileMoveDlg : public CDialog
{
public:
	CFileMoveDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_DATA(CFileMoveDlg)
	enum { IDD = IDD_FILEMOVE };
	CEdit	m_objEditFolder;
	//}}AFX_DATA

	char		m_szFromPath[_MAX_PATH];				//�ړ����p�X
	CString		m_objFolder;							//�ړ���f�B���N�g���̃f�t�H���g
	char		m_szToPath[_MAX_PATH];					//�m�肵���ړ���t�@�C���p�X

	//{{AFX_VIRTUAL(CFileMoveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileMoveDlg)
	afx_msg void OnBrowse();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CToolTipCtrl m_tooltip;
	CEditorApp*	m_pApp;
	char		m_szFromDir[_MAX_PATH];
	char		m_szFromName[_MAX_FNAME];
	CStaticFilespec   m_wndStaticFileName;
	CAutoResizer	m_objAutoResizer;
	HICON		m_hIcon;
	HICON		m_hIconS;
};

#endif //!defined(FILEMOVEDLG_H)

