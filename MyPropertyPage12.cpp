// MyPropertyPage12.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "MyPropertyPage12.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage12, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage12 プロパティ ページ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage12::CMyPropertyPage12() : CPropertyPage(CMyPropertyPage12::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();
	m_bIsChanged = FALSE;								// 変更発生フラグ

	//{{AFX_DATA_INIT(CMyPropertyPage12)
	m_bIsUseProxy = FALSE;
	m_bIsDialup = FALSE;
	m_bIsAutoDisconnect = FALSE;
	//}}AFX_DATA_INIT

	m_objProxyIPAddress = _T("");
	m_objDialupEntry = _T("");
	m_objRasUserName = _T("");
	m_objRasPasswd = _T("");
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage12::~CMyPropertyPage12()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＸデータ交換														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage12)
	DDX_Check(pDX, IDC_CHECK_USEPROXY, m_bIsUseProxy);
	DDX_Control(pDX, IDC_EDIT_PROXYIPADDRESS, m_objEditProxyIPAddress);
	DDX_Check(pDX, IDC_CHECK_DIALUP, m_bIsDialup);
	DDX_Control(pDX, IDC_COMBO_DIALUPENTRY, m_objComboDialupEntry);
	DDX_Control(pDX, IDC_EDIT_RASUSERNAME, m_objEditRasUserName);
	DDX_Control(pDX, IDC_EDIT_RASPASSWD, m_objEditRasPasswd);
	DDX_Check(pDX, IDC_CHECK_AUTODISCONNECT, m_bIsAutoDisconnect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyPropertyPage12, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage12)
	ON_BN_CLICKED(IDC_CHECK_USEPROXY, OnCheckUseproxy)
	ON_EN_CHANGE(IDC_EDIT_PROXYIPADDRESS, OnChangeEditProxyipaddress)
	ON_BN_CLICKED(IDC_CHECK_DIALUP, OnCheckDialup)
	ON_CBN_SELCHANGE(IDC_COMBO_DIALUPENTRY, OnSelchangeComboDialupentry)
	ON_EN_CHANGE(IDC_EDIT_RASUSERNAME, OnChangeEditRasusername)
	ON_EN_CHANGE(IDC_EDIT_RASPASSWD, OnChangeEditRaspasswd)
	ON_BN_CLICKED(IDC_CHECK_AUTODISCONNECT, OnCheckAutoDisconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage12 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生調査処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage12::IsChanged() 
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
void CMyPropertyPage12::ClearChangeFlag()
{
	m_bIsChanged = FALSE;
	SetModified(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage12 メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage12::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	//デフォルトデータ設定
	m_objEditProxyIPAddress.SetWindowText(m_objProxyIPAddress);
	m_objEditRasUserName.SetWindowText(m_objRasUserName);
	m_objEditRasPasswd.SetWindowText(m_objRasPasswd);

	//ダイアルアップエントリコンボボックス設定
	SetDialupEntryCombo();

	//プロキシサーバを使う場合にはプロキシサーバアドレス欄を有効化
	if(m_bIsUseProxy)
		m_objEditProxyIPAddress.EnableWindow(TRUE);
	else
		m_objEditProxyIPAddress.EnableWindow(FALSE);

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「プロキシサーバを使う」チェック時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnCheckUseproxy() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	if(m_bIsUseProxy)
		m_objEditProxyIPAddress.EnableWindow(TRUE);
	else
		m_objEditProxyIPAddress.EnableWindow(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：プロキシサーバのアドレス変更時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnChangeEditProxyipaddress() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「モデムを使用してダイアルアップ」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnCheckDialup() 
{
	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	if(!m_bIsDialup)
	{
		m_objComboDialupEntry.EnableWindow(FALSE);
		m_objEditRasUserName.EnableWindow(FALSE);
		m_objEditRasPasswd.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AUTODISCONNECT)->EnableWindow(FALSE);
	}
	else
	{
		m_objComboDialupEntry.EnableWindow(TRUE);
		m_objEditRasUserName.EnableWindow(TRUE);
		m_objEditRasPasswd.EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_AUTODISCONNECT)->EnableWindow(TRUE);
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアルアップエントリコンボボックス選択変更時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnSelchangeComboDialupentry() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＲＡＳユーザ名エディットボックス変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnChangeEditRasusername() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＲＡＳパスワードエディットボックス変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnChangeEditRaspasswd() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「終了時自動切断」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnCheckAutoDisconnect() 
{
	m_bIsChanged = TRUE;
	SetModified();
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＯＫ」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::OnOK() 
{
	int nIndex=0;
	CString objMsg;

	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	//データ取得
	m_objEditProxyIPAddress.GetWindowText(m_objProxyIPAddress);
	nIndex = m_objComboDialupEntry.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	m_objComboDialupEntry.GetLBText(nIndex,m_objDialupEntry);
	m_objEditRasUserName.GetWindowText(m_objRasUserName);
	m_objEditRasPasswd.GetWindowText(m_objRasPasswd);

	//ブランクチェック
	if(m_bIsUseProxy && m_objProxyIPAddress.IsEmpty())
	{
		objMsg.LoadString(IDS_INPUT_PROXYIPADDRESS);
		m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
		return;
	}
	if(m_bIsDialup)
	{
		if(m_objRasUserName.IsEmpty())
		{
			objMsg.LoadString(IDS_RAS_NOUSERNAME);
			m_pApp->MyMessageBox(objMsg,(MB_OK|MB_ICONSTOP),0);
			return;
		}
	}

	CPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage12 プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアルアップエントリコンボボックス設定								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage12::SetDialupEntryCombo()
{
	int				nCnt=0;
	LONG			lRet=0;
	BOOL			bFind=FALSE;
	LPRASENTRYNAME	lpRasEntry=NULL;
	DWORD			cEntry=0;
	DWORD			dwRet=0,dwNeedSize=0;
	UINT			ndx=0;

	//ダイアルアップネットワークインストール済みチェック
	if(m_pApp->m_hRasDllInstance == NULL)
	{
		GetDlgItem(IDC_CHECK_DIALUP)->EnableWindow(FALSE);
		m_objComboDialupEntry.EnableWindow(FALSE);
		m_objEditRasUserName.EnableWindow(FALSE);
		m_objEditRasPasswd.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AUTODISCONNECT)->EnableWindow(FALSE);
		m_bIsDialup = FALSE;
		m_bIsAutoDisconnect = FALSE;
		return;
	}

	//関数アドレス取得
	m_pApp->m_lpfnRasFunc1 = (LPFNRASFUNC1)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasGetEntryNeedSize");
	m_pApp->m_lpfnRasFunc2 = (LPFNRASFUNC2)::GetProcAddress(m_pApp->m_hRasDllInstance,"rasGetRasEntry");
	if( m_pApp->m_lpfnRasFunc1 == NULL || m_pApp->m_lpfnRasFunc2 == NULL)
	{
		GetDlgItem(IDC_CHECK_DIALUP)->EnableWindow(FALSE);
		m_objComboDialupEntry.EnableWindow(FALSE);
		m_objEditRasUserName.EnableWindow(FALSE);
		m_objEditRasPasswd.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AUTODISCONNECT)->EnableWindow(FALSE);
		m_bIsDialup = FALSE;
		m_bIsAutoDisconnect = FALSE;
		return;
	}

	//必要サイズ取得
	dwNeedSize = m_pApp->m_lpfnRasFunc1();
	lpRasEntry = (LPRASENTRYNAME)malloc((UINT)dwNeedSize);
	memset(lpRasEntry,0,dwNeedSize);

	//ＲＡＳ接続エントリ取得(rasGetRasEntry)
	lRet = m_pApp->m_lpfnRasFunc2( lpRasEntry, &cEntry );
	if( lRet != 0 ) //NG
	{
		if(lpRasEntry)
			free(lpRasEntry);

		GetDlgItem(IDC_CHECK_DIALUP)->EnableWindow(FALSE);
		m_objComboDialupEntry.EnableWindow(FALSE);
		m_objEditRasUserName.EnableWindow(FALSE);
		m_objEditRasPasswd.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AUTODISCONNECT)->EnableWindow(FALSE);
		m_bIsDialup = FALSE;
		m_bIsAutoDisconnect = FALSE;
		return;
	}

	//接続名をコンボボックスへ設定
	for(ndx=0; ndx<cEntry; ndx++)
		m_objComboDialupEntry.AddString(lpRasEntry[ndx].szEntryName);

	bFind = FALSE;
	for(ndx=0; ndx<cEntry; ndx++)
	{
		if(!m_objDialupEntry.CompareNoCase(lpRasEntry[ndx].szEntryName))
		{
			m_objComboDialupEntry.SetCurSel(ndx);
			bFind = TRUE;
			break;
		}
	}
	if(bFind == FALSE)
	{
		m_objDialupEntry = lpRasEntry[0].szEntryName;
		m_objComboDialupEntry.SetCurSel(0);
	}

	//ＲＡＳエントリメモリ開放
	free(lpRasEntry);
}

