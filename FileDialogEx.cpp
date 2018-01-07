// FileDialogEx.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "FileDialogEx.h"
#include <Dlgs.h>
#include "CodeConvManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
	//{{AFX_MSG_MAP(CFileDialogEx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CFileDialogEx::CFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	SetTemplate(0,IDD_CUSTOM_FILEOPEN);
	m_pApp = (CEditorApp*)AfxGetApp();

	//{{AFX_DATA_INIT(CFileDialogEx)
	//}}AFX_DATA_INIT
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＸデータ交換														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileDialogEx::DoDataExchange(CDataExchange* pDX)
{
	CFileDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDialogEx)
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：初期化終了															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileDialogEx::OnInitDone()
{
	CFileDialog::OnInitDone();
	CString objMsg;

	CRect r, rPath, rEdit, rChk, rTime;

	CWnd* pList = GetParent()->GetDlgItem(lst1);
	pList->GetWindowRect(r);
	GetParent()->ScreenToClient(r);

	//IDC_PATHコントロールを、リストビューコントロールに揃えられるようにサイズ変更する
	CWnd* pWnd = GetDlgItem(IDC_PATH);
	if(pWnd!=NULL)
	{
		pWnd->GetWindowRect(rPath);
		GetParent()->ScreenToClient(rPath);
		pWnd->SetWindowPos(NULL,r.left,rPath.top,r.Width(),rPath.Height(),SWP_NOACTIVATE|SWP_NOZORDER);
	}
	//IDC_TIMEコントロールを、リストビューコントロールに揃えられるようにサイズ変更する
	pWnd = GetDlgItem(IDC_TIME);
	if(pWnd!=NULL)
	{
		pWnd->GetWindowRect(rTime);
		GetParent()->ScreenToClient(rTime);
		pWnd->SetWindowPos(NULL,r.left,rTime.top,r.Width(),rTime.Height(),SWP_NOACTIVATE|SWP_NOZORDER);
	}
	//IDC_EDIT_PREVIEWコントロールを、リストビューコントロールに揃えられるようにサイズ変更する
	pWnd = GetDlgItem(IDC_EDIT_PREVIEW);
	if(pWnd!=NULL)
	{
		pWnd->GetWindowRect(rEdit);
		GetParent()->ScreenToClient(rEdit);
		pWnd->SetWindowPos(NULL,r.left,rEdit.top,r.Width(),rEdit.Height(),SWP_NOACTIVATE|SWP_NOZORDER);

		//フォント設定
		pWnd->SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	}

	pWnd = GetParent()->GetDlgItem(chx1);
	if(pWnd)
	{
		objMsg.LoadString(IDS_FOPEN_AS_READONLY);
		pWnd->SetWindowText(objMsg);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル名変更時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileDialogEx::OnFileNameChange()
{
	CString objMsg;
	CString objStrMTime;
	CString objStr, objStr2,objBuf;
	CString objFilePath = GetPathName();
	CFileStatus			objFileStatus;

	//ファイル更新日時とサイズ取得
	if(CFile::GetStatus(objFilePath,objFileStatus))
	{
		//ファイル情報
		int nSize = (int)(objFileStatus.m_size/1024);
		nSize += ((objFileStatus.m_size%1024) ? 1 : 0);
		objMsg.LoadString(IDS_FILEPROP_MTIME);
		objStrMTime = objFileStatus.m_mtime.Format(objMsg);
		objMsg.LoadString(IDS_FILEPROP_INFO);
		objBuf.Format(objMsg,objStrMTime,nSize);
		SetDlgItemText(IDC_TIME,objBuf);

		//読み取り専用チェックを有効化
		CWnd* pWnd = GetParent()->GetDlgItem(chx1);
		if(pWnd)
			pWnd->EnableWindow(TRUE);
	}
	else
	{
		//ファイル情報
		objBuf = _T("");
		SetDlgItemText(IDC_TIME,objBuf);
		//読み取り専用チェックを無効化
		CWnd* pWnd = GetParent()->GetDlgItem(chx1);
		if(pWnd)
			pWnd->EnableWindow(FALSE);
		//プレビューウインドウクリア
		SetDlgItemText(IDC_EDIT_PREVIEW,"");
		return;
	}

	//ファイル文字コードチェック
	CCodeConvManager	objCodeConvManager;
	UINT nCode=0;
	nCode = objCodeConvManager.DetectCode((LPCTSTR)objFilePath);
	if(nCode != INPUT_ASCII && nCode != INPUT_SJIS)
	{
		objMsg = _T("");
		if(nCode == INPUT_BINARY)
			objMsg.LoadString(IDS_FILEPROP_BINARY);
		else if(nCode == INPUT_EUC)
			objMsg.LoadString(IDS_FILEPROP_EUC);
		else if(nCode == INPUT_JIS)
			objMsg.LoadString(IDS_FILEPROP_JIS);
		else if(nCode == INPUT_UNKNOWN)
			objMsg.LoadString(IDS_FILEPROP_UNKNOWN);

		SetDlgItemText(IDC_EDIT_PREVIEW,objMsg);
		return;
	}

	CFile objFile;
	if(objFile.Open(objFilePath,CFile::modeRead)==FALSE)
	{
		SetDlgItemText(IDC_EDIT_PREVIEW,"");
		return;
	}
	CArchive ar(&objFile,CArchive::load);

	int  nNowRet=0;
	int  nBeforeRet=0;
	long lCRCount=0;
	long lLFCount=0;
	m_pApp->PrepareMyReadString();
	for(int nCnt=0; nCnt<40; nCnt++)//４０行分読む
	{
		lCRCount = lLFCount = 0;
		nNowRet = m_pApp->MyReadString(ar,objStr,&lCRCount,&lLFCount);
		if( nNowRet == 0 ) //EOF終了
		{
			if( objStr.IsEmpty() ) //データなし
				break;
		}

		objStr2 += objStr;
		objStr2 += "\r\n";
	}
	ar.Close();
	objFile.Close();

	SetDlgItemText(IDC_EDIT_PREVIEW,objStr2);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：フォルダ変更時処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CFileDialogEx::OnFolderChange()
{
	//IDC_PATHのキャプションを現在のディレクトリパスに設定
	CString objBuf, objMsg;
	objMsg.LoadString(IDS_FILEPROP_PATH);
	objBuf.Format(objMsg, GetFolderPath());
	SetDlgItemText(IDC_PATH,objBuf);
}

