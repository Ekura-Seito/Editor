/////////////////////////////////////////////////////////////////////////////
// CHtmlOpenDlg �_�C�A���O

#if !defined(HTMLOPENDLG_H)
#define HTMLOPENDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CMyComboBoxEx;
class CEditorApp;
class CAutoResizer;

class CHtmlOpenDlg : public CDialog
{
public:
	CHtmlOpenDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	//{{AFX_DATA(CHtmlOpenDlg)
	enum { IDD = IDD_HTMLOPEN };
	CMyComboBoxEx	m_objComboRequestPath;
	CString		m_objRequestPath;
	BOOL		m_bIsUseProxy;
	CEdit		m_objEditProxyIPAddress;
	//}}AFX_DATA

	CString			m_objSite;
	CString			m_objHost;					//�ڑ���z�X�g
	CString			m_objPath;					//�v���p�X
	CString			m_objFileName;				//�t�@�C����
	CString			m_objFileExt;				//�t�@�C���g���q
	CString			m_objPastRequestPath[20];	//�ߋ��ɊJ�����g�s�l�k�p�X
	CString			m_objProxyIPAddress;

	//{{AFX_VIRTUAL(CHtmlOpenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CHtmlOpenDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckUseproxy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CEditorApp*		m_pApp;						// �A�v���P�[�V�����N���X�ւ̃|�C���^
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(HTMLOPENDLG_H)

