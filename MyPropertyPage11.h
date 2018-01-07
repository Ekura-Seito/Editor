// MyPropertyPage11.h : ヘッダー ファイル
//

#if !defined(MYPROPERTYPAGE11_H)
#define MYPROPERTYPAGE11_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CMyPropertyPage11 : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyPropertyPage11)

public:
	CMyPropertyPage11();
	~CMyPropertyPage11();

	BOOL IsChanged();									// 変更発生調査
	void ClearChangeFlag();								// 
	void ChangesToRegistry();

	//{{AFX_DATA(CMyPropertyPage11)
	enum { IDD = IDD_PROPPAGE11 };
	CComboBox	m_objWaveFilesCombo;
	CTreeCtrl	m_objSoundsTreeCtrl;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMyPropertyPage11)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMyPropertyPage11)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTest();
	afx_msg void OnSelchangeComboWavefiles();
	afx_msg void OnSelchangedTreeSounds(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	BOOL MakeFileList(CString&,CObArray*);
	void AddFileBySort(CString&,CObArray*);

private:
	CEditorApp*		m_pApp;
	CImageList		m_ImageList;
	SOUND_DEF		m_stSoundDef[14];
	BOOL			m_bIsChanged;							//変更発生フラグ
	HTREEITEM		m_hRootItems[4];
};

#endif // !defined(MYPROPERTYPAGE11_H)

