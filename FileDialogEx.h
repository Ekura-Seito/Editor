// FileDialogEx.h : ヘッダー ファイル
//

#if !defined(AFX_FILEDIALOGEX_H__D4FA7A01_1748_11D5_920F_0000C07B38EF__INCLUDED_)
#define AFX_FILEDIALOGEX_H__D4FA7A01_1748_11D5_920F_0000C07B38EF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;

class CFileDialogEx : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogEx)

public:
	CFileDialogEx(BOOL bOpenFileDialog, // TRUE ならば FileOpen、 FALSE ならば FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

	//{{AFX_DATA(CFileDialogEx)
	enum { IDD = IDD_CUSTOM_FILEOPEN };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFileDialogEx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CFileDialogEx)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnFolderChange();
	virtual void OnFileNameChange();
	virtual void OnInitDone();

private:
	CEditorApp* m_pApp;
};

#endif // !defined(AFX_FILEDIALOGEX_H__D4FA7A01_1748_11D5_920F_0000C07B38EF__INCLUDED_)

