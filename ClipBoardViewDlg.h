// ClipBoardViewDlg.h : CClipBoardViewDlg �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(CLIPBOARDVIEWDLG_H)		//�Q�d�C���N���[�h�h�~
#define CLIPBOARDVIEWDLG_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CAutoResizer;

class CClipBoardViewDlg : public CDialog
{
public:
	CClipBoardViewDlg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	//{{AFX_DATA(CClipBoardViewDlg)
	enum { IDD = IDD_CLIPBOARDVIEW };
	CEdit	m_objClipContentsEdit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CClipBoardViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

protected:
	void ReadFromClipBoard(CString&);			// �N���b�v�{�[�h����ǂ�

	//{{AFX_MSG(CClipBoardViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL LoadWindowPlacement(LPWINDOWPLACEMENT);
	void SaveWindowPlacement(LPWINDOWPLACEMENT);

private:
	CEditorApp*		m_pApp;						//�A�v���P�[�V�����N���X�ւ̃|�C���^
	CAutoResizer	m_objAutoResizer;
	HICON			m_hIcon;
	HICON			m_hIconS;
};

#endif // !defined(CLIPBOARDVIEWDLG_H)

