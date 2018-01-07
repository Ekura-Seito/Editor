// FileCompareResultDlg.h : CFileCompareResultDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(FileCompareResultDLG_H)		//�Q�d�C���N���[�h�h�~
#define FileCompareResultDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CFileCompareResultDlg : public CDialog
{
public:
	CFileCompareResultDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	CString m_objFile1Name;					// ���t�@�C����
	CString m_objFile2Name;					// ���t�@�C����

	//{{AFX_DATA(CFileCompareResultDlg)
	enum { IDD = IDD_FILECOMPARERESULT };
	CString	m_objAddLineCountStr;
	CString	m_objDelLineCountStr;
	CString	m_objStatusStr;
	CString	m_objTimeStr;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFileCompareResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileCompareResultDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	CStaticFilespec   m_wndStaticFileName1;
	CStaticFilespec   m_wndStaticFileName2;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(FileCompareResultDLG_H)

