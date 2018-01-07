// MyPropertyPage11.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <io.h>					// for _access
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "MyPropertyPage11.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage11 プロパティ ページ

IMPLEMENT_DYNCREATE(CMyPropertyPage11, CPropertyPage)

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage11::CMyPropertyPage11() : CPropertyPage(CMyPropertyPage11::IDD)
{
	//{{AFX_DATA_INIT(CMyPropertyPage11)
	//}}AFX_DATA_INIT

	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								//変更発生フラグ
	memset(m_stSoundDef,0,sizeof(SOUND_DEF)*13);
	memset(m_hRootItems,0,sizeof(HTREEITEM)*4);
	//イメージリスト作成
	m_ImageList.Create(IDB_SOUNDICONS, 16, 1, RGB(255, 255, 255));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage11::~CMyPropertyPage11()
{
}

void CMyPropertyPage11::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage11)
	DDX_Control(pDX, IDC_COMBO_WAVEFILES, m_objWaveFilesCombo);
	DDX_Control(pDX, IDC_TREE_SOUNDS, m_objSoundsTreeCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage11, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage11)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_CBN_SELCHANGE(IDC_COMBO_WAVEFILES, OnSelchangeComboWavefiles)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SOUNDS, OnSelchangedTreeSounds)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage11 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生調査処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage11::IsChanged() 
{
	return m_bIsChanged;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生フラグクリア処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::ClearChangeFlag()
{
	m_bIsChanged = FALSE;
	SetModified(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更内容をレジストリへ反映させる処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::ChangesToRegistry()
{
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnXerxesStart",         m_stSoundDef[0].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnXerxesStart",       m_stSoundDef[0].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnXerxesEnd",           m_stSoundDef[1].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnXerxesEnd",         m_stSoundDef[1].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnMessageInfo",         m_stSoundDef[2].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnMessageInfo",       m_stSoundDef[2].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnMessageStop",         m_stSoundDef[3].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnMessageStop",       m_stSoundDef[3].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnMessageQuestion",     m_stSoundDef[4].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnMessageQuestion",   m_stSoundDef[4].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnMessageExclamation",  m_stSoundDef[5].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnMessageExclamation",m_stSoundDef[5].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowMinimize",      m_stSoundDef[6].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowMinimize",    m_stSoundDef[6].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowMaximize",      m_stSoundDef[7].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowMaximize",    m_stSoundDef[7].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowRestore",       m_stSoundDef[8].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowRestore",     m_stSoundDef[8].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowNext",          m_stSoundDef[9].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowNext",        m_stSoundDef[9].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowPrev",          m_stSoundDef[10].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowPrev",        m_stSoundDef[10].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowClose",         m_stSoundDef[11].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowClose",       m_stSoundDef[11].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnWindowAllClose",      m_stSoundDef[12].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnWindowAllClose",    m_stSoundDef[12].szWaveFileName);
	m_pApp->WriteProfileInt("Sound",   "IsPlayOnFileNew",             m_stSoundDef[13].bIsPlay);
	m_pApp->WriteProfileString("Sound","WaveFileOnFileNew",           m_stSoundDef[13].szWaveFileName);
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage11 メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage11::OnInitDialog() 
{
	int nCnt=0;
	CString objFilter, *pobjStr=NULL;
	BOOL		bRet=FALSE;
	CFileFind	objFileFind;
	CString		objMsg;


	CPropertyPage::OnInitDialog();

	//イメージリストセット
	m_objSoundsTreeCtrl.SetImageList(&m_ImageList, TVSIL_NORMAL);

	//SOUND_DEF構造体初期化
	m_stSoundDef[0].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnXerxesStart",0);
	strcpy(m_stSoundDef[0].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnXerxesStart",""));
	m_stSoundDef[1].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnXerxesEnd",0);
	strcpy(m_stSoundDef[1].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnXerxesEnd",""));
	m_stSoundDef[2].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnMessageInfo",0);
	strcpy(m_stSoundDef[2].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnMessageInfo",""));
	m_stSoundDef[3].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnMessageStop",0);
	strcpy(m_stSoundDef[3].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnMessageStop",""));
	m_stSoundDef[4].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnMessageQuestion",0);
	strcpy(m_stSoundDef[4].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnMessageQuestion",""));
	m_stSoundDef[5].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnMessageExclamation",0);
	strcpy(m_stSoundDef[5].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnMessageExclamation",""));
	m_stSoundDef[6].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowMinimize",0);
	strcpy(m_stSoundDef[6].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMinimize",""));
	m_stSoundDef[7].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowMaximize",0);
	strcpy(m_stSoundDef[7].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowMaximize",""));
	m_stSoundDef[8].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowRestore",0);
	strcpy(m_stSoundDef[8].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowRestore",""));
	m_stSoundDef[9].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowNext",0);
	strcpy(m_stSoundDef[9].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowNext",""));
	m_stSoundDef[10].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowPrev",0);
	strcpy(m_stSoundDef[10].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowPrev",""));
	m_stSoundDef[11].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowClose",0);
	strcpy(m_stSoundDef[11].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowClose",""));
	m_stSoundDef[12].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnWindowAllClose",0);
	strcpy(m_stSoundDef[12].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnWindowAllClose",""));
	m_stSoundDef[13].bIsPlay = m_pApp->GetProfileInt("Sound","IsPlayOnFileNew",0);
	strcpy(m_stSoundDef[13].szWaveFileName,(LPCTSTR)m_pApp->GetProfileString("Sound","WaveFileOnFileNew",""));

	//ルートアイテム挿入
	objMsg.LoadString(IDS_SOUND_XERXES_STARTEND);
	m_hRootItems[0] = m_objSoundsTreeCtrl.InsertItem(objMsg,2,3,TVI_ROOT,TVI_LAST);
	if(m_hRootItems[0] != NULL)
	{
		//「Xerxesの起動」アイテム挿入
		objMsg.LoadString(IDS_SOUND_XERXES_START);
		if(m_stSoundDef[0].bIsPlay != 0)
			m_stSoundDef[0].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[0],TVI_LAST);
		else
			m_stSoundDef[0].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[0],TVI_LAST);
		//「Xerxesの終了」アイテム挿入
		objMsg.LoadString(IDS_SOUND_XERXES_END);
		if(m_stSoundDef[1].bIsPlay != 0)
			m_stSoundDef[1].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[0],TVI_LAST);
		else
			m_stSoundDef[1].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[0],TVI_LAST);

		m_objSoundsTreeCtrl.Expand(m_hRootItems[0],TVE_EXPAND);
	}

	//ルートアイテム挿入
	objMsg.LoadString(IDS_SOUND_MESSAGE);
	m_hRootItems[1] = m_objSoundsTreeCtrl.InsertItem(objMsg,2,3,TVI_ROOT,TVI_LAST);
	if(m_hRootItems[1] != NULL)
	{
		//「メッセージ（情報）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_MESSAGE_INFO);
		if(m_stSoundDef[2].bIsPlay != 0)
			m_stSoundDef[2].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[1],TVI_LAST);
		else
			m_stSoundDef[2].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[1],TVI_LAST);
		//「メッセージ（警告）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_MESSAGE_STOP);
		if(m_stSoundDef[3].bIsPlay != 0)
			m_stSoundDef[3].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[1],TVI_LAST);
		else
			m_stSoundDef[3].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[1],TVI_LAST);
		//「メッセージ（問い合わせ）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_MESSAGE_QUESTION);
		if(m_stSoundDef[4].bIsPlay != 0)
			m_stSoundDef[4].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[1],TVI_LAST);
		else
			m_stSoundDef[4].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[1],TVI_LAST);
		//「メッセージ（注意）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_MESSAGE_EXCLAIM);
		if(m_stSoundDef[5].bIsPlay != 0)
			m_stSoundDef[5].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[1],TVI_LAST);
		else
			m_stSoundDef[5].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[1],TVI_LAST);

		m_objSoundsTreeCtrl.Expand(m_hRootItems[1],TVE_EXPAND);
	}

	//ルートアイテム挿入
	objMsg.LoadString(IDS_SOUND_WINDOW);
	m_hRootItems[2] = m_objSoundsTreeCtrl.InsertItem(objMsg,2,3,TVI_ROOT,TVI_LAST);
	if(m_hRootItems[2] != NULL)
	{
		//「ウインドウ（最小化）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_WINDOW_MINIMIZE);
		if(m_stSoundDef[6].bIsPlay != 0)
			m_stSoundDef[6].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[6].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//「ウインドウ（最大化）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_WINDOW_MAXIMIZE);
		if(m_stSoundDef[7].bIsPlay != 0)
			m_stSoundDef[7].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[7].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//「ウインドウ（もとのサイズに戻す）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_WINDOW_RESTORE);
		if(m_stSoundDef[8].bIsPlay != 0)
			m_stSoundDef[8].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[8].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//「ウインドウ（次のウインドウ）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_WINDOW_NEXT);
		if(m_stSoundDef[9].bIsPlay != 0)
			m_stSoundDef[9].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[9].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//「ウインドウ（前のウインドウ）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_WINDOW_PREV);
		if(m_stSoundDef[10].bIsPlay != 0)
			m_stSoundDef[10].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[10].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//「ウインドウ（閉じる）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_WINDOW_CLOSE);
		if(m_stSoundDef[11].bIsPlay != 0)
			m_stSoundDef[11].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[11].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);
		//「ウインドウ（すべて閉じる）」アイテム挿入
		objMsg.LoadString(IDS_SOUND_WINDOW_ALLCLOSE);
		if(m_stSoundDef[12].bIsPlay != 0)
			m_stSoundDef[12].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[2],TVI_LAST);
		else
			m_stSoundDef[12].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[2],TVI_LAST);

		m_objSoundsTreeCtrl.Expand(m_hRootItems[2],TVE_EXPAND);
	}

	//ルートアイテム挿入
	objMsg.LoadString(IDS_SOUND_FILE);
	m_hRootItems[3] = m_objSoundsTreeCtrl.InsertItem(objMsg,2,3,TVI_ROOT,TVI_LAST);
	if(m_hRootItems[3] != NULL)
	{
		//「新規作成」アイテム挿入
		objMsg.LoadString(IDS_SOUND_FILE_NEW);
		if(m_stSoundDef[13].bIsPlay != 0)
			m_stSoundDef[13].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,1,1,m_hRootItems[3],TVI_LAST);
		else
			m_stSoundDef[13].hItem = m_objSoundsTreeCtrl.InsertItem(objMsg,0,0,m_hRootItems[3],TVI_LAST);

		m_objSoundsTreeCtrl.Expand(m_hRootItems[3],TVE_EXPAND);
	}
	if(m_hRootItems[0] != NULL)
		m_objSoundsTreeCtrl.Select(m_hRootItems[0],TVGN_FIRSTVISIBLE);

	//Wave Fileコンボボックス設定
	objMsg.LoadString(IDS_SOUND_NONE);
	m_objWaveFilesCombo.AddString(objMsg);
	objFilter.Format("%s%s",m_pApp->m_strMediaDirPath,"\\*.wav");
	CObArray objStringArray;
	objStringArray.SetSize(0);
	MakeFileList(objFilter,&objStringArray);
	for(nCnt=0; nCnt<objStringArray.GetSize(); nCnt++)
	{
		pobjStr = (CString*)objStringArray.GetAt(nCnt);
		m_objWaveFilesCombo.AddString((*pobjStr));
	}
	m_objWaveFilesCombo.SetCurSel(0);
	m_objWaveFilesCombo.EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	//変更発生フラグクリア
	m_bIsChanged = FALSE;

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：サウンドツリー選択変更時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::OnSelchangedTreeSounds(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TV_ITEM item;
	char szBuf[256],szFileName[_MAX_FNAME];
	BOOL bRet=FALSE, bFind=FALSE;
	int	nIndex=0, nCnt=0;
	CString objMsg;

	m_bIsChanged = TRUE;
	SetModified();

	//旧選択アイテム情報取得
	memset(szBuf,0,sizeof(szBuf));
	memset(&item,0,sizeof(TV_ITEM));
	item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE;
	item.hItem = pNMTreeView->itemOld.hItem;
	item.pszText = szBuf;
	item.cchTextMax = 256;
	bRet = m_objSoundsTreeCtrl.GetItem(&item);
	if(!bRet)
	{
		*pResult = 0;
		return;
	}

	for(bFind=FALSE,nCnt=0; nCnt<4; nCnt++)
	{
		if(item.hItem == m_hRootItems[nCnt])
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind)
	{
		//旧選択アイテムがノードであった場合は何もしない
		;
	}
	else
	{
		//現在選択中のＷＡＶＥファイル名取得して内部構造体へ記録
		nIndex = m_objWaveFilesCombo.GetCurSel();
		if(nIndex == CB_ERR)
			return;
		else if(nIndex == 0) //なし
		{
			//アイテムイメージ設定
			m_objSoundsTreeCtrl.SetItemImage(pNMTreeView->itemOld.hItem,0,0);

			//音を鳴らさないに設定
			for(nCnt=0; nCnt<14; nCnt++)
			{
				if(item.hItem == m_stSoundDef[nCnt].hItem)
				{
					m_stSoundDef[nCnt].bIsPlay = 0;
					memset(m_stSoundDef[nCnt].szWaveFileName,0,_MAX_FNAME);
					break;
				}
			}
		}
		else
		{
			//アイテムイメージ設定
			m_objSoundsTreeCtrl.SetItemImage(pNMTreeView->itemOld.hItem,1,1);

			memset(szFileName,0,sizeof(szFileName));
			m_objWaveFilesCombo.GetLBText(nIndex,szFileName);

			//ＷＡＶＥファイル名設定
			for(nCnt=0; nCnt<14; nCnt++)
			{
				if(item.hItem == m_stSoundDef[nCnt].hItem)
				{
					m_stSoundDef[nCnt].bIsPlay = 1;
					strcpy(m_stSoundDef[nCnt].szWaveFileName,szFileName);
					break;
				}
			}
		}
	}

	//新選択アイテム情報取得
	memset(szBuf,0,sizeof(szBuf));
	memset(&item,0,sizeof(TV_ITEM));
	item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE;
	item.hItem = pNMTreeView->itemNew.hItem;
	item.pszText = szBuf;
	item.cchTextMax = 256;
	bRet = m_objSoundsTreeCtrl.GetItem(&item);
	if(!bRet)
	{
		*pResult = 0;
		return;
	}

	//選択されているアイテムがノードの場合は何もしない
	for(bFind=FALSE,nCnt=0; nCnt<4; nCnt++)
	{
		if(item.hItem == m_hRootItems[nCnt])
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind)
	{
		m_objWaveFilesCombo.SetCurSel(0);
		m_objWaveFilesCombo.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);
		*pResult = 0;
		return;
	}
	else
	{
		//選択されているアイテムに対応するＷＡＶＥファイルをコンボボックスへ
		memset(szFileName,0,sizeof(szFileName));
		for(nCnt=0; nCnt<14; nCnt++)
		{
			if(item.hItem == m_stSoundDef[nCnt].hItem)
			{
				strcpy(szFileName,m_stSoundDef[nCnt].szWaveFileName);
				break;
			}
		}

		if(strlen(szFileName) != 0)
		{
			m_objWaveFilesCombo.SelectString(-1,szFileName);
			GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);
		}
		else
		{
			m_objWaveFilesCombo.SetCurSel(0);
			GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);
		}
		m_objWaveFilesCombo.EnableWindow(TRUE);
	}

	*pResult = 0;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＷＡＶＥファイルコンボボックス選択変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::OnSelchangeComboWavefiles() 
{
	int nIndex=0, nCnt=0;
	HTREEITEM hItem=NULL;
	TV_ITEM item;
	char szBuf[256],szFileName[_MAX_FNAME];

	//現在選択中のツリーアイテム取得
	hItem = m_objSoundsTreeCtrl.GetSelectedItem();
	if(hItem == NULL)
		return;
	memset(szBuf,0,sizeof(szBuf));
	memset(&item,0,sizeof(TV_ITEM));
	item.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_IMAGE;
	item.hItem = hItem;
	item.pszText = szBuf;
	item.cchTextMax = 256;
	if(m_objSoundsTreeCtrl.GetItem(&item) == FALSE)
		return;
	//選択されているアイテムがノードの場合は何もしない
	for(nCnt=0; nCnt<4; nCnt++)
	{
		if(item.hItem == m_hRootItems[nCnt])
			return;
	}

	m_bIsChanged = TRUE;
	SetModified();

	//現在選択中のＷＡＶＥファイル名取得
	nIndex = m_objWaveFilesCombo.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	else if(nIndex == 0) //「ＷＡＶＥファイルなし」になった
	{
		//アイテムイメージ設定
		m_objSoundsTreeCtrl.SetItemImage(hItem,0,0);

		//テストボタン無効化
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(FALSE);

		//音を鳴らさないに設定
		for(nCnt=0; nCnt<14; nCnt++)
		{
			if(item.hItem == m_stSoundDef[nCnt].hItem)
			{
				m_stSoundDef[nCnt].bIsPlay = 0;
				memset(m_stSoundDef[nCnt].szWaveFileName,0,_MAX_FNAME);
				break;
			}
		}
	}
	else
	{
		//アイテムイメージ設定
		m_objSoundsTreeCtrl.SetItemImage(hItem,1,1);

		//テストボタン有効化
		GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(TRUE);

		memset(szFileName,0,sizeof(szFileName));
		m_objWaveFilesCombo.GetLBText(nIndex,szFileName);

		//ＷＡＶＥファイル名設定
		for(nCnt=0; nCnt<14; nCnt++)
		{
			if(item.hItem == m_stSoundDef[nCnt].hItem)
			{
				m_stSoundDef[nCnt].bIsPlay = 1;
				strcpy(m_stSoundDef[nCnt].szWaveFileName,szFileName);
				break;
			}
		}
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「テスト」ボタン押下時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::OnButtonTest() 
{
	int	nIndex=0;
	char szBuf[256],szFilePath[_MAX_PATH];

	//現在選択中のＷＡＶＥファイル名取得
	nIndex = m_objWaveFilesCombo.GetCurSel();
	if(nIndex == CB_ERR || nIndex == 0)
		return;
	memset(szBuf,0,sizeof(szBuf));
	if(m_objWaveFilesCombo.GetLBText(nIndex,szBuf) == CB_ERR)
		return;
	sprintf(szFilePath,"%s\\%s", (LPCTSTR)m_pApp->m_strMediaDirPath,szBuf);
	if(_access(szFilePath,0) == -1)
		return;

	//ＷＡＶＥファイルテスト
	CWaitCursor objCursor;
	m_pApp->MyPlaySound(szFilePath,TRUE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＯＫ」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::OnOK() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);
	CPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage11 プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ディレクトリ内ファイルリスト作成										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage11::MakeFileList		//ＴＲＵＥ：正常終了、ＦＡＬＳＥ：異常、キャンセル終了
	(	CString&	objFilter,
		CObArray*	pobjObArray
	)
{
	BOOL		bRet=FALSE;
	CString		objFilePath=_T("");
	CFileFind	objFileFind;

	//ファイル検索
	bRet = objFileFind.FindFile(objFilter);
	if(bRet != FALSE)
	{
		while(objFileFind.FindNextFile()!=FALSE)
		{
			if( objFileFind.IsDots() != FALSE ) // "." or ".."
				continue;
			if( objFileFind.IsDirectory() != FALSE ) //ディレクトリ
				continue;

			objFilePath = objFileFind.GetFileName();
			AddFileBySort(objFilePath,pobjObArray);
		}
		//最後の１個
		if(!objFileFind.IsDots() && !objFileFind.IsDirectory() )
		{
			objFilePath = objFileFind.GetFileName();
			AddFileBySort(objFilePath,pobjObArray);
		}
	}
	objFileFind.Close();

	if(bRet)
		return TRUE;
	else
		return FALSE;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アルファベット順にリストへ追加										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：                  													|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage11::AddFileBySort
	(	CString&	objFileName,
		CObArray*	pobjArray
	)
{
	int nIndex=0;
	int nSize=pobjArray->GetSize();
	CString* pobjStrTmp=NULL;
	CString* pobjStrBuf = new CString();
	(*pobjStrBuf) = objFileName;

	for(nIndex=0; nIndex<nSize; nIndex++)
	{
		pobjStrTmp = (CString*)pobjArray->GetAt(nIndex);
		if(pobjStrTmp->CompareNoCase((LPCTSTR)objFileName) <= 0)
			continue;
		else
			break;
	}

	if(nIndex == nSize)
		pobjArray->Add((CObject*)pobjStrBuf); //最後に追加
	else
		pobjArray->InsertAt(nIndex,(CObject*)pobjStrBuf);
}

