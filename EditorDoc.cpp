// EditorDoc.cpp : CEditorDoc クラスの動作の定義を行います。
//

#include "stdafx.h"
#include <Afxtempl.h>			//ＭＦＣのＣＭａｐを使用する
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <direct.h>
#include <io.h>					//for _access
#include "Editor_Def.h"
#include "ColorManager.h"
#include "Editor.h"
#include "Line.h"
#include "LineManager.h"
#include "UndoItem.h"
#include "UndoItemAddCharacter.h"
#include "UndoItemDelChar.h"
#include "UndoItemReturn.h"
#include "UndoItemBackSpaceChar.h"
#include "UndoItemCutSelectArea.h"
#include "UndoItemEditPaste.h"
#include "UndoItemReplaceSelectWord.h"
#include "UndoItemTabWhenSelected.h"
#include "UndoItemShiftTabWhenSelected.h"
#include "UndoItemDelTopSpaces.h"
#include "UndoItemTab2Space.h"
#include "UndoItemSpace2Tab.h"
#include "UndoItemSpaceZen2Han.h"
#include "UndoItemSpaceHan2Zen.h"
#include "UndoItemKatakanaHan2Zen.h"
#include "UndoItemKatakanaZen2Han.h"
#include "UndoItemAlnumHan2Zen.h"
#include "UndoItemAlnumZen2Han.h"
#include "UndoItemOleDrop.h"
#include "UndoItemInsCppComment.h"
#include "UndoItemDelCppComment.h"
#include "UndoItemInsVbComment.h"
#include "UndoItemDelVbComment.h"
#include "UndoItemInsAsmComment.h"
#include "UndoItemDelAsmComment.h"
#include "UndoItemInsQuote.h"
#include "UndoItemDelQuote.h"
#include "UndoItemInsDay.h"
#include "UndoItemInsTime.h"
#include "UndoItemAl2Upper.h"
#include "UndoItemAl2Lower.h"
#include "UndoManager.h"
#include "UuDecoder.h"
#include "Base64.h"
#include "QuotedPrintable.h"
#include "EditorDoc.h"
#include "BCMenu.h"
#include "EditorView.h"
#include "CodeConvManager.h"
#include "StaticFilespec.h"
#include "AutoResizer.h"
#include "FileMoveDlg.h"
#include "FileCopyDlg.h"
#include "FileRenameDlg.h"
#include "smtp.h"
#include "MyComboBoxEx.h"
#include "MailSendDlg.h"
#include "MailSendStatusDlg.h"

#ifdef _DEBUG
#define	new	DEBUG_NEW
#undef THIS_FILE
static char	THIS_FILE[]	= __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc

IMPLEMENT_DYNCREATE(CEditorDoc,	CDocument)

BEGIN_MESSAGE_MAP(CEditorDoc, CDocument)
	//{{AFX_MSG_MAP(CEditorDoc)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_VIEW_TAB_4, OnViewTab4)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_4,	OnUpdateViewTab4)
	ON_COMMAND(ID_VIEW_TAB_8, OnViewTab8)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_8,	OnUpdateViewTab8)
	ON_COMMAND(ID_EDIT_CLEARUNDOBUF, OnEditClearundobuf)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEARUNDOBUF, OnUpdateEditClearundobuf)
	ON_COMMAND(ID_FILE_OPENSAMENAME, OnFileOpensamename)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENSAMENAME, OnUpdateFileOpensamename)
	ON_COMMAND(ID_FILE_COPY, OnFileCopy)
	ON_UPDATE_COMMAND_UI(ID_FILE_COPY, OnUpdateFileCopy)
	ON_COMMAND(ID_FILE_RENAME, OnFileRename)
	ON_UPDATE_COMMAND_UI(ID_FILE_RENAME, OnUpdateFileRename)
	ON_COMMAND(ID_FILE_MOVE, OnFileMove)
	ON_UPDATE_COMMAND_UI(ID_FILE_MOVE, OnUpdateFileMove)
	ON_COMMAND(ID_FILE_SHELLOPEN, OnFileShellopen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SHELLOPEN, OnUpdateFileShellopen)
	ON_COMMAND(ID_EDIT_CODE2EUC, OnEditCode2euc)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CODE2EUC, OnUpdateEditCode2euc)
	ON_COMMAND(ID_EDIT_CODE2JIS, OnEditCode2jis)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CODE2JIS, OnUpdateEditCode2jis)
	ON_COMMAND(ID_EDIT_CODE2SJIS, OnEditCode2sjis)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CODE2SJIS, OnUpdateEditCode2sjis)
	ON_COMMAND(ID_VIEW_TAB_2, OnViewTab2)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TAB_2, OnUpdateViewTab2)
	ON_COMMAND(ID_FILE_SENDMAIL, OnFileSendmail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SENDMAIL, OnUpdateFileSendmail)
	ON_COMMAND(ID_EDIT_CRLF, OnEditCrlf)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CRLF, OnUpdateEditCrlf)
	ON_COMMAND(ID_EDIT_CR, OnEditCr)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CR, OnUpdateEditCr)
	ON_COMMAND(ID_EDIT_LF, OnEditLf)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LF, OnUpdateEditLf)
	ON_COMMAND(ID_EDIT_ALLUNDO, OnEditAllundo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ALLUNDO, OnUpdateEditAllundo)
	ON_COMMAND(ID_EDIT_ALLREDO, OnEditAllredo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ALLREDO, OnUpdateEditAllredo)
	ON_COMMAND(ID_EDIT_SOURCEARRANGE, OnEditSourcearrange)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SOURCEARRANGE, OnUpdateEditSourcearrange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc クラスの構築/消滅

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CEditorDoc::CEditorDoc()
{
	m_pApp = (CEditorApp*)AfxGetApp();

	m_dwFileAttributes = 0;
	m_bConvMode	= 0;
	m_bGrepMode	= 0;
	m_bDumpMode	= 0;
	m_bWCountMode =	0;
	m_bFSearchMode = 0;
	m_bBinaryMode =	0;
	m_bEncriptMode = 0;
	m_bDecriptMode = 0;
	m_bEncodeMode = 0;
	m_pobjLastWrittenTime =	NULL;
	m_bDelEndSpacesOnSave =	0;
	m_bSpace2TabOnSave = 0;
	m_bCompareMode = 0;
	m_objFileCmpFile1 = _T("");
	m_objFileCmpFile2 = _T("");
	m_bDirCmpMode = 0;
	m_objDirCmpFolder1 = _T("");
	m_objDirCmpFolder2 = _T("");
	m_bIsPrintPreview = 0;
	m_pobjMailSendStatusDlg = NULL;

	m_bDelEndSpacesOnSave =	m_pApp->GetProfileInt("CEditorDoc","bDelEndSpacesOnSave",0);
	m_bSpace2TabOnSave = m_pApp->GetProfileInt("CEditorDoc","bSpace2TabOnSave",0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CEditorDoc::~CEditorDoc()
{
	if(m_pobjLastWrittenTime!=NULL)
		delete m_pobjLastWrittenTime;

	m_pApp->WriteProfileInt("CEditorDoc","bDelEndSpacesOnSave",m_bDelEndSpacesOnSave);
	m_pApp->WriteProfileInt("CEditorDoc","bSpace2TabOnSave",m_bSpace2TabOnSave);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ドキュメント初期化処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::OnNewDocument()
{
	//基底クラスのハンドラコール
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI	ドキュメントはこのドキュメントを再利用します。)
	m_dwFileAttributes = 0;
	m_strFileName = "";
	m_strFilePath = "";
	m_strFileDir = "";
	m_strFileExt = "";
	m_objUndoManager.Initialize();//Undo,Redo管理クラス初期化
	if(m_pobjLastWrittenTime!=NULL)
		delete m_pobjLastWrittenTime;
	m_pobjLastWrittenTime=NULL;

	//ファイル拡張子に対応した色、フォントに設定
	SetFontOfCurrentFileExt(m_strFileExt);
	SetColorOfCurrentFileExt(m_strFileExt);

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ドキュメントオープン時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::OnOpenDocument(LPCTSTR	lpszPathName)
{
	//バイナリファイルチェック
	CCodeConvManager	objCodeConvManager;
	if(objCodeConvManager.DetectCode(lpszPathName) == INPUT_BINARY)
		SetBinaryMode(TRUE);
	else
		SetBinaryMode(FALSE);

	//基底クラスのハンドラコール
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	//ファイル名取得
	m_strFilePath = lpszPathName;
	long lPos=0;
	for(long lCnt=0; lCnt<m_strFilePath.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,(BYTE)m_strFilePath.GetAt(lCnt)))//２ﾊﾞｲﾄ文字の第一ﾊﾞｲﾄ
		{
			lCnt+=2;
		}
		else
		{
			if(m_strFilePath.GetAt(lCnt)=='\\')
			{
				//'\'位置保持
				lPos = lCnt;
			}
			lCnt++;
		}
	}
	m_strFileName = m_strFilePath.Mid(lPos+1,m_strFilePath.GetLength()-lPos);
	m_pApp->WriteProfileString("CEditorApp","OpenFileName",m_strFileName);

	//ファイルディレクトリ取得
	CString strDir = lpszPathName;
	int nROffset = strDir.ReverseFind('\\');
	int nFOffset = strDir.Find('\\');
	if( nROffset != nFOffset )
		strDir.SetAt(nROffset,0);
	else
		strDir.SetAt(nROffset+1,0);
	m_strFileDir = strDir;

	//ファイル拡張子取得
	lPos = -1;
	for(long lCnt=0; lCnt<m_strFileName.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,(BYTE)m_strFileName.GetAt(lCnt)))
		{
			lCnt+=2;
		}
		else
		{
			if(m_strFileName.GetAt(lCnt)=='.')
			{
				//'.'位置保持
				lPos = lCnt;
			}
			lCnt++;
		}
	}
	if(lPos != -1)
		m_strFileExt = m_strFileName.Mid(lPos+1,m_strFileName.GetLength()-lPos);
	else
		m_strFileExt = "";

	//ファイル拡張子に対応した色、フォントに設定
	SetFontOfCurrentFileExt(m_strFileExt);
	SetColorOfCurrentFileExt(m_strFileExt);

	//ディレクトリ取得
	if(m_pApp->GetProfileInt("CEditorApp","IsChdirToOpenFile",0) != 0)
	{
		//オープンしたファイルのディレクトリへ移動
		m_pApp->ChangeDirectory(m_strFileDir);
	}

	//ファイル最終更新日時取得
	if(m_pobjLastWrittenTime!=NULL)
		delete m_pobjLastWrittenTime;
	m_pobjLastWrittenTime =	new	CTime;
	CFileStatus		objFileStatus;
	CFile::GetStatus(m_strFilePath,objFileStatus);
	(*m_pobjLastWrittenTime) = objFileStatus.m_mtime;

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ドキュメント保存時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	BOOL bRet=0;

	//基底クラスのハンドラコール
	bRet = CDocument::OnSaveDocument(lpszPathName);

	//ファイル名取得
	m_strFilePath = lpszPathName;
	long lPos=0;
	for(long lCnt=0; lCnt<m_strFilePath.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,(BYTE)m_strFilePath.GetAt(lCnt)))//２ﾊﾞｲﾄ文字の第一ﾊﾞｲﾄ
		{
			lCnt+=2;
		}
		else
		{
			if(m_strFilePath.GetAt(lCnt)=='\\')
			{
				//'\'位置保持
				lPos = lCnt;
			}
			lCnt++;
		}
	}
	m_strFileName = m_strFilePath.Mid(lPos+1,m_strFilePath.GetLength()-lPos);

	//ファイルディレクトリ取得
	CString strDir = lpszPathName;
	int nROffset = strDir.ReverseFind('\\');
	int nFOffset = strDir.Find('\\');
	if( nROffset != nFOffset )
		strDir.SetAt(nROffset,0);
	else
		strDir.SetAt(nROffset+1,0);
	m_strFileDir = strDir;

	//ファイル拡張子取得
	lPos = -1;
	for(long lCnt=0; lCnt<m_strFileName.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,(BYTE)m_strFileName.GetAt(lCnt)))
		{
			lCnt+=2;
		}
		else
		{
			if(m_strFileName.GetAt(lCnt)=='.')
			{
				//'.'位置保持
				lPos = lCnt;
			}
			lCnt++;
		}
	}
	if(lPos != -1)
		m_strFileExt = m_strFileName.Mid(lPos+1,m_strFileName.GetLength()-lPos);
	else
		m_strFileExt = "";

	//ファイル拡張子に対応した色、フォントに設定
	SetFontOfCurrentFileExt(m_strFileExt);
	SetColorOfCurrentFileExt(m_strFileExt);

	//ディレクトリ取得
	if(m_pApp->GetProfileInt("CEditorApp","IsChdirToOpenFile",0) != 0)
	{
		//オープンしたファイルのディレクトリへ移動
		m_pApp->ChangeDirectory(m_strFileDir);
	}

	//ファイル最終更新日時取得
	if(m_pobjLastWrittenTime!=NULL)
		delete m_pobjLastWrittenTime;
	m_pobjLastWrittenTime =	new	CTime;
	CFileStatus		objFileStatus;
	CFile::GetStatus(lpszPathName,objFileStatus);
	(*m_pobjLastWrittenTime) = objFileStatus.m_mtime;

	return bRet;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ドキュメントクローズ時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnCloseDocument()
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	m_dwFileAttributes = 0;
	m_strFileName = "";
	m_strFilePath = "";
	if(m_pobjLastWrittenTime!=NULL)
		delete m_pobjLastWrittenTime;
	m_pobjLastWrittenTime=NULL;

	//基底クラスのハンドラコール
	CDocument::OnCloseDocument();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ドキュメントタイトル設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetTitle(LPCTSTR lpszTitle)
{
	CString title;

	//設定取得
	BOOL bIsFullTitle =	m_pApp->GetProfileInt("CEditorDoc","bIsFullTitle",0);

	if(	bIsFullTitle !=	0 )	//タイトルフルパス表示
	{
		title = m_strFilePath;
		if(	strstr(lpszTitle," *") != 0 )
			title += " *";

		if(	m_strFilePath.GetLength() )
			CDocument::SetTitle(title);
		else
			CDocument::SetTitle(lpszTitle);
	}
	else
	{
		CDocument::SetTitle(lpszTitle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc シリアライゼーション

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：シリアライズ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::Serialize(CArchive& ar)
{
	char* pPtr=NULL;
	long	lCnt=0;
	CFile* pobjFile	= ar.GetFile();

	if(ar.IsStoring()) //保存
	{
		//行末空白削除フラグチェック
		if(GetFlagDelEndSpacesOnSave())
		{
			for(lCnt=0;	lCnt<m_objLineManager.GetLineCount(); lCnt++)
			{
				CLine* pobjLine	= m_objLineManager.GetOneLine(lCnt);
				if(pobjLine	!= NULL)
				{
					pobjLine->DelEndSpaces();
					m_objLineManager.SetOneLine(lCnt,pobjLine);
				}
			}
			m_objUndoManager.Initialize();	//Undo,Redo	管理クラス初期化
		}
		//空白－＞タブ変換フラグチェック
		if(GetFlagSpace2TabOnSave())
		{
			long lfWidth=0;
			POSITION pos = GetFirstViewPosition();
			while(pos != 0)
			{
				CEditorView* pView = (CEditorView*)GetNextView(pos);
				lfWidth	= pView->m_lfWidth;
				break;
			}
			m_objLineManager.ConvSpace2TabOnSave(lfWidth,GetTabSize());
			m_objUndoManager.Initialize();	//Undo,Redo	管理クラス初期化
		}

		m_objLineManager.Serialize(ar);

		if(	GetFlagDelEndSpacesOnSave()	|| GetFlagSpace2TabOnSave()	)
		{
			POSITION pos = GetFirstViewPosition();
			while(pos != 0)
			{
				CEditorView* pView = (CEditorView*)GetNextView(pos);
				//再描画
				pView->OnUpdate(NULL,NULL,NULL);
				pView->Invalidate(TRUE);
//				//キャレット位置を初期位置へ
//				pView->MySetCaretPos(CPoint(pView->m_LineNoPos.x,pView->m_caretPos.y));
				m_pApp->MessagePump();
			}
		}
	}
	else //読み込み
	{
		//ファイル属性調査
		CString	objFilePath	= pobjFile->GetFilePath();
		m_dwFileAttributes = GetFileAttributes(	(LPCTSTR)objFilePath );
		if(IsBinary()) //バイナリ表示モード
		{
			//読み込み専用とする
			m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
		}

		//読み込み
		if(IsBinary())
		{
			//バイナリ表示モードで読み込み
			m_objLineManager.SerializeBinary(ar);
		}
		else
		{
			//テキスト表示モードで読み込み
			m_objLineManager.Serialize(ar);
		}
		m_objUndoManager.Initialize();	//Undo,Redo	管理クラス初期化
//1999.05.16 Replace Start
//		UpdateAllViews(NULL,NULL,NULL);
		POSITION pos = GetFirstViewPosition();
		while(pos != NULL)
		{
			CEditorView* pView = (CEditorView*)GetNextView(pos);
			pView->OnUpdate(NULL,NULL,NULL);
			pView->Invalidate(TRUE);
		}
//1999.05.16 Replace End
	}

	//ドキュメントクリーン
	SetModifiedFlag(FALSE);

	//タイトル更新
	CString	objTitle = GetTitle();
	if(	(objTitle.GetLength() >	2) &&
		(objTitle.Right(2) == "	*")
	)
	{
		objTitle = objTitle.Left(objTitle.GetLength() -	2);
		SetTitle(objTitle);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc クラスの診断

#ifdef _DEBUG
void CEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEditorDoc::Dump(CDumpContext&	dc)	const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc メッセージハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－上書き保存」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnUpdateFileSave(CCmdUI* pCmdUI)
{
	if(IsReadOnly())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－名前を付けて保存」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnUpdateFileSaveAs(CCmdUI*	pCmdUI)
{
	if(IsReadOnly())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－同じ名前のファイルを開く」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileOpensamename() 
{
	DWORD	dwRet=0;
	CString objFilter,objFolder,objOneKind,objPath;
	int		nOpenCount=0;
	CString fname="";

	//ファイル名（拡張子なし）取得
	long lPos=0;
	long lCnt=0;
	for(lCnt=0; lCnt<m_strFileName.GetLength(); )
	{
		if(IsDBCSLeadByteEx(0,(BYTE)m_strFileName.GetAt(lCnt)))//２ﾊﾞｲﾄ文字の第一ﾊﾞｲﾄ
		{
			lCnt+=2;
		}
		else
		{
			if(m_strFileName.GetAt(lCnt)=='.')
			{
				fname.Format("%s.*",m_strFileName.Left(lCnt));
				break;
			}
			lCnt++;
		}
	}
	objOneKind = fname;

	//ファイル検索キー作成
	objFolder = m_strFileDir;
	if(m_pApp->IsDriveRoot(objFolder))
		objFilter.Format("%s%s",objFolder,objOneKind);
	else
		objFilter.Format("%s\\%s",objFolder,objOneKind);

	//同名のファイルを検索
	CFileFind	objFileFind;
	nOpenCount = 0;
	BOOL bRet = objFileFind.FindFile(objFilter);
	if(bRet != FALSE)
	{
		while(objFileFind.FindNextFile()!=FALSE)
		{
			if( objFileFind.IsDots() != FALSE ) // "." or ".."
				continue;
			if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				continue;
			//同名ファイルオープン
			objPath = objFileFind.GetFilePath();
			if(objPath.CompareNoCase(m_strFilePath) != 0)
			{
				m_pApp->OpenFile(objPath);
				nOpenCount++;
			}
		}
		//最後の１個
		if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
		{
			objPath = objFileFind.GetFilePath();
			if(objPath.CompareNoCase(m_strFilePath) != 0)
			{
				m_pApp->OpenFile(objPath);
				nOpenCount++;
			}
		}
	}
	objFileFind.Close();

	if(nOpenCount == 0)
	{
		CString	objStr, objMsg;
		objMsg.LoadString(IDS_NOFILE_INTHISDIR);
		objStr.Format((LPCTSTR)objMsg, m_strFileDir);
		m_pApp->MyMessageBox((LPCTSTR)objStr,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorDoc::OnUpdateFileOpensamename(CCmdUI* pCmdUI) 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ファイル名チェック
	if(	m_strFilePath.GetLength()==0 || m_strFileName.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－関連づけられたソフトで開く」選択時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileShellopen() 
{
	CString objMsg;
	CWaitCursor	objCursor;
	HINSTANCE result = ::ShellExecute(NULL, _T("open"), m_strFilePath, NULL,NULL, SW_SHOW);
	if((UINT)result <= HINSTANCE_ERROR)
	{
		objMsg.LoadString(IDS_SOFTKICK_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
	}
}
void CEditorDoc::OnUpdateFileShellopen(CCmdUI* pCmdUI) 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ファイル名チェック
	if(	m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－移動」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileMove() 
{
	CFileMoveDlg	objFileMoveDlg;

	//ファイル移動ダイアログ表示
	strcpy(objFileMoveDlg.m_szFromPath,m_strFilePath);
	objFileMoveDlg.m_objFolder = m_pApp->GetProfileString("CEditorDoc","FileMoveToDir","");
	if(objFileMoveDlg.DoModal() != IDOK)
		return;
	m_pApp->WriteProfileString("CEditorDoc","FileMoveToDir",objFileMoveDlg.m_objFolder);

	SHFILEOPSTRUCT	stFileOp;
	memset(&stFileOp,0,sizeof(SHFILEOPSTRUCT));
	char	szFrom[_MAX_PATH],szTo[_MAX_PATH];
	memset(szFrom,0,sizeof(szFrom));
	memset(szTo,0,sizeof(szTo));

	//マウスカーソルを砂時計にする
	CWaitCursor	objWaitCursor;

	//途中経過を表示するウインドウの親のウインドウ
	CWnd* pWnd = (CWnd*)AfxGetMainWnd();
	stFileOp.hwnd = pWnd->m_hWnd;
	//オペレーション
	stFileOp.wFunc = FO_MOVE;
	//元
	strcpy(szFrom, m_strFilePath);
	stFileOp.pFrom = szFrom;
	//先
	strcpy(szTo, objFileMoveDlg.m_szToPath);
	stFileOp.pTo = szTo;
	//動作に関するフラグ
	stFileOp.fFlags = FOF_ALLOWUNDO;

	//移動処理実行
	if( ::SHFileOperation(&stFileOp) == 0 ) //OK
	{
		if( _access(objFileMoveDlg.m_szToPath,0) != -1 &&
			_access(m_strFilePath,0) == -1
		)
		{
			//ドキュメントクラスに新しいパスを登録
			m_strFilePath = objFileMoveDlg.m_szToPath;
			SetPathName(m_strFilePath);

			//ディレクトリ移動
			if(m_pApp->GetProfileInt("CEditorApp","IsChdirToOpenFile",0) != 0)
			{
				//オープンしたファイルのディレクトリへ移動
				m_pApp->ChangeDirectory((LPCTSTR)objFileMoveDlg.m_objFolder);
			}

			//ファイル最終更新日時取得
			if(m_pobjLastWrittenTime!=NULL)
				delete m_pobjLastWrittenTime;
			m_pobjLastWrittenTime =	new	CTime;
			CFileStatus		objFileStatus;
			CFile::GetStatus(m_strFilePath,objFileStatus);
			(*m_pobjLastWrittenTime) = objFileStatus.m_mtime;
		}
	}
}
void CEditorDoc::OnUpdateFileMove(CCmdUI* pCmdUI) 
{
	//ファイル名チェック
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if( IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－コピー」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileCopy() 
{
	CFileCopyDlg	objFileCopyDlg;

	//コピー先指定ダイアログ表示
	objFileCopyDlg.m_bRenameOnCollision = m_pApp->GetProfileInt("CEditorDoc","RenameOnCollision",0);
	objFileCopyDlg.m_objFile            = m_strFilePath;
	objFileCopyDlg.m_objFolder          = m_pApp->GetProfileString("CEditorDoc","FileCopyToDir","");
	if(objFileCopyDlg.DoModal() != IDOK)
		return;
	m_pApp->WriteProfileString("CEditorDoc","FileCopyToDir",objFileCopyDlg.m_objFolder);
	m_pApp->WriteProfileInt("CEditorDoc","RenameOnCollision",objFileCopyDlg.m_bRenameOnCollision);

	SHFILEOPSTRUCT	stFileOp;
	memset(&stFileOp,0,sizeof(SHFILEOPSTRUCT));
	char	szFrom[_MAX_PATH],szTo[_MAX_PATH];
	memset(szFrom,0,sizeof(szFrom));
	memset(szTo,0,sizeof(szTo));

	//マウスカーソルを砂時計にする
	CWaitCursor	objWaitCursor;

	//途中経過を表示するウインドウの親のウインドウ
	CWnd* pWnd = (CWnd*)AfxGetMainWnd();
	stFileOp.hwnd = pWnd->m_hWnd;
	//オペレーション
	stFileOp.wFunc = FO_COPY;
	//コピー元
	strcpy(szFrom, m_strFilePath);
	stFileOp.pFrom = szFrom;
	//コピー先
	strcpy(szTo, (LPCTSTR)objFileCopyDlg.m_objFolder);
	stFileOp.pTo = szTo;
	//動作に関するフラグ
	if(objFileCopyDlg.m_bRenameOnCollision != FALSE)
		stFileOp.fFlags = FOF_ALLOWUNDO|FOF_RENAMEONCOLLISION;
	else
		stFileOp.fFlags = FOF_ALLOWUNDO;

	//ファイルコピー処理実行
	::SHFileOperation(&stFileOp);
}
void CEditorDoc::OnUpdateFileCopy(CCmdUI* pCmdUI) 
{
	//ファイル名チェック
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if( IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ファイル－名前の変更」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileRename() 
{
	long	lCnt=0;
	CFileRenameDlg	objFileRenameDlg;

	//ファイル名変更ダイアログ表示
	strcpy(objFileRenameDlg.m_szFromPath,m_strFilePath);
	objFileRenameDlg.m_objToFile = m_pApp->GetProfileString("CEditorDoc","FileRenameTo","");
	if(objFileRenameDlg.DoModal() != IDOK)
		return;
	m_pApp->WriteProfileString("CEditorDoc","FileRenameTo",objFileRenameDlg.m_objToFile);

	SHFILEOPSTRUCT	stFileOp;
	memset(&stFileOp,0,sizeof(SHFILEOPSTRUCT));
	char	szFrom[_MAX_PATH],szTo[_MAX_PATH];
	memset(szFrom,0,sizeof(szFrom));
	memset(szTo,0,sizeof(szTo));

	//マウスカーソルを砂時計にする
	CWaitCursor	objWaitCursor;

	//途中経過を表示するウインドウの親のウインドウ
	CWnd* pWnd = (CWnd*)AfxGetMainWnd();
	stFileOp.hwnd = pWnd->m_hWnd;
	//オペレーション
	stFileOp.wFunc = FO_RENAME;
	//元
	strcpy(szFrom, m_strFilePath);
	stFileOp.pFrom = szFrom;
	//先
	strcpy(szTo, objFileRenameDlg.m_szToPath);
	stFileOp.pTo = szTo;
	//動作に関するフラグ
	stFileOp.fFlags = FOF_ALLOWUNDO;

	//名前変更処理実行
	if( ::SHFileOperation(&stFileOp) == 0 ) //OK
	{
		if( _access(objFileRenameDlg.m_szToPath,0) != -1 &&
			_access(m_strFilePath,0) == -1
		)
		{
			//タイトル変更
			m_strFileName = (LPCTSTR)objFileRenameDlg.m_objToFile;
			m_strFilePath = objFileRenameDlg.m_szToPath;
			//ファイル拡張子取得
			for(lCnt=0; lCnt<m_strFileName.GetLength(); )
			{
				if(IsDBCSLeadByteEx(0,(BYTE)m_strFileName.GetAt(lCnt)))
				{
					lCnt+=2;
				}
				else
				{
					if(m_strFileName.GetAt(lCnt)=='.')
					{
						m_strFileExt = m_strFileName.Mid(lCnt+1,m_strFileName.GetLength()-lCnt);
						break;
					}
					lCnt++;
				}
			}

			//ファイル拡張子に対応した色、フォントに設定
			SetFontOfCurrentFileExt(m_strFileExt);
			SetColorOfCurrentFileExt(m_strFileExt);

			//ドキュメントクラスに新しいパスを登録
			SetPathName(m_strFilePath);

			//ファイル最終更新日時取得
			if(m_pobjLastWrittenTime!=NULL)
				delete m_pobjLastWrittenTime;
			m_pobjLastWrittenTime =	new	CTime;
			CFileStatus		objFileStatus;
			CFile::GetStatus(m_strFilePath,objFileStatus);
			(*m_pobjLastWrittenTime) = objFileStatus.m_mtime;
		}
	}
}
void CEditorDoc::OnUpdateFileRename(CCmdUI* pCmdUI) 
{
	//ファイル名チェック
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if( IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－元に戻す」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditUndo()
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(m_objUndoManager.IsCanUndo())
	{
		m_objUndoManager.doUndo();
//		UpdateAllViews(NULL,NULL,NULL);
		POSITION pos = GetFirstViewPosition();
		while(pos != NULL)
		{
			CEditorView* pView = (CEditorView*)GetNextView(pos);
			pView->OnUpdate(NULL,NULL,NULL);
			pView->Invalidate(TRUE);
		}
	}
	else
	{
		MessageBeep((UINT)-1);
	}
}
void CEditorDoc::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_objUndoManager.IsCanUndo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－繰り返し」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditRedo()
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(m_objUndoManager.IsCanRedo())
	{
		m_objUndoManager.doRedo();
//		UpdateAllViews(NULL,NULL,NULL);
		POSITION pos = GetFirstViewPosition();
		while(pos != 0)
		{
			CEditorView* pView = (CEditorView*)GetNextView(pos);
			pView->OnUpdate(NULL,NULL,NULL);
			pView->Invalidate(TRUE);
		}
	}
	else
	{
		MessageBeep((UINT)-1);
	}
}
void CEditorDoc::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_objUndoManager.IsCanRedo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－Undo/Redoバッファクリア」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditClearundobuf()
{
	m_objUndoManager.Initialize();
}
void CEditorDoc::OnUpdateEditClearundobuf(CCmdUI* pCmdUI)
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_objUndoManager.IsCanUndo()	|| m_objUndoManager.IsCanRedo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－すべて元に戻す」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditAllundo() 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(m_objUndoManager.IsCanUndo())
	{
		while(m_objUndoManager.IsCanUndo())
		{
			m_objUndoManager.doUndo();
			m_pApp->MessagePump();

//			UpdateAllViews(NULL,NULL,NULL);
			POSITION pos = GetFirstViewPosition();
			while(pos != NULL)
			{
				CEditorView* pView = (CEditorView*)GetNextView(pos);
				pView->OnUpdate(NULL,NULL,NULL);
				pView->Invalidate(TRUE);
			}
		}
	}
	else
	{
		MessageBeep((UINT)-1);
	}
}
void CEditorDoc::OnUpdateEditAllundo(CCmdUI* pCmdUI) 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_objUndoManager.IsCanUndo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－すべて繰り返し」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditAllredo() 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(m_objUndoManager.IsCanRedo())
	{
		while(m_objUndoManager.IsCanRedo())
		{
			m_objUndoManager.doRedo();
			m_pApp->MessagePump();

//			UpdateAllViews(NULL,NULL,NULL);
			POSITION pos = GetFirstViewPosition();
			while(pos != 0)
			{
				CEditorView* pView = (CEditorView*)GetNextView(pos);
				pView->OnUpdate(NULL,NULL,NULL);
				pView->Invalidate(TRUE);
			}
		}
	}
	else
	{
		MessageBeep((UINT)-1);
	}
}
void CEditorDoc::OnUpdateEditAllredo(CCmdUI* pCmdUI) 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(m_objUndoManager.IsCanRedo())
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－改行コード DOS/Windows形式」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditCrlf() 
{
	SetKaigyoKind(KAIGYO_CRLF);
}
void CEditorDoc::OnUpdateEditCrlf(CCmdUI* pCmdUI) 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if( IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//改行コード判定
	if(GetKaigyoKind() == KAIGYO_CRLF)
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－改行コード UNIX形式」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditLf() 
{
	SetKaigyoKind(KAIGYO_LF);
}
void CEditorDoc::OnUpdateEditLf(CCmdUI* pCmdUI) 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if( IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//改行コード判定
	if(GetKaigyoKind() == KAIGYO_LF)
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－改行コード Macintosh形式」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditCr() 
{
	SetKaigyoKind(KAIGYO_CR);
}
void CEditorDoc::OnUpdateEditCr(CCmdUI* pCmdUI) 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if( IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//改行コード判定
	if(GetKaigyoKind() == KAIGYO_CR)
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－ＥＵＣ変換」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditCode2euc() 
{
	char	szTempFileName[_MAX_FNAME];
	UINT	nDetectCode=0;
	int		nRet=0;
	CCodeConvManager	objCodeConvManager;
	CString objMsg;

	//出力用一時ファイル名取得
	::GetTempFileName( m_strFileDir, "Cnv", 0, szTempFileName );

	//入力ファイル文字コードの判別
	nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if(nDetectCode != INPUT_SJIS && nDetectCode != INPUT_JIS)
		return;

	//文字コード変換実行
	if(nDetectCode == INPUT_SJIS)
		nRet = objCodeConvManager.ConvertSjisToEuc(m_strFilePath,szTempFileName);
	else if(nDetectCode == INPUT_JIS)
		nRet = objCodeConvManager.ConvertJisToEuc(m_strFilePath,szTempFileName);
	if(nRet == FALSE)
	{
		objMsg.LoadString(IDS_CONVERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//入力ファイル削除
	::DeleteFile(m_strFilePath);
	//出力ファイル名を入力ファイル名へ変更
	rename(szTempFileName,m_strFilePath);

	//再読み込み
	ReloadDocument(FALSE);
}
void CEditorDoc::OnUpdateEditCode2euc(CCmdUI* pCmdUI) 
{
	//ファイル名チェック
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if( IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//文字コード判定
	CCodeConvManager	objCodeConvManager;
	UINT nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if( nDetectCode == INPUT_BINARY  ||		//入力ファイルがバイナリファイル
		nDetectCode == INPUT_ASCII   ||		//入力ファイルに漢字コードなし
		nDetectCode == INPUT_UNKNOWN 		//文字コード不明
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else if( nDetectCode == INPUT_EUC )
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－ＳＪＩＳ変換」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditCode2sjis() 
{
	char	szTempFileName[_MAX_FNAME];
	UINT	nDetectCode=0;
	int		nRet=0;
	CCodeConvManager	objCodeConvManager;
	CString objMsg;

	//出力用一時ファイル名取得
	::GetTempFileName( m_strFileDir, "Cnv", 0, szTempFileName );

	//入力ファイル文字コードの判別
	nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if(nDetectCode != INPUT_EUC && nDetectCode != INPUT_JIS)
		return;

	//文字コード変換実行
	if(nDetectCode == INPUT_EUC)
		nRet = objCodeConvManager.ConvertEucToSjis(m_strFilePath,szTempFileName);
	else if(nDetectCode == INPUT_JIS)
		nRet = objCodeConvManager.ConvertJisToSjis(m_strFilePath,szTempFileName);
	if(nRet == FALSE)
	{
		objMsg.LoadString(IDS_CONVERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//入力ファイル削除
	::DeleteFile(m_strFilePath);
	//出力ファイル名を入力ファイル名へ変更
	rename(szTempFileName,m_strFilePath);

	//再読み込み
	ReloadDocument(FALSE);
}
void CEditorDoc::OnUpdateEditCode2sjis(CCmdUI* pCmdUI) 
{
	//ファイル名チェック
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if(	IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//文字コード判定
	CCodeConvManager	objCodeConvManager;
	UINT nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if( nDetectCode == INPUT_BINARY  ||		//入力ファイルがバイナリファイル
		nDetectCode == INPUT_ASCII   ||		//入力ファイルに漢字コードなし
		nDetectCode == INPUT_UNKNOWN 		//文字コード不明
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else if( nDetectCode == INPUT_SJIS )
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「編集－ＪＩＳ変換」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditCode2jis() 
{
	char	szTempFileName[_MAX_FNAME];
	UINT	nDetectCode=0;
	int		nRet=0;
	CCodeConvManager	objCodeConvManager;
	CString objMsg;

	//出力用一時ファイル名取得
	::GetTempFileName( m_strFileDir, "Cnv", 0, szTempFileName );

	//入力ファイル文字コードの判別
	nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if(nDetectCode != INPUT_SJIS && nDetectCode != INPUT_EUC)
		return;

	//文字コード変換実行
	if(nDetectCode == INPUT_SJIS)
		nRet = objCodeConvManager.ConvertSjisToJis(m_strFilePath,szTempFileName);
	else if(nDetectCode == INPUT_EUC)
		nRet = objCodeConvManager.ConvertEucToJis(m_strFilePath,szTempFileName);
	if(nRet == FALSE)
	{
		objMsg.LoadString(IDS_CONVERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//入力ファイル削除
	::DeleteFile(m_strFilePath);
	//出力ファイル名を入力ファイル名へ変更
	rename(szTempFileName,m_strFilePath);

	//再読み込み
	ReloadDocument(FALSE);
}
void CEditorDoc::OnUpdateEditCode2jis(CCmdUI* pCmdUI) 
{
	//ファイル名チェック
	if( m_strFileName.GetLength()==0 || m_strFilePath.GetLength()==0 )
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ＧＲＥＰ、ダンプ、コード変換、ワードカウント、ファイル検索、
	//暗号化、復号化チェック
	if(	IsReadOnly()||
		IsGrep()    ||
		IsDump()    ||
		IsConv()    ||
		IsWCount()  ||
		IsFSearch() ||
		IsBinary()  ||
		IsEncript() ||
		IsDecript() ||
		IsEncode()  ||
		IsCompare() ||
		IsDirCmp()
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//文字コード判定
	CCodeConvManager	objCodeConvManager;
	UINT nDetectCode = objCodeConvManager.DetectCode(m_strFilePath);
	if( nDetectCode == INPUT_BINARY  ||		//入力ファイルがバイナリファイル
		nDetectCode == INPUT_ASCII   ||		//入力ファイルに漢字コードなし
		nDetectCode == INPUT_UNKNOWN 		//文字コード不明
	)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
	else if( nDetectCode == INPUT_JIS )
	{
		pCmdUI->SetCheck(TRUE);
		return;
	}

	pCmdUI->SetCheck(FALSE);
	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－２タブ」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnViewTab2() 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(	GetTabSize() ==	2 )	//すでに２タブ
		return;

	//２タブ設定
	SetTabSize(2);
}
void CEditorDoc::OnUpdateViewTab2(CCmdUI* pCmdUI) 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(	GetTabSize() ==	2 )
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－４タブ」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnViewTab4()
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(	GetTabSize() ==	4 )	//すでに４タブ
		return;

	//４タブ設定
	SetTabSize(4);
}
void CEditorDoc::OnUpdateViewTab4(CCmdUI* pCmdUI)
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(	GetTabSize() ==	4 )
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「表示－８タブ」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnViewTab8()
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		return;
	}

	if(	GetTabSize() ==	8 )	//すでに８タブ
		return;

	//８タブ設定
	SetTabSize(8);
}
void CEditorDoc::OnUpdateViewTab8(CCmdUI* pCmdUI)
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	if(	GetTabSize() ==	8 )
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－メール送信」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnFileSendmail() 
{
	long			lCnt=0, lRet=0;
	UINT			unRet=0;
	char			szBuf[256],szMsg[512],szRasMsg[512],szTempFileName[_MAX_FNAME];
	CLine*			pobjLine=NULL;
	CSmtp			objSmtp;
	CMailSendDlg	objMailSendDlg;
	CString			objStr;

	//現在のアカウントに対応する、メール関連設定を取得
	char			szSection[64];
	int				nAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(nAccountNo < 0 || nAccountNo > 8)
		nAccountNo = 0;
	sprintf(szSection,"Mail_Account%d",nAccountNo);
	CString objMailAddress = m_pApp->GetProfileString(szSection,"MailAddress","");
	CString objSmtpServer  = m_pApp->GetProfileString(szSection,"SmtpServer","");
	BOOL bIsDialup         = m_pApp->GetProfileInt(szSection,"bIsDialup",0);
	CString objDialupEntry = m_pApp->GetProfileString(szSection,"DialupEntry","");
	CString objRasUserName = m_pApp->GetProfileString(szSection,"RasUserName","");
	CString objRasPasswd   = m_pApp->GetProfileString(szSection,"RasPasswd","");
	BOOL bAutoDisconnect   = m_pApp->GetProfileInt(szSection,"AutoDisconnect",0);
	int nEncodeType        = m_pApp->GetProfileInt(szSection,"EncodeType",SMTP_ENCODE_BASE64);
	CString objSignature   = m_pApp->GetProfileString(szSection,"Signature","");

	DWORD dwRasState=0;
	CString objMsg;

	//事前チェック
	if(objMailAddress.IsEmpty())
	{
		objMsg.LoadString(IDS_SMTP_NOFROM);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(objSmtpServer.IsEmpty())
	{
		objMsg.LoadString(IDS_SMTP_NOSERVER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	//宛先と件名、添付ファイルを設定してもらう
	objMailSendDlg.m_objTo        = m_pApp->GetProfileString(szSection,"SendTo","");
	if(m_pApp->GetProfileInt(szSection,"bIsAutoSetSubject",1) == 1)
		objMailSendDlg.m_objSubject = GetTitle();
	else
		objMailSendDlg.m_objSubject = _T("");
	objMailSendDlg.m_objCc        = m_pApp->GetProfileString(szSection,"SendCc","");
	objMailSendDlg.m_objBcc       = m_pApp->GetProfileString(szSection,"SendBcc","");
	objMailSendDlg.m_objReplyTo   = m_pApp->GetProfileString(szSection,"SendReplyTo",""); 
	objMailSendDlg.m_objReference = m_pApp->GetProfileString(szSection,"Reference","");
	if(objMailSendDlg.DoModal() != IDOK)
	{
		objMailSendDlg.m_objAttachFiles.RemoveAll();
		return;
	}
	m_pApp->WriteProfileString(szSection,"SendTo",objMailSendDlg.m_objTo);
	m_pApp->WriteProfileString(szSection,"SendCc",objMailSendDlg.m_objCc);
	m_pApp->WriteProfileString(szSection,"SendBcc",objMailSendDlg.m_objBcc);
	m_pApp->WriteProfileString(szSection,"SendReplyTo",objMailSendDlg.m_objReplyTo); 
	m_pApp->WriteProfileString(szSection,"Reference",objMailSendDlg.m_objReference);

	//メール送信状況モードレスダイアログ表示
	m_pobjMailSendStatusDlg = new CMailSendStatusDlg();
	m_pobjMailSendStatusDlg->Create(IDD_MAILSENDSTATUS);
	WaitForMailSendDlgDisp();

	//メール送信パラメータ設定
	objSmtp.SetEncodeType(nEncodeType);
	objSmtp.SetFrom(objMailAddress);
	for(lCnt=0; lCnt<objMailSendDlg.m_objToArray.GetSize(); lCnt++)
	{
		objStr = objMailSendDlg.m_objToArray.GetAt(lCnt);
		objSmtp.SetTo(objStr);
	}
	objSmtp.SetSubject(objMailSendDlg.m_objSubject);
	for(lCnt=0; lCnt<objMailSendDlg.m_objCcArray.GetSize(); lCnt++)
	{
		objStr = objMailSendDlg.m_objCcArray.GetAt(lCnt);
		objSmtp.SetCc(objStr);
	}
	for(lCnt=0; lCnt<objMailSendDlg.m_objBccArray.GetSize(); lCnt++)
	{
		objStr = objMailSendDlg.m_objBccArray.GetAt(lCnt);
		objSmtp.SetBcc(objStr);
	}
	objSmtp.SetReplyTo(objMailSendDlg.m_objReplyTo);
	objSmtp.SetReference(objMailSendDlg.m_objReference);
	for(lCnt=0; lCnt<objMailSendDlg.m_objAttachFiles.GetSize(); lCnt++)
	{
		objStr = objMailSendDlg.m_objAttachFiles.GetAt(lCnt);
		objSmtp.SetAttachFile(objStr);
	}
	objMailSendDlg.m_objToArray.RemoveAll();
	objMailSendDlg.m_objCcArray.RemoveAll();
	objMailSendDlg.m_objBccArray.RemoveAll();
	objMailSendDlg.m_objAttachFiles.RemoveAll();
	objSmtp.SetSignature(objSignature);
	objSmtp.m_bIsSendMessageID    = m_pApp->GetProfileInt(szSection,"bIsSendMessage",1);
	objSmtp.m_bIsSendDate         = m_pApp->GetProfileInt(szSection,"bIsSendDate",1);
	objSmtp.m_bIsReqMsgDispsition = objMailSendDlg.m_bIsReqMsgDispsition;

	if(bIsDialup) //ダイアルアップ接続
	{
		if(m_pApp->m_hRasDllInstance)
		{
			//関数アドレス取得
			m_pApp->m_lpfnRasFunc3 = (LPFNRASFUNC3)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasConnect");
			m_pApp->m_lpfnRasFunc4 = (LPFNRASFUNC4)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasIsConnected");
			m_pApp->m_lpfnRasFunc5 = (LPFNRASFUNC5)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasDisConnect");
			if( m_pApp->m_lpfnRasFunc3 != NULL && 
				m_pApp->m_lpfnRasFunc4 != NULL && 
				m_pApp->m_lpfnRasFunc5 != NULL )
			{
				//ＲＡＳ接続(rasConnect)
				objMsg.LoadString(IDS_RASCONNECT);
				m_pobjMailSendStatusDlg->SetStatus(objMsg);
				lRet = m_pApp->m_lpfnRasFunc3((char*)(LPCTSTR)objDialupEntry,
									(char*)(LPCTSTR)objRasUserName,
									(char*)(LPCTSTR)objRasPasswd,
									"",
									szBuf );
				if(lRet == -6)
				{
					//ＲＡＳ切断(rasDisconnect)
					objMsg.LoadString(IDS_RASDISCONNECT);
					m_pobjMailSendStatusDlg->SetStatus(objMsg);
					m_pApp->m_lpfnRasFunc5();
					objMsg.LoadString(IDS_RASCONNECTERR);
					sprintf(szMsg,(LPCTSTR)objMsg,szBuf);
					m_pApp->MyMessageBox(szMsg,(MB_OK|MB_ICONSTOP),0);

					//メール送信状況モードレスダイアログ消去
					m_pobjMailSendStatusDlg->DestroyWindow();
					delete m_pobjMailSendStatusDlg;
					m_pobjMailSendStatusDlg = NULL;
					return;
				}
				else if(lRet == 1) //すでにオープン済み
				{
					objMsg.LoadString(IDS_RASCONNECTEND);
					m_pobjMailSendStatusDlg->SetStatus(objMsg);
					//そのままメール送信へ
				}
				else if(lRet == 0) //ＲＡＳ接続ＯＫ
				{
					//接続完了待ち
					for(;;)
					{
						m_pApp->MessagePump();
						Sleep(10);
						if(m_pobjMailSendStatusDlg->IsCanceled())
						{
							//ＲＡＳ切断(rasDisconnect)
							objMsg.LoadString(IDS_RASDISCONNECT);
							m_pobjMailSendStatusDlg->SetStatus(objMsg);
							m_pApp->m_lpfnRasFunc5();

							//メール送信状況モードレスダイアログ消去
							m_pobjMailSendStatusDlg->DestroyWindow();
							delete m_pobjMailSendStatusDlg;
							m_pobjMailSendStatusDlg = NULL;
							return;
						}

						//状態チェック(rasIsConnected)
						memset(szRasMsg,0,sizeof(szRasMsg));
						lRet = m_pApp->m_lpfnRasFunc4( &dwRasState, szRasMsg );
						objMsg.LoadString(IDS_RASISCONNECT);
						sprintf(szMsg,(LPCTSTR)objMsg,dwRasState,szRasMsg);
						m_pobjMailSendStatusDlg->SetStatus(szMsg);
						if(lRet) //接続された
						{
							objMsg.LoadString(IDS_RASCONNECTEND);
							m_pobjMailSendStatusDlg->SetStatus(objMsg);
							break;
						}
						else
						{
							if(dwRasState == 100 || dwRasState == 8193)
							{
								//ＲＡＳ切断(rasDisconnect)
								m_pApp->m_lpfnRasFunc5();
								objMsg.LoadString(IDS_RAS_CONNECTERR);
								m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);

								//メール送信状況モードレスダイアログ消去
								m_pobjMailSendStatusDlg->DestroyWindow();
								delete m_pobjMailSendStatusDlg;
								m_pobjMailSendStatusDlg = NULL;
								return;
							}
						}
					}
				}
			}
		}
	}

	//状況表示
	if(m_pobjMailSendStatusDlg->IsCanceled())
	{
		if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //ダイアルアップ接続
		{
			//ＲＡＳ切断(rasDisconnect)
			objMsg.LoadString(IDS_RASDISCONNECT);
			m_pobjMailSendStatusDlg->SetStatus(objMsg);
			m_pApp->m_lpfnRasFunc5();
		}
		//メール送信状況モードレスダイアログ消去
		m_pobjMailSendStatusDlg->DestroyWindow();
		delete m_pobjMailSendStatusDlg;
		m_pobjMailSendStatusDlg = NULL;
		return;
	}

	//メール送信
	objMsg.LoadString(IDS_MAILSERVER_CONNECT);
	m_pobjMailSendStatusDlg->SetStatus(objMsg);
	if(objSmtp.Connect(objSmtpServer))
	{
		//状況表示
		objMsg.LoadString(IDS_MAIL_SEND);
		m_pobjMailSendStatusDlg->SetStatus(objMsg);
		if(m_pobjMailSendStatusDlg->IsCanceled())
		{
			if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //ダイアルアップ接続
			{
				//ＲＡＳ切断(rasDisconnect)
				objMsg.LoadString(IDS_RASDISCONNECT);
				m_pobjMailSendStatusDlg->SetStatus(objMsg);
				m_pApp->m_lpfnRasFunc5();
			}

			//切断
			objSmtp.Disconnect();

			//メール送信状況モードレスダイアログ消去
			m_pobjMailSendStatusDlg->DestroyWindow();
			delete m_pobjMailSendStatusDlg;
			m_pobjMailSendStatusDlg = NULL;
			return;
		}

		//内容を一時ファイルへ保存
		memset(szTempFileName,0,sizeof(szTempFileName));
		::GetTempFileName( m_pApp->m_strExePath,"Smtp",0,szTempFileName );
		CFile objOutFile;
		if( objOutFile.Open( szTempFileName, (CFile::modeCreate|CFile::modeWrite), NULL ) ) //OK
		{
			for(lCnt=0; lCnt<GetLineCount(); lCnt++)
			{
				pobjLine = GetOneLine(lCnt);
				if(pobjLine == NULL)
					continue;
				objOutFile.Write(pobjLine->Data(),pobjLine->GetLength());
				if(lCnt < GetLineCount()-1)
					objOutFile.Write("\r\n",2);
			}
			objOutFile.Close();
		}

		if(objSmtp.SendMail(szTempFileName))
		{
			objMsg.LoadString(IDS_MAIL_SENDEND);
			m_pobjMailSendStatusDlg->SetStatus(objMsg);

			//切断
			objSmtp.Disconnect();

			//メール送信終了通知
			objMsg.LoadString(IDS_SMTP_SENDEND);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
		}
		::DeleteFile(szTempFileName);
	}

	if( bIsDialup && bAutoDisconnect && m_pApp->m_lpfnRasFunc5) //ダイアルアップ接続
	{
		//ＲＡＳ切断(rasDisconnect)
		objMsg.LoadString(IDS_RASDISCONNECT);
		m_pobjMailSendStatusDlg->SetStatus(objMsg);
		m_pApp->m_lpfnRasFunc5();
	}

	//メール送信状況モードレスダイアログ消去
	m_pobjMailSendStatusDlg->DestroyWindow();
	delete m_pobjMailSendStatusDlg;
	m_pobjMailSendStatusDlg = NULL;
}
void CEditorDoc::OnUpdateFileSendmail(CCmdUI* pCmdUI) 
{
	//印刷中チェック
	if(GetFlagPrintPreview())
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	//ドキュメントエンプティチェック
	if(GetLineCount() == 0)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	pCmdUI->Enable(TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ツール－ソース整形」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::OnEditSourcearrange() 
{
	long lCnt=0;
	long lLine=0;
	long lOffset=0;
	long lTabSize=GetTabSize();
	long lCurrentIndent=0;
	long lMatchLength=0;
	long lOpenParenCnt=0;
	unsigned char cBuf=0, cBuf2=0;
	unsigned char c1stChar=0;
	unsigned char c2ndChar=0;
	CLine* pobjLine=NULL;
	CLine* pobjNextLine=NULL;
	CLine objLineTemp;
	CString objFindStr=_T("");
	BOOL bFind=FALSE;
	BOOL bIsInsideOfSingleQuote=FALSE;
	BOOL bIsInsideOfDoubleQuote=FALSE;
	BOOL bIsInsideOfDoubleSlash=FALSE;
	BOOL bIsInsideOfComment=FALSE;
	BOOL bIsFillTab=TRUE;				//インデントをタブで行うか？スペースで行うか？
	BOOL bIsPushOutOpenParen=TRUE;		//'{'を次の行へ追い出すか？


	bIsInsideOfComment = FALSE;
	for(lLine=0; ; lLine++)
	{
		//１行取り出し
		pobjLine = GetOneLine(lLine);
		if(pobjLine==NULL)
			break;
		if(pobjLine->GetLength() == 0)
			continue;

		//インデント
		if(!bIsInsideOfComment)
		{
			pobjLine->DelTopSpaces();
			if(lCurrentIndent > 0)
			{
				if(bIsFillTab)	//タブで充填する
					pobjLine->InsertChars(0,'\t',lCurrentIndent);
				else			//スペースで充填する
					pobjLine->InsertChars(0,' ',lCurrentIndent*lTabSize);
			}
		}

		//検索ループ
		bIsInsideOfSingleQuote = FALSE;
		bIsInsideOfDoubleSlash = FALSE;
		bIsInsideOfDoubleQuote = FALSE;
		for(lOffset=-1,lCnt=0; lCnt<pobjLine->GetLength(); )
		{
			cBuf = pobjLine->GetAt(lCnt);
			if(IsDBCSLeadByteEx(0,cBuf))
				lCnt+=2;
			else
			{
				// '\'' や '"' の中にある'{' や '}'は無視する
				// '//'のＣ＋＋コメント以降にある'{' や '}'は無視する
				// /* ～ */ のＣコメントの中にある'{' や '}'は無視する
				if(cBuf == '\'')
					bIsInsideOfSingleQuote = !bIsInsideOfSingleQuote;
				else if(cBuf == '"')
					bIsInsideOfDoubleQuote = !bIsInsideOfDoubleQuote;
				else if(cBuf == '/')
				{
					cBuf2 = pobjLine->GetAt(lCnt+1);
					if(!IsDBCSLeadByteEx(0,cBuf2) && cBuf2 == '/')
						bIsInsideOfDoubleSlash = TRUE;
					else if(!IsDBCSLeadByteEx(0,cBuf2) && cBuf2 == '*' && !bIsInsideOfComment)
						bIsInsideOfComment = TRUE;
				}
				else if(cBuf == '*')
				{
					cBuf2 = pobjLine->GetAt(lCnt+1);
					if(!IsDBCSLeadByteEx(0,cBuf2) && cBuf2 == '/' && bIsInsideOfComment)
						bIsInsideOfComment = FALSE;
				}
				else if(cBuf == '{' && 
						!bIsInsideOfSingleQuote && !bIsInsideOfDoubleQuote &&
						!bIsInsideOfDoubleSlash && !bIsInsideOfComment
				)
				{
					objFindStr = '{';
					lOffset=lCnt;
					break;
				}
				else if(cBuf == '}' && 
						!bIsInsideOfSingleQuote && !bIsInsideOfDoubleQuote &&
						!bIsInsideOfDoubleSlash && !bIsInsideOfComment
				)
				{
					objFindStr = '}';
					lOffset=lCnt;
					break;
				}
				lCnt++;
			}
		}

		if(lOffset == -1)
			continue;

		if(!objFindStr.Compare("{")) // '{' が見つかった場合
		{
			if(bIsPushOutOpenParen)//'{'を次の行へ追い出す場合
			{
				for(bFind=FALSE,lCnt=0; lCnt<lOffset; lCnt++)
				{
					cBuf = pobjLine->GetAt(lCnt);
					if(IsDBCSLeadByteEx(0,cBuf) || (!IsDBCSLeadByteEx(0,cBuf) && cBuf!=' ' && cBuf!='\t'))
					{
						bFind=TRUE;
						break;
					}
				}
				if(bFind) //'{'の前に空白文字以外がある
				{
					//'{'発見位置より後ろを保持
					pobjNextLine = new CLine();
					pobjNextLine->Set(pobjLine->Data(lOffset));
					//'{'発見位置で１行区切る
					pobjLine->SetAt(lOffset,0);
					//切り出された行を次の行とする
					InsertOneLine(lLine+1,pobjNextLine);
					continue;
				}
			}

			if((lOffset+1) < pobjLine->GetLength()) //'{'が行中にある
			{
				//'{'発見位置より後ろを保持
				pobjNextLine = new CLine();
				pobjNextLine->Set(pobjLine->Data(lOffset+1));
				//'{'発見位置の次で１行区切る
				pobjLine->SetAt(lOffset+1,0);
				//切り出された行を次の行とする
				InsertOneLine(lLine+1,pobjNextLine);
			}
			//インデント数に１プラス
			lCurrentIndent++;
			continue;
		}
		else if(!objFindStr.Compare("}")) // '}' が見つかった場合
		{
			for(bFind=FALSE,lCnt=0; lCnt<lOffset; lCnt++)
			{
				cBuf = pobjLine->GetAt(lCnt);
				if(IsDBCSLeadByteEx(0,cBuf) || (!IsDBCSLeadByteEx(0,cBuf) && cBuf!=' ' && cBuf!='\t'))
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind) //'}'の前に空白文字以外がある
			{
				//'}'発見位置より後ろを保持
				pobjNextLine = new CLine();
				pobjNextLine->Set(pobjLine->Data(lOffset));
				//'}'発見位置で１行区切る
				pobjLine->SetAt(lOffset,0);
				//切り出された行を次の行とする
				InsertOneLine(lLine+1,pobjNextLine);
				continue;
			}

			if((lOffset+1) < pobjLine->GetLength()) //'}'が行中にある
			{
				//'}'発見位置より後ろを保持
				pobjNextLine = new CLine();
				pobjNextLine->Set(pobjLine->Data(lOffset+1));
				//'}'発見位置の次で１行区切る
				pobjLine->SetAt(lOffset+1,0);
				//切り出された行を次の行とする
				InsertOneLine(lLine+1,pobjNextLine);
			}

			//タブ削除
			cBuf = pobjLine->GetAt(0);
			if(!IsDBCSLeadByteEx(0,cBuf) && cBuf == '\t')
				pobjLine->DeleteChar(0,&c1stChar,&c2ndChar);
			//インデント数から１マイナス
			lCurrentIndent--;
		}
	}

	//表示更新
	UpdateAllViews(NULL,NULL,NULL);

	//ドキュメントダーティ
	SetModifiedFlag(TRUE);

	//タイトル更新
	CString	objTitle = GetTitle();
	if(	(objTitle.GetLength() <	2) ||
		(objTitle.Right(2) != "	*")
	)
	{
		objTitle +=	" *";
		SetTitle(objTitle);
	}
}
void CEditorDoc::OnUpdateEditSourcearrange(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：現在のファイル拡張子に対応するフォント設定							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFontOfCurrentFileExt
	(	const char*	szFileExt
	)
{
	CString objKey;
	CString objFileExt = szFileExt;
	CString objMsg;

	if(objFileExt.IsEmpty())
		objKey = "FileExt_none";
	else if(!objFileExt.CompareNoCase("txt"))
		objKey = "FileExt_txt";
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
		objKey = "FileExt_htm html shtml shtm";
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
		objKey = "FileExt_c cpp";
	else if(!objFileExt.CompareNoCase("h"))
		objKey = "FileExt_h";
	else if(!objFileExt.CompareNoCase("asm"))
		objKey = "FileExt_asm";
	else if(!objFileExt.CompareNoCase("asp"))
		objKey = "FileExt_asp";
	else if(!objFileExt.CompareNoCase("frm"))
		objKey = "FileExt_frm";
	else if(!objFileExt.CompareNoCase("bas"))
		objKey = "FileExt_bas";
	else if(!objFileExt.CompareNoCase("pc"))
		objKey = "FileExt_pc";
	else if(!objFileExt.CompareNoCase("log"))
		objKey = "FileExt_log";
	else if(!objFileExt.CompareNoCase("dat"))
		objKey = "FileExt_dat";
	else if(!objFileExt.CompareNoCase("bat"))
		objKey = "FileExt_bat";
	else if(!objFileExt.CompareNoCase("ini"))
		objKey = "FileExt_ini";
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
		objKey = "FileExt_mak mk";
	else if(!objFileExt.CompareNoCase("java"))
		objKey = "FileExt_java";
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
		objKey = "FileExt_y l";
	else if(!objFileExt.CompareNoCase("wrl"))
		objKey = "FileExt_wrl";
	else
		objKey = "FileExt_other";

	objMsg.LoadString(IDS_FONTNAME_DISP);
	long lDispFontPoint     = m_pApp->GetProfileInt(objKey,"DispFontPoint",120);
	CString objDispFontName = m_pApp->GetProfileString(objKey,"DispFontName",objMsg);
	objMsg.LoadString(IDS_FONTNAME_PRINT);
	long lPrintFontPoint    = m_pApp->GetProfileInt(objKey,"PrintFontPoint",80);
	CString objPrintFontName= m_pApp->GetProfileString(objKey,"PrintFontName",objMsg);

	POSITION pos3 = GetFirstViewPosition();
	while(pos3 != 0)
	{
		CEditorView* pView = (CEditorView*)GetNextView(pos3);
		if(pView!=NULL)
		{
			//表示フォント設定
			pView->SetDispFontPoint(lDispFontPoint);
			pView->SetDispFontName(objDispFontName);
			//印字フォント設定
			pView->SetPrintFontPoint(lPrintFontPoint);
			pView->SetPrintFontName(objPrintFontName);
		}
	}

	//ビューのデフォルト値を変更
	m_pApp->WriteProfileInt("CEditorView","DispFontPoint",lDispFontPoint);
	m_pApp->WriteProfileString("CEditorView","DispFontName",objDispFontName);
	m_pApp->WriteProfileInt("CEditorView","PrintFontPoint",lPrintFontPoint);
	m_pApp->WriteProfileString("CEditorView","PrintFontName",objPrintFontName);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：現在のファイル拡張子に対応する色設定									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetColorOfCurrentFileExt
	(	const char*	szFileExt
	)
{
	char	szKeyNo1Fname[_MAX_PATH],szKeyNo2Fname[_MAX_PATH];
	char	szKeyNo3Fname[_MAX_PATH],szKeyNo4Fname[_MAX_PATH];
	CString objKey;
	CString objFileExt = szFileExt;

	if(objFileExt.IsEmpty())
	{
		objKey = "FileExt_none";
		strcpy(szKeyNo1Fname,"keyword_no1.txt");
		strcpy(szKeyNo2Fname,"keyword_no2.txt");
		strcpy(szKeyNo3Fname,"keyword_no3.txt");
		strcpy(szKeyNo4Fname,"keyword_no4.txt");
	}
	else if(!objFileExt.CompareNoCase("txt"))
	{
		objKey = "FileExt_txt";
		strcpy(szKeyNo1Fname,"keyword_no1_txt.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_txt.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_txt.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_txt.txt");
	}
	else if(!objFileExt.CompareNoCase("htm")   || !objFileExt.CompareNoCase("html") ||
			!objFileExt.CompareNoCase("shtml") || !objFileExt.CompareNoCase("shtm") )
	{
		objKey = "FileExt_htm html shtml shtm";
		strcpy(szKeyNo1Fname,"keyword_no1_htm html shtml shtm.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_htm html shtml shtm.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_htm html shtml shtm.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_htm html shtml shtm.txt");
	}
	else if(!objFileExt.CompareNoCase("c") || !objFileExt.CompareNoCase("cpp"))
	{
		objKey = "FileExt_c cpp";
		strcpy(szKeyNo1Fname,"keyword_no1_c cpp.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_c cpp.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_c cpp.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_c cpp.txt");
	}
	else if(!objFileExt.CompareNoCase("h"))
	{
		objKey = "FileExt_h";
		strcpy(szKeyNo1Fname,"keyword_no1_h.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_h.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_h.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_h.txt");
	}
	else if(!objFileExt.CompareNoCase("asm"))
	{
		objKey = "FileExt_asm";
		strcpy(szKeyNo1Fname,"keyword_no1_asm.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_asm.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_asm.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_asm.txt");
	}
	else if(!objFileExt.CompareNoCase("asp"))
	{
		objKey = "FileExt_asp";
		strcpy(szKeyNo1Fname,"keyword_no1_asp.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_asp.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_asp.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_asp.txt");
	}
	else if(!objFileExt.CompareNoCase("frm"))
	{
		objKey = "FileExt_frm";
		strcpy(szKeyNo1Fname,"keyword_no1_frm.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_frm.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_frm.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_frm.txt");
	}
	else if(!objFileExt.CompareNoCase("bas"))
	{
		objKey = "FileExt_bas";
		strcpy(szKeyNo1Fname,"keyword_no1_bas.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_bas.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_bas.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_bas.txt");
	}
	else if(!objFileExt.CompareNoCase("pc"))
	{
		objKey = "FileExt_pc";
		strcpy(szKeyNo1Fname,"keyword_no1_pc.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_pc.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_pc.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_pc.txt");
	}
	else if(!objFileExt.CompareNoCase("log"))
	{
		objKey = "FileExt_log";
		strcpy(szKeyNo1Fname,"keyword_no1_log.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_log.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_log.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_log.txt");
	}
	else if(!objFileExt.CompareNoCase("dat"))
	{
		objKey = "FileExt_dat";
		strcpy(szKeyNo1Fname,"keyword_no1_dat.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_dat.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_dat.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_dat.txt");
	}
	else if(!objFileExt.CompareNoCase("bat"))
	{
		objKey = "FileExt_bat";
		strcpy(szKeyNo1Fname,"keyword_no1_bat.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_bat.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_bat.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_bat.txt");
	}
	else if(!objFileExt.CompareNoCase("ini"))
	{
		objKey = "FileExt_ini";
		strcpy(szKeyNo1Fname,"keyword_no1_ini.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_ini.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_ini.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_ini.txt");
	}
	else if(!objFileExt.CompareNoCase("mak") || !objFileExt.CompareNoCase("mk"))
	{
		objKey = "FileExt_mak mk";
		strcpy(szKeyNo1Fname,"keyword_no1_mak mk.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_mak mk.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_mak mk.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_mak mk.txt");
	}
	else if(!objFileExt.CompareNoCase("java"))
	{
		objKey = "FileExt_java";
		strcpy(szKeyNo1Fname,"keyword_no1_java.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_java.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_java.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_java.txt");
	}
	else if(!objFileExt.CompareNoCase("y") || !objFileExt.CompareNoCase("l"))
	{
		objKey = "FileExt_y l";
		strcpy(szKeyNo1Fname,"keyword_no1_y l.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_y l.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_y l.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_y l.txt");
	}
	else if(!objFileExt.CompareNoCase("wrl"))
	{
		objKey = "FileExt_wrl";
		strcpy(szKeyNo1Fname,"keyword_no1_wrl.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_wrl.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_wrl.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_wrl.txt");
	}
	else
	{
		objKey = "FileExt_other";
		strcpy(szKeyNo1Fname,"keyword_no1_other.txt");
		strcpy(szKeyNo2Fname,"keyword_no2_other.txt");
		strcpy(szKeyNo3Fname,"keyword_no3_other.txt");
		strcpy(szKeyNo4Fname,"keyword_no4_other.txt");
	}

	COLORREF FontColor       = m_pApp->GetProfileInt(objKey,"FontColor",RGB(255,255,255));
	COLORREF BackColor       = m_pApp->GetProfileInt(objKey,"BackColor",RGB(0,0,0));
	COLORREF InvertFontColor = m_pApp->GetProfileInt(objKey,"InvertFontColor",RGB(0,0,0));
	COLORREF InvertBackColor = m_pApp->GetProfileInt(objKey,"InvertBackColor",RGB(255,255,255));
	COLORREF KeyNo1Color     = m_pApp->GetProfileInt(objKey,"KeyNo1Color",RGB(0,255,255));
	COLORREF KeyNo2Color     = m_pApp->GetProfileInt(objKey,"KeyNo2Color",RGB(0,255,255));
	COLORREF KeyNo3Color     = m_pApp->GetProfileInt(objKey,"KeyNo3Color",RGB(0,255,255));
	COLORREF KeyNo4Color     = m_pApp->GetProfileInt(objKey,"KeyNo4Color",RGB(0,255,255));
	COLORREF MarkLineColor   = m_pApp->GetProfileInt(objKey,"MarkLineColor",RGB(128,0,128));
	BOOL     bComment1Clr    = m_pApp->GetProfileInt(objKey,"bComment1Clr",1);
	COLORREF CommentColor    = m_pApp->GetProfileInt(objKey,"CommentColor",RGB(0,255,0));
	BOOL     bComment2Clr    = m_pApp->GetProfileInt(objKey,"bComment2Clr",0);
	COLORREF CommentColor2   = m_pApp->GetProfileInt(objKey,"CommentColor2",RGB(0,255,0));
	BOOL     bComment3Clr    = m_pApp->GetProfileInt(objKey,"bComment3Clr",0);
	COLORREF CommentColor3   = m_pApp->GetProfileInt(objKey,"CommentColor3",RGB(0,255,0));
	BOOL     bComment4Clr    = m_pApp->GetProfileInt(objKey,"bComment4Clr",0);
	COLORREF CommentColor4   = m_pApp->GetProfileInt(objKey,"CommentColor4",RGB(0,255,0));
	BOOL     bDispLineNo     = m_pApp->GetProfileInt(objKey,"bDispLineNo",1);
	COLORREF LineNoColor     = m_pApp->GetProfileInt(objKey,"LineNoColor",RGB(0,255,255));
	COLORREF LineNoBackColor = m_pApp->GetProfileInt(objKey,"LineNoBackColor",RGB(0,0,0));
	BOOL     bDispRuler	     = m_pApp->GetProfileInt(objKey,"bDispRuler",1);
	COLORREF RulerColor	     = m_pApp->GetProfileInt(objKey,"RulerColor",RGB(0,0,0));
	COLORREF RulerBackColor  = m_pApp->GetProfileInt(objKey,"RulerBackColor",RGB(192,192,192));
	BOOL     bDispCRLF		 = m_pApp->GetProfileInt(objKey,"bDispCRLF",1);
	COLORREF CRLFColor		 = m_pApp->GetProfileInt(objKey,"CRLFColor",RGB(255,255,0));
	BOOL     bDispTab		 = m_pApp->GetProfileInt(objKey,"bDispTab",0);
	COLORREF TabColor		 = m_pApp->GetProfileInt(objKey,"TabColor",RGB(255,255,255));
	BOOL     bDispZenkakuSpace = m_pApp->GetProfileInt(objKey,"bDispZenkakuSpace",0);
	COLORREF ZenkakuSpaceColor = m_pApp->GetProfileInt(objKey,"ZenkakuSpaceColor",RGB(255,255,255));
	BOOL     bDispUnderLine  = m_pApp->GetProfileInt(objKey,"bDispUnderLine",1);
	COLORREF UnderLineColor  = m_pApp->GetProfileInt(objKey,"UnderLineColor",RGB(255,255,0));
	BOOL     bDispUrl		 = m_pApp->GetProfileInt(objKey,"bDispUrl",1);
	COLORREF UrlColor		 = m_pApp->GetProfileInt(objKey,"UrlColor",RGB(255,0,255));
	BOOL     bDispEMail	     = m_pApp->GetProfileInt(objKey,"bDispEMail",1);
	COLORREF EMailColor	     = m_pApp->GetProfileInt(objKey,"EMailColor",RGB(255,0,255));

	SetFontColor(FontColor);
	DeleteAllKeyWord();
	SetKeyNo1Color(KeyNo1Color,szKeyNo1Fname);
	SetKeyNo2Color(KeyNo2Color,szKeyNo2Fname);
	SetKeyNo3Color(KeyNo3Color,szKeyNo3Fname);
	SetKeyNo4Color(KeyNo4Color,szKeyNo4Fname);
	SetFlagComment1Clr(bComment1Clr);
	SetCommentColor(CommentColor);
	SetFlagComment2Clr(bComment2Clr);
	SetCommentColor2(CommentColor2);
	SetFlagComment3Clr(bComment3Clr);
	SetCommentColor3(CommentColor3);
	SetFlagComment4Clr(bComment4Clr);
	SetCommentColor4(CommentColor4);
	SetFlagUrlColor(bDispUrl);
	SetUrlColor(UrlColor);
	SetFlagEMailColor(bDispEMail);
	SetEMailColor(EMailColor);
	POSITION pos = GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)GetNextView(pos);
		if(pView!=NULL)
		{
			pView->SetFontColor(FontColor);
			pView->SetBackColor(BackColor);
			pView->SetInvertFontColor(InvertFontColor);
			pView->SetInvertBackColor(InvertBackColor);
			pView->SetMarkLineColor(MarkLineColor);
			pView->SetFlagDispLineNo(bDispLineNo);
			pView->SetLineNoColor(LineNoColor);
			pView->SetLineNoBackColor(LineNoBackColor);
			pView->SetFlagDispRuler(bDispRuler);
			pView->SetRulerColor(RulerColor);
			pView->SetRulerBackColor(RulerBackColor);
			pView->SetFlagDispCRLF(bDispCRLF);
			pView->SetCRLFColor(CRLFColor);
			pView->SetFlagDispTab(bDispTab);
			pView->SetTabColor(TabColor);
			pView->SetFlagDispZenkakuSpace(bDispZenkakuSpace);
			pView->SetZenkakuSpaceColor(ZenkakuSpaceColor);
			pView->SetFlagDispUnderLine(bDispUnderLine);
			pView->SetUnderLineColor(UnderLineColor);
		}
	}

	//ビューのデフォルト値を変更
	m_pApp->WriteProfileInt("CEditorView","FontColor",FontColor);
	m_pApp->WriteProfileInt("CEditorView","BackColor",BackColor);
	m_pApp->WriteProfileInt("CEditorView","InvertFontColor",InvertFontColor);
	m_pApp->WriteProfileInt("CEditorView","InvertBackColor",InvertBackColor);
	m_pApp->WriteProfileInt("CEditorView","KeyNo1Color",KeyNo1Color);
	m_pApp->WriteProfileInt("CEditorView","KeyNo2Color",KeyNo2Color);
	m_pApp->WriteProfileInt("CEditorView","KeyNo3Color",KeyNo3Color);
	m_pApp->WriteProfileInt("CEditorView","KeyNo4Color",KeyNo4Color);
	m_pApp->WriteProfileInt("CEditorView","MarkLineColor",MarkLineColor);
	m_pApp->WriteProfileInt("CEditorView","bComment1Clr",bComment1Clr);
	m_pApp->WriteProfileInt("CEditorView","CommentColor",CommentColor);
	m_pApp->WriteProfileInt("CEditorView","bComment2Clr",bComment2Clr);
	m_pApp->WriteProfileInt("CEditorView","CommentColor2",CommentColor2);
	m_pApp->WriteProfileInt("CEditorView","bComment3Clr",bComment3Clr);
	m_pApp->WriteProfileInt("CEditorView","CommentColor3",CommentColor3);
	m_pApp->WriteProfileInt("CEditorView","bComment4Clr",bComment4Clr);
	m_pApp->WriteProfileInt("CEditorView","CommentColor4",CommentColor4);
	m_pApp->WriteProfileInt("CEditorView","bDispLineNo",bDispLineNo);
	m_pApp->WriteProfileInt("CEditorView","LineNoColor",LineNoColor);
	m_pApp->WriteProfileInt("CEditorView","LineNoBackColor",LineNoBackColor);
	m_pApp->WriteProfileInt("CEditorView","bDispRuler",bDispRuler);
	m_pApp->WriteProfileInt("CEditorView","RulerColor",RulerColor);
	m_pApp->WriteProfileInt("CEditorView","RulerBackColor",RulerBackColor);
	m_pApp->WriteProfileInt("CEditorView","bDispCRLF",bDispCRLF);
	m_pApp->WriteProfileInt("CEditorView","CRLFColor",CRLFColor);
	m_pApp->WriteProfileInt("CEditorView","bDispTab",bDispTab);
	m_pApp->WriteProfileInt("CEditorView","TabColor",TabColor);
	m_pApp->WriteProfileInt("CEditorView","bDispZenkakuSpace",bDispZenkakuSpace);
	m_pApp->WriteProfileInt("CEditorView","ZenkakuSpaceColor",ZenkakuSpaceColor);
	m_pApp->WriteProfileInt("CEditorView","bDispUnderLine",bDispUnderLine);
	m_pApp->WriteProfileInt("CEditorView","UnderLineColor",UnderLineColor);
	m_pApp->WriteProfileInt("CEditorView","bDispUrl",bDispUrl);
	m_pApp->WriteProfileInt("CEditorView","UrlColor",UrlColor);
	m_pApp->WriteProfileInt("CEditorView","bDispEMail",bDispEMail);
	m_pApp->WriteProfileInt("CEditorView","EMailColor",EMailColor);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＲＬ上チェック処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsAboveURL
	(	long	lLine,
		long	lOffset
	)
{
	CLine* pobjLine = m_objLineManager.GetOneLine(lLine);
	if(pobjLine == NULL)
		return FALSE;

	return pobjLine->IsAboveURL(lOffset);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｅ－ｍａｉｌ上チェック処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsAboveEMail
	(	long	lLine,
		long	lOffset
	)
{
	CLine* pobjLine = m_objLineManager.GetOneLine(lLine);
	if(pobjLine == NULL)
		return FALSE;

	return pobjLine->IsAboveEMail(lOffset);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷プレビューフラグ取得												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagPrintPreview()
{
	return m_bIsPrintPreview;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：印刷プレビューフラグ設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagPrintPreview
	(	BOOL	bIsPrintPreview
	)
{
	m_bIsPrintPreview = bIsPrintPreview;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：バイト数取得処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::GetBytes
	(	long	lLimitLine,				//指定行
		long*	plLimitBytes,			//指定行までのバイト数返却用バッファ
		long*	plSumBytes				//総バイト数返却用バッファ
	)
{
	m_objLineManager.GetBytes(lLimitLine,plLimitBytes,plSumBytes);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル内容再読み込み必要性チェック処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsNeedReload()			//TRUE : 必要あり、FALSE : 必要なし
{
	if(m_pobjLastWrittenTime==NULL)
		return FALSE;
	if(	m_strFilePath.GetLength()==0 )
		return FALSE;

	//ファイル最終更新日時取得
	CFileStatus		objFileStatus;
	CFile::GetStatus(m_strFilePath,objFileStatus);
	CTimeSpan	objSpan(0,0,0,5);
	CTimeSpan	objDiff	 = objFileStatus.m_mtime - (*m_pobjLastWrittenTime);
	CTimeSpan	objDiff2 = (*m_pobjLastWrittenTime) - objFileStatus.m_mtime;
	if(	objDiff>objSpan	|| objDiff2>objSpan )
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル内容再読み込み処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ReloadDocument(BOOL bIsConfirm)
{
	CString objMsg;

	//ファイル名チェック
	if(	m_strFilePath.GetLength()==0 )
		return;

	//ファイルオープン
	CFile		objFile;
	if(	!objFile.Open( m_strFilePath, CFile::modeRead, NULL ) )
		return;

	if(	bIsConfirm )
	{
		//再読み込み確認
		char	szMsg[1024];
		if(IsModified())
		{
			objMsg.LoadString(IDS_CONFIRM_REREAD1);
			sprintf(szMsg,(LPCTSTR)objMsg,m_strFilePath);
			int	nRet = m_pApp->MyMessageBox(szMsg,(MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2),0);
			if(nRet==IDNO)
			{
				objFile.Close();
				return;
			}
		}
		else
		{
			objMsg.LoadString(IDS_CONFIRM_REREAD2);
			sprintf(szMsg,(LPCTSTR)objMsg,m_strFilePath);
			int	nRet = m_pApp->MyMessageBox(szMsg,(MB_YESNO|MB_ICONQUESTION),0);
			if(nRet==IDNO)
			{
				objFile.Close();
				return;
			}
		}
	}

	//アーカイブに連結
	CArchive	ar(&objFile,CArchive::load);

	//メンバ初期化
	m_dwFileAttributes = 0;
	m_bGrepMode	= 0;
	m_objLineManager.Initialize();
	m_objUndoManager.Initialize();

	//ファイル内容読み込み
	Serialize(ar);

	//キャレット位置を初期位置へ
	POSITION pos = GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)GetNextView(pos);
		pView->MySetCaretPos(CPoint(pView->m_LineNoPos.x,pView->m_RulerPos.y));
		m_pApp->MessagePump();
	}

	//クローズ
	ar.Close();
	objFile.Close();

	//ファイル最終更新日時取得
	if(m_pobjLastWrittenTime != NULL)
		delete m_pobjLastWrittenTime;
	m_pobjLastWrittenTime = new CTime();
	CFileStatus		objFileStatus;
	CFile::GetStatus(m_strFilePath,objFileStatus);
	(*m_pobjLastWrittenTime) = objFileStatus.m_mtime;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル名取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::MyGetFileName
	(	char*	pPtr
	)
{
	strcpy(pPtr,m_strFileName);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイルパス取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::MyGetFilePath
	(	char*	pPtr
	)
{
	strcpy(pPtr,m_strFilePath);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル拡張子取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CString CEditorDoc::MyGetFileExt()
{
	CString objStr = m_strFileExt;
	return objStr;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：読み込み専用調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsReadOnly()
{
	if(	(m_dwFileAttributes	& FILE_ATTRIBUTE_READONLY) != 0 )
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：読み込み専用設定														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::SetReadOnly()
{
	BOOL	bRet=FALSE;
	DWORD	dwOrg =	m_dwFileAttributes;
	CString objMsg;

	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
	if(	m_strFilePath.GetLength() )
	{
		bRet = SetFileAttributes(m_strFilePath,m_dwFileAttributes);
		if(bRet	== FALSE)
		{
			objMsg.LoadString(IDS_ERRCHANGEFILEATTR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			m_dwFileAttributes = dwOrg;
			return FALSE;
		}
	}

	//Undo,Redo	管理クラス初期化
	m_objUndoManager.Initialize();
	//ドキュメントクリーン
	SetModifiedFlag(FALSE);

	//タイトル更新
	CString	objTitle = GetTitle();
	if(	(objTitle.GetLength() >	2) &&
		(objTitle.Right(2) == "	*")
	)
	{
		objTitle = objTitle.Left(objTitle.GetLength() -	2);
		SetTitle(objTitle);
	}

	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：読み込み専用設定														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::SetReadOnly2()
{
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：読み込み専用解除														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::UnsetReadOnly()
{
	BOOL	bRet=FALSE;
	DWORD	dwOrg =	m_dwFileAttributes;
	CString objMsg;

	m_dwFileAttributes &= ~(FILE_ATTRIBUTE_READONLY);
	if(	m_strFilePath.GetLength() )
	{
		bRet = SetFileAttributes(m_strFilePath,m_dwFileAttributes);
		if(bRet	== FALSE)
		{
			objMsg.LoadString(IDS_ERRCHANGEFILEATTR);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			m_dwFileAttributes = dwOrg;
			return FALSE;
		}
	}
	return TRUE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：空ドキュメント調査													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsEmpty()
{
	if(	GetLineCount() == 0 )
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＮＤＯ可能調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsCanUndo()
{
	if(m_objUndoManager.IsCanUndo())
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＲＥＤＯ可能調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsCanRedo()
{
	if(m_objUndoManager.IsCanRedo())
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行数取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetLineCount()
{
	return m_objLineManager.GetLineCount();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行取得処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine* CEditorDoc::GetOneLine
	(	long	lLine			//行番号
	)
{
	return m_objLineManager.GetOneLine(lLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行の長さの最大値を取得処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetMaxLineLength()
{
	return m_objLineManager.GetMaxLineLength();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定行の文字列長取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetLineLength(long	lLineNo)
{
	return m_objLineManager.GetLineLength(lLineNo);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：２バイト文字の第一バイトチェック処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::MyIsDBCSLeadByteEx	//TRUE:２バイト文字の第一バイト,FALSE:その他
	(	ULONG	lLineNo,	//行番号
		ULONG	lOffset		//行頭からのオフセット
	)
{
	if(lOffset < 0)
		return FALSE;

	CLine*	pobjLine = m_objLineManager.GetOneLine(lLineNo);
	if(pobjLine==NULL)//なかった
		return FALSE;
	if(lOffset >= (ULONG)pobjLine->GetLength())
		return FALSE;

	BYTE c = (BYTE)pobjLine->GetAt(lOffset);
	BOOL bRet =	IsDBCSLeadByteEx(0,c);
	return bRet;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブコードチェック処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsTabCode //TRUE:タブコード,FALSE:その他
	(	ULONG	lLineNo,	//行番号
		ULONG	lOffset		//行頭からのオフセット
	)
{
	if(lOffset < 0)
		return FALSE;

	CLine*	pobjLine = m_objLineManager.GetOneLine(lLineNo);
	if(pobjLine==NULL)//なかった
		return FALSE;
	if(lOffset >= (ULONG)pobjLine->GetLength())
		return FALSE;

	UCHAR c	= pobjLine->GetAt(lOffset);
	if(c ==	'\t')
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行に含まれるタブの数取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetTabCount
	(	long	lLineNo		//行番号
	)
{
	CLine*	pobjLine = m_objLineManager.GetOneLine(lLineNo);
	if(pobjLine==NULL)//なかった
		return 0;

	long lTabCount=0;
	for(long lCnt=0; lCnt<pobjLine->GetLength(); )
	{
		UCHAR c	= pobjLine->GetAt(lCnt);
		if(IsDBCSLeadByteEx(0,c))
		{
			lCnt+=2;
			continue;
		}
		if(c ==	'\t')
			lTabCount++;
		lCnt++;
	}
	return lTabCount;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字登録処理															|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AddCharacter
	(	long	lCurrentLine,			// 文字登録対象の行番号
		long	lCurrentOffset,			// 文字登録対象の列番号（ﾊﾞｲﾄ単位）
		UINT	n1stChar,				// 登録文字第一ﾊﾞｲﾄ
		UINT	n2ndChar,				// 登録文字第二ﾊﾞｲﾄ
		BOOL	bInsertKey,				// 挿入／上書きﾓｰﾄﾞﾌﾗｸﾞ
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（文字入力）生成
	CUndoItemAddCharacter* pobjUndoItemAddCharacter	= new CUndoItemAddCharacter(this);

	//処理実行
	pobjUndoItemAddCharacter->Do( lCurrentLine,
								  lCurrentOffset,
								  n1stChar,
								  n2ndChar,
								  bInsertKey,
								  objCaretPos,
								  objScrollPosLog,
								  bDispRuler,
								  bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemAddCharacter);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行コード処理														|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessReturnCode
	(	long	lCurrentLine,			// 文字登録対象の行番号
		long	lCurrentOffset,			// 文字登録対象の列番号（ﾊﾞｲﾄ単位）
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（改行）生成
	CUndoItemReturn* pobjUndoItemReturn	= new CUndoItemReturn(this);

	//処理実行
	pobjUndoItemReturn->Do(	lCurrentLine,
							lCurrentOffset,
							objCaretPos,
							objScrollPosLog,
							bDispRuler,
							bDispLineNo	);

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemReturn);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：DELETEコード処理														|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDeleteCode
	(	long	lCurrentLine,			// 文字登録対象の行番号
		long	lCurrentOffset,			// 文字登録対象の列番号（ﾊﾞｲﾄ単位）
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（文字DELETE）生成
	CUndoItemDelChar* pobjUndoItemDelChar =	new	CUndoItemDelChar(this);

	//処理実行
	pobjUndoItemDelChar->Do( lCurrentLine,
							 lCurrentOffset,
							 objCaretPos,
							 objScrollPosLog,
							 bDispRuler,
							 bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelChar);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：BACK SPACEコード処理													|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessBackSpaceCode
	(	long	lCurrentLine,			// 文字登録対象の行番号
		long	lCurrentOffset,			// 文字登録対象の列番号（ﾊﾞｲﾄ単位）
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（文字BACK SPACE）生成
	CUndoItemBackSpaceChar*	pobjUndoItemBackSpaceChar =	new	CUndoItemBackSpaceChar(this);

	//処理実行
	pobjUndoItemBackSpaceChar->Do( lCurrentLine,
								   lCurrentOffset,
								   objCaretPos,
								   objScrollPosLog,
								   bDispRuler,
								   bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemBackSpaceChar);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択範囲テキスト削除処理												|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::DeleteSelectedArea
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		BOOL	bToClipBoard,			// クリップボードへコピーするか？(TRUE:する、FALSE:しない）
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（選択範囲消去）生成
	CUndoItemCutSelectArea*	pobjUndoItemCutSelectArea =	new	CUndoItemCutSelectArea(this);

	//処理実行
	pobjUndoItemCutSelectArea->Do(lSelectStartLine,
								  lSelectStartOffset,
								  lSelectEndLine,
								  lSelectEndOffset,
								  bToClipBoard,
								  objCaretPos,
								  objScrollPosLog,
								  bDispRuler,
								  bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemCutSelectArea);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：張り付け処理															|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessPaste
	(	long	lCurrentLine,			// キャレット位置の行番号
		long	lCurrentOffset,			// キャレット位置の列番号
		long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（張り付け処理）生成
	CUndoItemEditPaste*	pobjUndoItemEditPaste =	new	CUndoItemEditPaste(this);

	//処理実行
	pobjUndoItemEditPaste->Do(	lCurrentLine,
								lCurrentOffset,
								lSelectStartLine,
								lSelectStartOffset,
								lSelectEndLine,
								lSelectEndOffset,
								objCaretPos,
								objScrollPosLog,
								bDispRuler,
								bDispLineNo	);

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemEditPaste);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＯＬＥドロップ処理													|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessOleDrop
	(	COleDataObject*	pobjOleDataObject,		// ＯＬＥデータオブジェクト
		long	lCurrentLine,					// キャレット位置の行番号
		long	lCurrentOffset,					// キャレット位置の列番号
		CPoint&	objCaretPos,					// キャレット位置
		CPoint&	objScrollPosLog,				// スクロール位置
		BOOL	bDispRuler,						// ルーラ表示フラグ
		BOOL	bDispLineNo						// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（ＯＬＥドロップ処理）生成
	CUndoItemOleDrop* pobjUndoItemOleDrop =	new	CUndoItemOleDrop(this);

	//処理実行
	pobjUndoItemOleDrop->Do(	pobjOleDataObject,
								lCurrentLine,
								lCurrentOffset,
								objCaretPos,
								objScrollPosLog,
								bDispRuler,
								bDispLineNo	);

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemOleDrop);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択単語置換処理														|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessReplaceSelectWord
	(	CString& objReplaceStr,			// 置換後文字列
		long	lCurrentLine,			// キャレット位置の行番号
		long	lCurrentOffset,			// キャレット位置の列番号
		long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（置換処理）生成
	CUndoItemReplaceSelectWord*	pobjUndoItemReplaceSelectWord =	new	CUndoItemReplaceSelectWord(this);

	//処理実行
	pobjUndoItemReplaceSelectWord->Do(	objReplaceStr,
										lCurrentLine,
										lCurrentOffset,
										lSelectStartLine,
										lSelectStartOffset,
										lSelectEndLine,
										lSelectEndOffset,
										objCaretPos,
										objScrollPosLog,
										bDispRuler,
										bDispLineNo	);

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemReplaceSelectWord);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択している状態でのＴＡＢキー操作処理							|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessTabWhenSelected
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（範囲選択時のタブコード処理）生成
	CUndoItemTabWhenSelected* pobjUndoItemTabWhenSelected =	new	CUndoItemTabWhenSelected(this);

	//処理実行
	pobjUndoItemTabWhenSelected->Do( lSelectStartLine,
									 lSelectStartOffset,
									 lSelectEndLine,
									 lSelectEndOffset,
									 objCaretPos,
									 objScrollPosLog,
									 bDispRuler,
									 bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemTabWhenSelected);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択している状態でのＳｈｉｆｔ＋ＴＡＢキー操作処理				|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessShiftTabWhenSelected
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（範囲選択時のシフトタブコード処理）生成
	CUndoItemShiftTabWhenSelected* pobjUndoItemShiftTabWhenSelected	= new CUndoItemShiftTabWhenSelected(this);

	//処理実行
	pobjUndoItemShiftTabWhenSelected->Do( lSelectStartLine,
										 lSelectStartOffset,
										 lSelectEndLine,
										 lSelectEndOffset,
										 objCaretPos,
										 objScrollPosLog,
										 bDispRuler,
										 bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemShiftTabWhenSelected);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択している領域の先頭空白削除処理								|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDelTopSpaces
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス生成
	CUndoItemDelTopSpaces* pobjUndoItemDelTopSpaces	= new CUndoItemDelTopSpaces(this);

	//処理実行
	pobjUndoItemDelTopSpaces->Do( lSelectStartLine,
								 lSelectStartOffset,
								 lSelectEndLine,
								 lSelectEndOffset,
								 objCaretPos,
								 objScrollPosLog,
								 bDispRuler,
								 bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelTopSpaces);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブ－＞半角スペース変換処理											|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessTab2Space
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo,			// 行番号表示フラグ
		long	lfWidth					// フォント幅
	)
{
	//取り消し可能操作クラス（タブ－＞半角スペース処理）生成
	CUndoItemTab2Space*	pobjUndoItemTab2Space =	new	CUndoItemTab2Space(this);

	//処理実行
	pobjUndoItemTab2Space->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemTab2Space);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：半角スペース－＞タブ変換処理											|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessSpace2Tab
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo,			// 行番号表示フラグ
		long	lfWidth					// フォント幅
	)
{
	//取り消し可能操作クラス（半角スペース－＞タブ処理）生成
	CUndoItemSpace2Tab*	pobjUndoItemSpace2Tab =	new	CUndoItemSpace2Tab(this);

	//処理実行
	pobjUndoItemSpace2Tab->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemSpace2Tab);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角スペース－＞半角スペース変換処理									|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessSpaceZen2Han
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo,			// 行番号表示フラグ
		long	lfWidth					// フォント幅
	)
{
	//取り消し可能操作クラス（全角スペース－＞半角スペース処理）生成
	CUndoItemSpaceZen2Han* pobjUndoItemSpaceZen2Han	= new CUndoItemSpaceZen2Han(this);

	//処理実行
	pobjUndoItemSpaceZen2Han->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemSpaceZen2Han);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：半角スペース－＞全角スペース変換処理									|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessSpaceHan2Zen
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo,			// 行番号表示フラグ
		long	lfWidth					// フォント幅
	)
{
	//取り消し可能操作クラス（半角スペース－＞全角スペース処理）生成
	CUndoItemSpaceHan2Zen* pobjUndoItemSpaceHan2Zen	= new CUndoItemSpaceHan2Zen(this);

	//処理実行
	pobjUndoItemSpaceHan2Zen->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemSpaceHan2Zen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：半角ｶﾀｶﾅ－＞全角カタカナ変換処理										|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessKatakanaHan2Zen
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo,			// 行番号表示フラグ
		long	lfWidth					// フォント幅
	)
{
	//取り消し可能操作クラス（半角ｶﾀｶﾅ－＞全角カタカナ処理）生成
	CUndoItemKatakanaHan2Zen* pobjUndoItemKatakanaHan2Zen =	new	CUndoItemKatakanaHan2Zen(this);

	//処理実行
	pobjUndoItemKatakanaHan2Zen->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemKatakanaHan2Zen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角カタカナ－＞半角ｶﾀｶﾅ変換処理										|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessKatakanaZen2Han
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo,			// 行番号表示フラグ
		long	lfWidth					// フォント幅
	)
{
	//取り消し可能操作クラス（全角カタカナ－＞半角ｶﾀｶﾅ処理）生成
	CUndoItemKatakanaZen2Han* pobjUndoItemKatakanaZen2Han =	new	CUndoItemKatakanaZen2Han(this);

	//処理実行
	pobjUndoItemKatakanaZen2Han->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemKatakanaZen2Han);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：半角英数字－＞全角英数字変換処理										|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessAlnumHan2Zen
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo,			// 行番号表示フラグ
		long	lfWidth					// フォント幅
	)
{
	//取り消し可能操作クラス（半角英数字－＞全角英数字処理）生成
	CUndoItemAlnumHan2Zen* pobjUndoItemAlnumHan2Zen	= new CUndoItemAlnumHan2Zen(this);

	//処理実行
	pobjUndoItemAlnumHan2Zen->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemAlnumHan2Zen);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全角英数字－＞半角英数字変換処理										|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessAlnumZen2Han
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo,			// 行番号表示フラグ
		long	lfWidth					// フォント幅
	)
{
	//取り消し可能操作クラス（全角英数字－＞半角英数字処理）生成
	CUndoItemAlnumZen2Han* pobjUndoItemAlnumZen2Han	= new CUndoItemAlnumZen2Han(this);

	//処理実行
	pobjUndoItemAlnumZen2Han->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo,
							   lfWidth );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemAlnumZen2Han);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択領域へのＣ／Ｃ＋＋コメント挿入処理							|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsCppComment
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（範囲選択領域へのＣ／Ｃ＋＋コメント挿入処理）生成
	CUndoItemInsCppComment*	pobjUndoItemInsCppComment =	new	CUndoItemInsCppComment(this);

	//処理実行
	pobjUndoItemInsCppComment->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsCppComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択領域のＣ／Ｃ＋＋コメント削除処理								|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDelCppComment
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（範囲選択領域のＣ／Ｃ＋＋コメント削除処理）生成
	CUndoItemDelCppComment*	pobjUndoItemDelCppComment =	new	CUndoItemDelCppComment(this);

	//処理実行
	pobjUndoItemDelCppComment->Do( lSelectStartLine,
								   lSelectStartOffset,
								   lSelectEndLine,
								   lSelectEndOffset,
								   objCaretPos,
								   objScrollPosLog,
								   bDispRuler,
								   bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelCppComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択領域へのＶＢコメント挿入処理									|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsVbComment
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（範囲選択領域へのＶＢコメント挿入処理）生成
	CUndoItemInsVbComment* pobjUndoItemInsVbComment	= new CUndoItemInsVbComment(this);

	//処理実行
	pobjUndoItemInsVbComment->Do( lSelectStartLine,
							   lSelectStartOffset,
							   lSelectEndLine,
							   lSelectEndOffset,
							   objCaretPos,
							   objScrollPosLog,
							   bDispRuler,
							   bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsVbComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択領域のＶＢコメント削除処理									|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDelVbComment
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（範囲選択領域のＶＢコメント削除処理）生成
	CUndoItemDelVbComment* pobjUndoItemDelVbComment	= new CUndoItemDelVbComment(this);

	//処理実行
	pobjUndoItemDelVbComment->Do( lSelectStartLine,
								  lSelectStartOffset,
								  lSelectEndLine,
								  lSelectEndOffset,
								  objCaretPos,
								  objScrollPosLog,
								  bDispRuler,
								  bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelVbComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択領域へのアセンブラコメント挿入処理							|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsAsmComment
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（範囲選択領域へのアセンブラコメント挿入処理）生成
	CUndoItemInsAsmComment*	pobjUndoItemInsAsmComment =	new	CUndoItemInsAsmComment(this);

	//処理実行
	pobjUndoItemInsAsmComment->Do( lSelectStartLine,
								   lSelectStartOffset,
								   lSelectEndLine,
								   lSelectEndOffset,
								   objCaretPos,
								   objScrollPosLog,
								   bDispRuler,
								   bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsAsmComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択領域のアセンブラコメント削除処理								|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDelAsmComment
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（範囲選択領域のアセンブラコメント削除処理）生成
	CUndoItemDelAsmComment*	pobjUndoItemDelAsmComment =	new	CUndoItemDelAsmComment(this);

	//処理実行
	pobjUndoItemDelAsmComment->Do( lSelectStartLine,
								   lSelectStartOffset,
								   lSelectEndLine,
								   lSelectEndOffset,
								   objCaretPos,
								   objScrollPosLog,
								   bDispRuler,
								   bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelAsmComment);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択領域への引用符号挿入処理										|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsQuote
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（範囲選択領域へのメール引用符号挿入処理）生成
	CUndoItemInsQuote*	pobjUndoItemInsQuote = new CUndoItemInsQuote(this);

	//処理実行
	pobjUndoItemInsQuote->Do( lSelectStartLine,
							  lSelectStartOffset,
							  lSelectEndLine,
							  lSelectEndOffset,
							  objCaretPos,
							  objScrollPosLog,
							  bDispRuler,
							  bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsQuote);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：範囲選択領域の引用符号削除処理										|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessDelQuote
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（範囲選択領域のメール引用符号削除処理）生成
	CUndoItemDelQuote*	pobjUndoItemDelQuote = new CUndoItemDelQuote(this);

	//処理実行
	pobjUndoItemDelQuote->Do( lSelectStartLine,
							  lSelectStartOffset,
							  lSelectEndLine,
							  lSelectEndOffset,
							  objCaretPos,
							  objScrollPosLog,
							  bDispRuler,
							  bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemDelQuote);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：日付文字列挿入処理													|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsDay
	(	long		lCurrentLine,			// キャレット位置行
		long		lCurrentOffset,			// キャレット位置オフセット
		CString&	objDayStr,				// 日付文字列
		CPoint&		objCaretPos,			// キャレット位置
		CPoint&		objScrollPosLog,		// スクロール位置
		BOOL		bDispRuler,				// ルーラ表示フラグ
		BOOL		bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（日付文字列挿入処理）生成
	CUndoItemInsDay* pobjUndoItemInsDay	= new CUndoItemInsDay(this);

	//処理実行
	pobjUndoItemInsDay->Do(	lCurrentLine,
							lCurrentOffset,
							objDayStr,
							objCaretPos,
							objScrollPosLog,
							bDispRuler,
							bDispLineNo	);

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsDay);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：時刻文字列挿入処理													|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessInsTime
	(	long		lCurrentLine,			// キャレット位置行
		long		lCurrentOffset,			// キャレット位置オフセット
		CString&	objTimeStr,				// 時刻文字列
		CPoint&		objCaretPos,			// キャレット位置
		CPoint&		objScrollPosLog,		// スクロール位置
		BOOL		bDispRuler,				// ルーラ表示フラグ
		BOOL		bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（時刻文字列挿入処理）生成
	CUndoItemInsTime* pobjUndoItemInsTime =	new	CUndoItemInsTime(this);

	//処理実行
	pobjUndoItemInsTime->Do( lCurrentLine,
							lCurrentOffset,
							objTimeStr,
							objCaretPos,
							objScrollPosLog,
							bDispRuler,
							bDispLineNo	);

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemInsTime);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：英字－＞大文字変換処理												|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessAl2Upper
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（英字－＞大文字変換処理）生成
	CUndoItemAl2Upper* pobjUndoItemAl2Upper	= new CUndoItemAl2Upper(this);

	//処理実行
	pobjUndoItemAl2Upper->Do( lSelectStartLine,
							  lSelectStartOffset,
							  lSelectEndLine,
							  lSelectEndOffset,
							  objCaretPos,
							  objScrollPosLog,
							  bDispRuler,
							  bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemAl2Upper);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：英字－＞小文字変換処理												|*/
/*|	解説	：																		|*/
/*|	備考	：デザインパターンでのＣｏｍｍａｎｄパターンを使用						|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::ProcessAl2Lower
	(	long	lSelectStartLine,		// 選択開始行番号
		long	lSelectStartOffset,		// 選択開始列番号
		long	lSelectEndLine,			// 選択終了行番号
		long	lSelectEndOffset,		// 選択終了列番号
		CPoint&	objCaretPos,			// キャレット位置
		CPoint&	objScrollPosLog,		// スクロール位置
		BOOL	bDispRuler,				// ルーラ表示フラグ
		BOOL	bDispLineNo				// 行番号表示フラグ
	)
{
	//取り消し可能操作クラス（英字－＞小文字変換処理）生成
	CUndoItemAl2Lower* pobjUndoItemAl2Lower	= new CUndoItemAl2Lower(this);

	//処理実行
	pobjUndoItemAl2Lower->Do( lSelectStartLine,
							  lSelectStartOffset,
							  lSelectEndLine,
							  lSelectEndOffset,
							  objCaretPos,
							  objScrollPosLog,
							  bDispRuler,
							  bDispLineNo );

	//取り消し可能操作管理クラスへ登録
	m_objUndoManager.addUndoItem((CUndoItem*)pobjUndoItemAl2Lower);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブサイズ取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetTabSize()
{
	return m_objLineManager.GetTabSize();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：タブサイズ設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetTabSize(long lTabSize)
{
	m_objLineManager.SetTabSize(lTabSize);
	//ビューへ更新通知
	UpdateAllViews(NULL,NULL,NULL);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行コード区分取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetKaigyoKind()
{
	return m_objLineManager.GetKaigyoKind();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：改行コード区分設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetKaigyoKind
	(	long	lKaigyoKind
	)
{
	if(lKaigyoKind == m_objLineManager.GetKaigyoKind())	//変更なし
		return;

	m_objLineManager.SetKaigyoKind(lKaigyoKind);
	if(	!IsGrep()   && !IsDump()    && !IsConv()	 && 
		!IsWCount() && !IsFSearch() && !IsReadOnly() && 
		!IsBinary() && !IsEncript() && !IsDecript()  &&
		!IsEncode() && !IsCompare() && !IsDirCmp()
	)
	{
		//ドキュメントダーティ
		SetModifiedFlag(TRUE);

		//タイトル更新
		CString	objTitle = GetTitle();
		if(	(objTitle.GetLength() <	2) ||
			(objTitle.Right(2) != "	*")
		)
		{
			objTitle +=	" *";
			SetTitle(objTitle);
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択範囲テキスト取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CLine* CEditorDoc::MyGetSelectedText
	(	long	lSelectStartLine,		//選択開始行番号
		long	lSelectStartOffset,		//選択開始列番号
		long	lSelectEndLine,			//選択終了行番号
		long	lSelectEndOffset		//選択終了列番号
	)
{
	return m_objLineManager.MyGetSelectedText(lSelectStartLine,
											  lSelectStartOffset,
											  lSelectEndLine,
											  lSelectEndOffset);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：１行設定処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetOneLine
	(	long		lLineNo,		//行番号
		CLine*		pobjLine		//行
	)
{
	m_objLineManager.SetOneLine(lLineNo,pobjLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定行削除処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::DeleteOneLine
	(	long	lLineNo			//削除対象行
	)
{
	m_objLineManager.DeleteOneLine(lLineNo);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：行挿入処理															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::InsertOneLine
	(	long	lLineNo,
		CLine*	pobjLine
	)
{
	m_objLineManager.InsertOneLine(lLineNo,pobjLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マークフラグ設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagMark
	(	long	lLine,
		BOOL	bMark
	)
{
	m_objLineManager.SetFlagMark(lLine,bMark);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マークフラグ取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagMark
	(	long	lLine
	)
{
	return m_objLineManager.GetFlagMark(lLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：マーク行数取得処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
long CEditorDoc::GetMarkLineCount()
{
	return m_objLineManager.GetMarkLineCount();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：保存時行末空白削除フラグ取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagDelEndSpacesOnSave()
{
	return m_bDelEndSpacesOnSave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：保存時行末空白削除フラグ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagDelEndSpacesOnSave
	(	BOOL	bDelEndSpacesOnSave
	)
{
	m_bDelEndSpacesOnSave =	bDelEndSpacesOnSave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：保存時空白－＞タブ変換フラグ取得処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagSpace2TabOnSave()
{
	return m_bSpace2TabOnSave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：保存時空白－＞タブ変換フラグ設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagSpace2TabOnSave
	(	BOOL	bSpace2TabOnSave
	)
{
	m_bSpace2TabOnSave = bSpace2TabOnSave;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択領域のＵＵＤｅｃｏｄｅ処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::UuDecode
	(	long	lSelectStartLine,
		long	lSelectStartOffset,
		long	lSelectEndLine,
		long	lSelectEndOffset
	)
{
	CUudecoder	objUudecoder;
	CString objBuf;
	BYTE	byBuf[128];
	LONG	lLen=0,lLineNo=0;
	CString objMsg;

	/* ヘッダ行検索 */
	CLine* pobjLine	= (CLine*)GetOneLine(lSelectStartLine);
	if(pobjLine == NULL)
	{
		objMsg.LoadString(IDS_NO_UUDECODEHEADER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if( pobjLine->Compare(lSelectStartOffset,(const unsigned char*)"begin",5) &&
		pobjLine->Compare(lSelectStartOffset,(const unsigned char*)"BEGIN",5) )
	{
		objMsg.LoadString(IDS_NO_UUDECODEHEADER);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}

	/* 出力ファイル名取得 */
	char	szFileName[_MAX_FNAME];
	CString objStrFName=_T("");
	CFileDialog	objFileDialog(FALSE);
	objMsg.LoadString(IDS_UUDECODE_OUTFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//ファイル名取得
	objStrFName = objFileDialog.GetPathName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	/* 出力ファイルオープン */
	CFile	objFile;
	if( objFile.Open( szFileName, (CFile::modeCreate|CFile::modeWrite)) == NULL )
	{
		/* ファイルオープンエラー */
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::store);

	/* ＵＵｄｅｃｏｄｅ */
	for(lLineNo=lSelectStartLine+1; lLineNo<lSelectEndLine; lLineNo++)
	{
		pobjLine = (CLine*)GetOneLine(lLineNo);
		if(pobjLine == NULL)
		{
			objMsg.LoadString(IDS_NO_UUDECODEFOOTER);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}

		objBuf.Empty();
		memset(byBuf,0,sizeof(byBuf));
		lLen=0;
		objBuf = pobjLine->String();
		if(!objBuf.IsEmpty())
		{
			if(!objUudecoder.StringDecode(&objBuf,byBuf,&lLen))
				break;
			ar.Write(byBuf,lLen);
		}
	}

	/* クローズ */
	ar.Close();
	objFile.Close();

	objMsg.LoadString(IDS_END_UUDECODE);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択領域のＢａｓｅ６４デコード処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::Base64Decode
	(	long	lSelectStartLine,
		long	lSelectStartOffset,
		long	lSelectEndLine,
		long	lSelectEndOffset
	)
{
	CBase64		objBase64;
	LONG		lLine=0,lLen=0;
	CString		objBuf;
	BYTE		byBuf[128];
	CString		objMsg;

	//出力ファイル名取得
	char	szFileName[_MAX_FNAME];
	CString objStrFName=_T("");
	CFileDialog	objFileDialog(FALSE);
	objMsg.LoadString(IDS_BASE64_OUTFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//ファイル名取得
	objStrFName = objFileDialog.GetPathName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	//出力ファイルオープン
	CFile	objFile;
	if( objFile.Open( szFileName, (CFile::modeCreate|CFile::modeWrite)) == 0 )
	{
		//ファイルオープンエラー
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::store);

	//ＢＡＳＥ６４デコード
	for(lLine=lSelectStartLine; lLine<=lSelectEndLine; lLine++)
	{
		CLine* pobjLine = (CLine*)GetOneLine(lLine);
		if(pobjLine == NULL)
		{
			objMsg.LoadString(IDS_ILLEGAL_LINE_BASE64DECODE);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}

		objBuf.Empty();
		memset(byBuf,0,sizeof(byBuf));
		lLen=0;
		if(lLine==lSelectStartLine)
			objBuf = pobjLine->Data(lSelectStartOffset);
		else
			objBuf = pobjLine->String();

		if(!objBuf.IsEmpty())
		{
			if(!objBase64.StringDecode(&objBuf,byBuf,&lLen))
				break;
			ar.Write(byBuf,lLen);
		}
	}

	//クローズ
	ar.Close();
	objFile.Close();

	objMsg.LoadString(IDS_END_BASE64DECODE);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：選択領域のＱｕｏｔｅｄ－Ｐｒｉｎｔａｂｌｅデコード処理				|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::QuotedPrintableDecode
	(	long	lSelectStartLine,
		long	lSelectStartOffset,
		long	lSelectEndLine,
		long	lSelectEndOffset
	)
{
	CQuotedPrintable	objQuotedPrintable;
	LONG	lLine=0,lLen=0;
	CString objBuf;
	BYTE		byBuf[128];
	CString objMsg;

	//出力ファイル名取得
	char	szFileName[_MAX_FNAME];
	CString objStrFName=_T("");
	CFileDialog	objFileDialog(FALSE);
	objMsg.LoadString(IDS_QUOTE_OUTFILE_TITLE);
	objFileDialog.m_ofn.lpstrTitle = objMsg;
	if( objFileDialog.DoModal() != IDOK )
		return;

	//ファイル名取得
	objStrFName = objFileDialog.GetPathName();
	strcpy(szFileName,(LPCTSTR)objStrFName);

	//出力ファイルオープン
	CFile	objFile;
	if( objFile.Open( szFileName, (CFile::modeCreate|CFile::modeWrite)) == NULL )
	{
		//ファイルオープンエラー
		objMsg.LoadString(IDS_FOPEN_ERR);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	CArchive	ar(&objFile,CArchive::store);

	//ＱｕｏｔｅｄＰｒｉｎｔａｂｌｅデコード
	for(lLine=lSelectStartLine; lLine<=lSelectEndLine; lLine++)
	{
		CLine* pobjLine = (CLine*)GetOneLine(lLine);
		if(pobjLine == NULL)
		{
			objMsg.LoadString(IDS_ILLEGAL_LINE_QUOTEDDECODE);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}

		objBuf.Empty();
		memset(byBuf,0,sizeof(byBuf));
		lLen=0;
		if(lLine==lSelectStartLine)
			objBuf = pobjLine->Data(lSelectStartOffset);
		else
			objBuf = pobjLine->String();

		if(!objBuf.IsEmpty())
		{
			if(!objQuotedPrintable.StringDecode(&objBuf,byBuf,&lLen))
				break;
			ar.Write(byBuf,lLen);
			if(lLine!=lSelectEndLine)
			{
				//行末が"="でない場合には、そこを元データの改行として解釈する
				if(objBuf.GetAt(objBuf.GetLength()-1)!='=')
				{
					ar << (BYTE)'\r';
					ar << (BYTE)'\n';
				}
			}
		}
	}

	//クローズ
	ar.Close();
	objFile.Close();

	objMsg.LoadString(IDS_END_QUOTEDDECODE);
	m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONINFORMATION),0);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（カラー関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワードカラーリング処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::KeyWordColoring
	(	CLine*	pobjLine
	)
{
	m_objColorManager.KeyWordColoring(pobjLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：通常の文字色設定処理													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFontColor
	(	COLORREF	color
	)
{
	m_objColorManager.SetFontColor(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｃ・Ｃ＋＋コメント有効フラグ取得処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagComment1Clr()
{
	return m_objColorManager.GetFlagComment1Clr();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：Ｃ・Ｃ＋＋コメント有効フラグ設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagComment1Clr
	(	BOOL	bComment1Clr
	)
{
	m_objColorManager.SetFlagComment1Clr(bComment1Clr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色取得処理（Ｃ／Ｃ＋＋用）									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetCommentColor()
{
	return m_objColorManager.GetCommentColor();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色設定処理（Ｃ／Ｃ＋＋用）									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetCommentColor
	(	COLORREF	color
	)
{
	m_objColorManager.SetCommentColor(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＶＢコメント有効フラグ取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagComment2Clr()
{
	return m_objColorManager.GetFlagComment2Clr();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＶＢコメント有効フラグ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagComment2Clr
	(	BOOL	bComment2Clr
	)
{
	m_objColorManager.SetFlagComment2Clr(bComment2Clr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色取得処理（ＶＢ用）											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetCommentColor2()
{
	return m_objColorManager.GetCommentColor2();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色設定処理（ＶＢ用）											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetCommentColor2
	(	COLORREF	color
	)
{
	m_objColorManager.SetCommentColor2(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アセンブラコメント有効フラグ取得処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagComment3Clr()
{
	return m_objColorManager.GetFlagComment3Clr();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アセンブラコメント有効フラグ設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagComment3Clr
	(	BOOL	bComment3Clr
	)
{
	m_objColorManager.SetFlagComment3Clr(bComment3Clr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色取得処理（アセンブラ用）									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetCommentColor3()
{
	return m_objColorManager.GetCommentColor3();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色設定処理（アセンブラ用）									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetCommentColor3
	(	COLORREF	color
	)
{
	m_objColorManager.SetCommentColor3(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：引用符号コメント有効フラグ取得処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagComment4Clr()
{
	return m_objColorManager.GetFlagComment4Clr();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：引用符号コメント有効フラグ設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagComment4Clr
	(	BOOL	bComment4Clr
	)
{
	m_objColorManager.SetFlagComment4Clr(bComment4Clr);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色取得処理（引用符号用）										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetCommentColor4()
{
	return m_objColorManager.GetCommentColor4();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コメント色設定処理（引用符号用）										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetCommentColor4
	(	COLORREF	color
	)
{
	m_objColorManager.SetCommentColor4(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＲＬ有効フラグ取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagUrlColor()
{
	return m_objColorManager.GetFlagUrlColor();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＲＬ有効フラグ設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagUrlColor
	(	BOOL	bUrl
	)
{
	m_objColorManager.SetFlagUrlColor(bUrl);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＲＬ色取得処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetUrlColor()
{
	return m_objColorManager.GetUrlColor();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＵＲＬ色設定処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetUrlColor
	(	COLORREF	color
	)
{
	m_objColorManager.SetUrlColor(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｅ－ｍａｉｌ有効フラグ取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::GetFlagEMailColor()
{
	return m_objColorManager.GetFlagEMailColor();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｅ－ｍａｉｌ有効フラグ設定処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFlagEMailColor
	(	BOOL	bEMail
	)
{
	m_objColorManager.SetFlagEMailColor(bEMail);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｅ－ｍａｉｌ色取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetEMailColor()
{
	return m_objColorManager.GetEMailColor();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ｅ－ｍａｉｌ色設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetEMailColor
	(	COLORREF	color
	)
{
	m_objColorManager.SetEMailColor(color);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：全キーワードの削除処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::DeleteAllKeyWord()
{
	m_objColorManager.DeleteAllKeyWord();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワードＮｏ１の文字色取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetKeyNo1Color()
{
	return m_objColorManager.GetKeyNo1Color();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワード定義ファイル（ＮＯ１）読み込み＆色設定処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetKeyNo1Color
	(	COLORREF	color,
		char*		szKeyNo1Fname
	)
{
	m_objColorManager.SetKeyNo1Color(color,szKeyNo1Fname);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワードＮｏ２の文字色取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetKeyNo2Color()
{
	return m_objColorManager.GetKeyNo2Color();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワード定義ファイル（ＮＯ２）読み込み＆色設定処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetKeyNo2Color
	(	COLORREF	color,
		char*		szKeyNo2Fname
	)
{
	m_objColorManager.SetKeyNo2Color(color,szKeyNo2Fname);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワードＮｏ３の文字色取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetKeyNo3Color()
{
	return m_objColorManager.GetKeyNo3Color();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワード定義ファイル（ＮＯ３）読み込み＆色設定処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetKeyNo3Color
	(	COLORREF	color,
		char*		szKeyNo3Fname
	)
{
	m_objColorManager.SetKeyNo3Color(color,szKeyNo3Fname);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワードＮｏ４の文字色取得処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
COLORREF CEditorDoc::GetKeyNo4Color()
{
	return m_objColorManager.GetKeyNo4Color();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：キーワード定義ファイル（ＮＯ４）読み込み＆色設定処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetKeyNo4Color
	(	COLORREF	color,
		char*		szKeyNo4Fname
	)
{
	m_objColorManager.SetKeyNo4Color(color,szKeyNo4Fname);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（ＧＲＥＰ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＧＲＥＰモード設定処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetGrepMode
	(	BOOL	bGrepMode			//TRUE : ＧＲＥＰ用、FALSE : ＧＲＥＰではない
	)
{
	m_bGrepMode	= bGrepMode;

	//読み込み専用とする
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＧＲＥＰ調査															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsGrep()			//TRUE : ＧＲＥＰ用、FALSE : ＧＲＥＰではない
{
	return m_bGrepMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＧＲＥＰ結果１行追加													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendGrepLine
	(	CLine&	objGrepLine
	)
{
	m_objLineManager.AppendGrepLine(objGrepLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＨＴＴＰダウンロード結果１行追加										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendHttpLine
	(	CLine&	objHttpLine
	)
{
	m_objLineManager.AppendHttpLine(objHttpLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定行ジャンプ送信													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SendJumpMsg
	(	long	lDestLine,				// ジャンプ先行番号（１オリジン）
		BOOL	bIsInvalidate			// 再描画するか？
	)
{
	POSITION pos = GetFirstViewPosition();
	while(pos != 0)
	{
		CEditorView* pView = (CEditorView*)GetNextView(pos);
		//指定行へのジャンプ指示
		pView->Jump2LineOffset((lDestLine-1),0);
		if(bIsInvalidate)
			pView->Invalidate();

		m_pApp->MessagePump();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（文字コード変換関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字コード変換結果表示モード設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetConvMode
	(	BOOL	bConvMode			//TRUE : 文字コード変換結果表示用、FALSE : ではない
	)
{
	m_bConvMode	= bConvMode;

	//読み込み専用とする
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字コード変換結果表示モード調査処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsConv()
{
	return m_bConvMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：文字コード変換結果１行追加											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendConvLine
	(	CLine&	objConvLine
	)
{
	m_objLineManager.AppendConvLine(objConvLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（ダンプ関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダンプ結果表示モード設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetDumpMode
	(	BOOL	bDumpMode			//TRUE : ダンプ用、FALSE : ダンプではない
	)
{
	m_bDumpMode	= bDumpMode;

	//読み込み専用とする
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダンプ結果表示モード調査												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsDump()
{
	return m_bDumpMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダンプ結果１行追加													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendDumpLine
	(	CLine&	objDumpLine
	)
{
	m_objLineManager.AppendDumpLine(objDumpLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＣＲ、ＬＦの数取得処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::GetCRLFCount
	(	long*	plCRCount,		//CRの数(out)
		long*	plLFCount		//LFの数(out)
	)
{
	m_objLineManager.GetCRLFCount(plCRCount,plLFCount);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（ワードカウント関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ワードカウント結果表示モード設定										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetWCountMode
	(	BOOL	bWCountMode			//TRUE : ワードカウント用、FALSE : ワードカウントではない
	)
{
	m_bWCountMode =	bWCountMode;

	//読み込み専用とする
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ワードカウント結果表示モード調査										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsWCount()
{
	return m_bWCountMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ワードカウント結果１行追加											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendWCountLine
	(	CLine&	objWCountLine
	)
{
	m_objLineManager.AppendWCountLine(objWCountLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（ファイル検索関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル検索モード設定処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetFSearchMode
	(	BOOL	bFSearchMode			//TRUE : ファイル検索用、FALSE : ファイル検索ではない
	)
{
	m_bFSearchMode = bFSearchMode;

	//読み込み専用とする
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル検索調査														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsFSearch()			//TRUE : ファイル検索用、FALSE : ファイル検索ではない
{
	return m_bFSearchMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル検索結果１行追加												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendFSearchLine
	(	CLine&	objFSearchLine
	)
{
	m_objLineManager.AppendFSearchLine(objFSearchLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（ファイル暗号化関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル暗号化結果表示モード設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetEncriptMode
	(	BOOL	bEncriptMode
	)
{
	m_bEncriptMode = bEncriptMode;

	//読み込み専用とする
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル暗号化結果表示モード調査										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsEncript()
{
	return m_bEncriptMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル暗号化結果１行追加											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendEncriptLine
	(	CLine&	objEncriptLine
	)
{
	m_objLineManager.AppendEncriptLine(objEncriptLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（ファイル復号化関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル復号化結果表示モード設定処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetDecriptMode
	(	BOOL	bDecriptMode
	)
{
	m_bDecriptMode = bDecriptMode;

	//読み込み専用とする
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル復号化結果表示モード調査										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsDecript()
{
	return m_bDecriptMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル復号化結果１行追加											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendDecriptLine
	(	CLine&	objDecriptLine
	)
{
	m_objLineManager.AppendDecriptLine(objDecriptLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（エンコード関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：エンコード結果表示モード設定											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetEncodeMode
	(	BOOL	bEncodeMode
	)
{
	m_bEncodeMode = bEncodeMode;

	//読み込み専用とする
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：エンコード結果表示モード調査											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsEncode()
{
	return m_bEncodeMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：エンコード結果１行追加												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendEncodeLine
	(	CLine&	objEncodeLine
	)
{
	m_objLineManager.AppendEncodeLine(objEncodeLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（ファイル比較関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較結果表示モード設定										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetCompareMode
	(	BOOL		bCompareMode,
		char*		szFileCmpFile1,
		char*		szFileCmpFile2
	)
{
	m_bCompareMode = bCompareMode;
	m_objFileCmpFile1 = szFileCmpFile1;
	m_objFileCmpFile2 = szFileCmpFile2;

	//読み込み専用とする
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較結果表示モード調査										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsCompare()
{
	return m_bCompareMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較　比較ファイル取得										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::GetCompareFiles
	(	CString&	objFileCmpFile1,	// ファイル比較用第一ファイルパス
		CString&	objFileCmpFile2		// ファイル比較用第二ファイルパス
	)
{
	objFileCmpFile1	= m_objFileCmpFile1;
	objFileCmpFile2	= m_objFileCmpFile2;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較結果１行追加												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::AppendCompareLine
	(	CLine*	pobjCompareLine
	)
{
	m_objLineManager.AppendCompareLine(pobjCompareLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ファイル比較結果１行挿入												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::InsertCompareLine
	(	LONG	lLine,
		CLine*	pobjCompareLine
	)
{
	m_objLineManager.InsertCompareLine(lLine,pobjCompareLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：指定行のファイル比較行種別取得										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorDoc::GetDiffLineKind
	(	long	lLine
	)
{
	return m_objLineManager.GetDiffLineKind(lLine);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（ディレクトリ比較関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較結果表示モード設定									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetDirCmpMode
	(	BOOL	bDirCmpMode
	)
{
	m_bDirCmpMode = bDirCmpMode;

	//読み込み専用とする
	m_dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較結果表示モード調査									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsDirCmp()
{
	return m_bDirCmpMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較結果１行挿入											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::InsertDirCmpLine
	(	LONG	lLine,
		CLine*	pobjDirCmpLine
	)
{
	m_objLineManager.InsertDirCmpLine(lLine,pobjDirCmpLine);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較用、ディレクトリ名設定								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetDirCmpFolder
	(	CString&	objDirCmpFolder1,
		CString&	objDirCmpFolder2
	)
{
	m_objDirCmpFolder1 = objDirCmpFolder1;
	m_objDirCmpFolder2 = objDirCmpFolder2;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較用、ディレクトリ名取得								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::GetDirCmpFolder
	(	CString&	objDirCmpFolder1,
		CString&	objDirCmpFolder2
	)
{
	objDirCmpFolder1 = m_objDirCmpFolder1;
	objDirCmpFolder2 = m_objDirCmpFolder2;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ比較用、相違種別取得										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
int CEditorDoc::GetDirCmpLineKind
	(	long	lLineNo
	)
{
	return m_objLineManager.GetDirCmpLineKind(lLineNo);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc パブリック関数（バイナリ表示関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：バイナリ表示モード設定												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::SetBinaryMode
	(	BOOL	bBinaryMode
	)
{
	m_bBinaryMode =	bBinaryMode;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：バイナリ表示モード調査												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CEditorDoc::IsBinary()
{
	return m_bBinaryMode;
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc プロテクト関数（メール送信関連）

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メール送信状況ダイアログ表示待ち										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CEditorDoc::WaitForMailSendDlgDisp()
{
	BOOL	bRet;

	for(;;)
	{
		if( m_pobjMailSendStatusDlg != NULL ) break;
	}
	for(;;)
	{
		if( m_pobjMailSendStatusDlg->m_hWnd != NULL )
		{
			Sleep(100);
			bRet = ::IsWindow(m_pobjMailSendStatusDlg->m_hWnd);
			if(bRet!=0)
			{
				Sleep(100);
				break;
			}
		}
	}
	return;
}

