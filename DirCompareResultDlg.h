// DirCompareResultDlg.h : CDirCompareResultDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(DirCompareResultDLG_H)		//�Q�d�C���N���[�h�h�~
#define DirCompareResultDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CStaticFilespec;

class CDirCompareResultDlg : public CDialog
{
public:
	CDirCompareResultDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	CString m_objDir1Name;					// ���f�B���N�g����
	CString m_objDir2Name;					// ���f�B���N�g����
	long	m_lCompareFileCount;
	long	m_lAddFileCount;
	long	m_lDelFileCount;
	long	m_lDiffFileCount;
	long	m_lTime;

	//{{AFX_DATA(CDirCompareResultDlg)
	enum { IDD = IDD_DIRCOMPARERESULT };
	CString	m_objStatusStr;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDirCompareResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDirCompareResultDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*		m_pApp;
	CStaticFilespec   m_wndStaticDirName1;
	CStaticFilespec   m_wndStaticDirName2;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(DirCompareResultDLG_H)

