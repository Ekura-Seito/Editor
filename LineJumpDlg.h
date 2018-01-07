// LineJumpDlg.h : CLineJumpDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(LINEJUMPDLG_H)		//�Q�d�C���N���[�h�h�~
#define LINEJUMPDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CLineJumpDlg : public CDialog
{
public:
	CLineJumpDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	long		m_lMaxNo;							//�ԍ��ő�l

	//{{AFX_DATA(CLineJumpDlg)
	enum { IDD = IDD_DIALOG_JUMP };
	UINT	m_lLineNo;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CLineJumpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CLineJumpDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CEditorApp*	m_pApp;
	HICON		m_hIcon;
	HICON		m_hIconS;
};

#endif // !defined(LINEJUMPDLG_H)

