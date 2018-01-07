// MyPropertyPage9.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#pragma warning(disable:4786)
#include "editor.h"
#include "editor_def.h"
#include "MyPropertyPage9.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMyPropertyPage9, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage9 プロパティ ページ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：コンストラクタ														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage9::CMyPropertyPage9() : CPropertyPage(CMyPropertyPage9::IDD)
{
	m_pApp = (CEditorApp*)AfxGetApp();

	//{{AFX_DATA_INIT(CMyPropertyPage9)
	m_bIsDialup = FALSE;
	m_bAutoDisconnect = FALSE;
	m_nEncodeType = -1;
	m_bIsSendDate = FALSE;
	m_bIsSendMessageID = FALSE;
	m_bIsAutoSetSubject = FALSE;
	m_nAuthorizationType = -1;
	//}}AFX_DATA_INIT

	m_bIsChanged = FALSE;								// 変更発生フラグ
	m_nCurrentAccountNo = 0;
	memset(m_stMailSettings,0,sizeof(MAIL_SETTINGS)*MAIL_ACCOUNT_MAX);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：デストラクタ															|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
CMyPropertyPage9::~CMyPropertyPage9()
{
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＤＤＸデータ交換														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyPropertyPage9)
	DDX_Control(pDX, IDC_COMBO_ACCOUNT, m_objComboAccount);
	DDX_Control(pDX, IDC_EDIT_MAILADDRESS, m_objEditMailAddress);
	DDX_Control(pDX, IDC_EDIT_SMTPSERVER, m_objEditSmtpServer);
	DDX_Control(pDX, IDC_EDIT_POPSERVER, m_objEditPopServer);
	DDX_Control(pDX, IDC_EDIT_POPUSERNAME, m_objEditPopUserName);
	DDX_Control(pDX, IDC_EDIT_POPPASSWD, m_objEditPopPasswd);
	DDX_Check(pDX, IDC_CHECK_DIALUP, m_bIsDialup);
	DDX_Control(pDX, IDC_COMBO_DIALUPENTRY, m_objComboDialupEntry);
	DDX_Control(pDX, IDC_EDIT_RASUSERNAME, m_objEditRasUserName);
	DDX_Control(pDX, IDC_EDIT_RASPASSWD, m_objEditRasPasswd);
	DDX_Check(pDX, IDC_CHECK_AUTODISCONNECT, m_bAutoDisconnect);
	DDX_Radio(pDX, IDC_RADIO_ENCODE_BASE64, m_nEncodeType);
	DDX_Control(pDX, IDC_EDIT_SIGNATURE, m_objEditSignature);
	DDX_Check(pDX, IDC_CHECK_SENDDATE, m_bIsSendDate);
	DDX_Check(pDX, IDC_CHECK_SENDMSGID, m_bIsSendMessageID);
	DDX_Check(pDX, IDC_CHECK_SUBJECTAUTOSET, m_bIsAutoSetSubject);
	DDX_Radio(pDX, IDC_RADIO_AUTH_STANDARD, m_nAuthorizationType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyPropertyPage9, CPropertyPage)
	//{{AFX_MSG_MAP(CMyPropertyPage9)
	ON_EN_CHANGE(IDC_EDIT_MAILADDRESS, OnChangeEditMailaddress)
	ON_EN_CHANGE(IDC_EDIT_SMTPSERVER, OnChangeEditSmtpserver)
	ON_EN_CHANGE(IDC_EDIT_POPSERVER, OnChangeEditPopserver)
	ON_EN_CHANGE(IDC_EDIT_POPUSERNAME, OnChangeEditPopusername)
	ON_EN_CHANGE(IDC_EDIT_POPPASSWD, OnChangeEditPoppasswd)
	ON_BN_CLICKED(IDC_CHECK_DIALUP, OnCheckDialup)
	ON_CBN_SELCHANGE(IDC_COMBO_DIALUPENTRY, OnSelchangeComboDialupentry)
	ON_EN_CHANGE(IDC_EDIT_RASUSERNAME, OnChangeEditRasusername)
	ON_EN_CHANGE(IDC_EDIT_RASPASSWD, OnChangeEditRaspasswd)
	ON_BN_CLICKED(IDC_CHECK_AUTODISCONNECT, OnCheckAutoDisconnect)
	ON_BN_CLICKED(IDC_RADIO_ENCODE_BASE64, OnRadioEncodeBase64)
	ON_BN_CLICKED(IDC_RADIO_ENCODE_QUOTED, OnRadioEncodeQuoted)
	ON_EN_CHANGE(IDC_EDIT_SIGNATURE, OnChangeEditSignature)
	ON_BN_CLICKED(IDC_RADIO_ENCODE_UUENCODE, OnRadioEncodeUuencode)
	ON_BN_CLICKED(IDC_CHECK_SENDDATE, OnCheckSenddate)
	ON_BN_CLICKED(IDC_CHECK_SENDMSGID, OnCheckSendmsgid)
	ON_BN_CLICKED(IDC_CHECK_SUBJECTAUTOSET, OnCheckSubjectautoset)
	ON_BN_CLICKED(IDC_RADIO_AUTH_STANDARD, OnRadioAuthStandard)
	ON_BN_CLICKED(IDC_RADIO_AUTH_APOP, OnRadioAuthApop)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCOUNT, OnSelchangeComboAccount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage9 パブリック関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：変更発生調査処理														|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage9::IsChanged() 
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
void CMyPropertyPage9::ClearChangeFlag()
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
void CMyPropertyPage9::ChangesToRegistry()
{
	int nCnt=0;
	char szSection[64];

	//アカウント別設定情報を全部保存
	m_pApp->WriteProfileInt("Mail","CurrentAccountNo",m_nCurrentAccountNo);
	for(nCnt=0; nCnt<MAIL_ACCOUNT_MAX; nCnt++)
	{
		sprintf(szSection,"Mail_Account%d",nCnt);

		m_pApp->WriteProfileString(szSection,"MailAddress",m_stMailSettings[nCnt].m_szMailAddress);
		m_pApp->WriteProfileString(szSection,"SmtpServer",m_stMailSettings[nCnt].m_szSmtpServer);
		m_pApp->WriteProfileString(szSection,"PopServer",m_stMailSettings[nCnt].m_szPopServer);
		m_pApp->WriteProfileString(szSection,"PopUser",m_stMailSettings[nCnt].m_szPopUserName);
		m_pApp->WriteProfileString(szSection,"PopPasswd",m_stMailSettings[nCnt].m_szPopPasswd);
		m_pApp->WriteProfileInt(szSection,"bIsDialup",m_stMailSettings[nCnt].m_bIsDialup);
		m_pApp->WriteProfileString(szSection,"DialupEntry",m_stMailSettings[nCnt].m_szDialupEntry);
		m_pApp->WriteProfileString(szSection,"RasUserName",m_stMailSettings[nCnt].m_szRasUserName);
		m_pApp->WriteProfileString(szSection,"RasPasswd",m_stMailSettings[nCnt].m_szRasPasswd);
		m_pApp->WriteProfileInt(szSection,"AutoDisconnect",m_stMailSettings[nCnt].m_bAutoDisconnect);
		m_pApp->WriteProfileInt(szSection,"EncodeType",m_stMailSettings[nCnt].m_nEncodeType);
		m_pApp->WriteProfileInt(szSection,"bIsAutoSetSubject",m_stMailSettings[nCnt].m_bIsAutoSetSubject);
		m_pApp->WriteProfileInt(szSection,"bIsSendMessage",m_stMailSettings[nCnt].m_bIsSendMessageID);
		m_pApp->WriteProfileInt(szSection,"bIsSendDate",m_stMailSettings[nCnt].m_bIsSendDate);
		m_pApp->WriteProfileString(szSection,"Signature",m_stMailSettings[nCnt].m_szSignature);
		m_pApp->WriteProfileInt(szSection,"nAuthorizationType",m_stMailSettings[nCnt].m_nAuthorizationType);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage9 メッセージ ハンドラ

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアログ表示初期化													|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
BOOL CMyPropertyPage9::OnInitDialog() 
{
	int		nCnt=0;
	char	szSection[64];
	CString objMsg;

	CPropertyPage::OnInitDialog();

	//フォント設定
	m_objEditMailAddress.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objEditSmtpServer.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objEditPopServer.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objEditPopUserName.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objEditPopPasswd.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objEditRasUserName.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
	m_objEditRasPasswd.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);
//	m_objEditSignature.SetFont((CFont::FromHandle((HFONT)GetStockObject(SYSTEM_FIXED_FONT))),TRUE);

	//文字数制限
	m_objEditMailAddress.LimitText(63);
	m_objEditSmtpServer.LimitText(63);
	m_objEditPopServer.LimitText(63);
	m_objEditPopUserName.LimitText(63);
	m_objEditPopPasswd.LimitText(63);
	m_objEditRasUserName.LimitText(63);
	m_objEditRasPasswd.LimitText(63);
	m_objEditSignature.LimitText(1023);

	//アカウント別設定情報を全部取得
	m_nCurrentAccountNo = m_pApp->GetProfileInt("Mail","CurrentAccountNo",0);
	if(m_nCurrentAccountNo < 0 || m_nCurrentAccountNo > 8)
		m_nCurrentAccountNo = 0;
	for(nCnt=0; nCnt<MAIL_ACCOUNT_MAX; nCnt++)
	{
		sprintf(szSection,"Mail_Account%d",nCnt);

		//アカウント名コンボボックス設定
		objMsg.LoadString(IDS_MAILACCOUNT_0 + nCnt);
		m_objComboAccount.AddString(objMsg);

		strcpy(m_stMailSettings[nCnt].m_szMailAddress,(LPCTSTR)m_pApp->GetProfileString(szSection,"MailAddress",""));
		strcpy(m_stMailSettings[nCnt].m_szSmtpServer,(LPCTSTR)m_pApp->GetProfileString(szSection,"SmtpServer",""));
		strcpy(m_stMailSettings[nCnt].m_szPopServer,(LPCTSTR)m_pApp->GetProfileString(szSection,"PopServer",""));
		strcpy(m_stMailSettings[nCnt].m_szPopUserName,(LPCTSTR)m_pApp->GetProfileString(szSection,"PopUser",""));
		strcpy(m_stMailSettings[nCnt].m_szPopPasswd,(LPCTSTR)m_pApp->GetProfileString(szSection,"PopPasswd",""));
		m_stMailSettings[nCnt].m_bIsDialup          = m_pApp->GetProfileInt(szSection,"bIsDialup",0);
		strcpy(m_stMailSettings[nCnt].m_szDialupEntry,(LPCTSTR)m_pApp->GetProfileString(szSection,"DialupEntry",""));
		strcpy(m_stMailSettings[nCnt].m_szRasUserName,(LPCTSTR)m_pApp->GetProfileString(szSection,"RasUserName",""));
		strcpy(m_stMailSettings[nCnt].m_szRasPasswd,(LPCTSTR)m_pApp->GetProfileString(szSection,"RasPasswd",""));
		m_stMailSettings[nCnt].m_bAutoDisconnect    = m_pApp->GetProfileInt(szSection,"AutoDisconnect",0);
		m_stMailSettings[nCnt].m_nEncodeType        = m_pApp->GetProfileInt(szSection,"EncodeType",SMTP_ENCODE_BASE64);
		m_stMailSettings[nCnt].m_bIsAutoSetSubject  = m_pApp->GetProfileInt(szSection,"bIsAutoSetSubject",1);
		m_stMailSettings[nCnt].m_bIsSendMessageID   = m_pApp->GetProfileInt(szSection,"bIsSendMessage",1);
		m_stMailSettings[nCnt].m_bIsSendDate        = m_pApp->GetProfileInt(szSection,"bIsSendDate",1);
		strcpy(m_stMailSettings[nCnt].m_szSignature,(LPCTSTR)m_pApp->GetProfileString(szSection,"Signature",""));
		m_stMailSettings[nCnt].m_nAuthorizationType	= m_pApp->GetProfileInt(szSection,"nAuthorizationType",POP3_AUTHENTICATE_STANDARD);
	}

	//現在のアカウントの設定を画面へ
	m_objComboAccount.SetCurSel(m_nCurrentAccountNo);
	m_objEditMailAddress.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szMailAddress);
	m_objEditSmtpServer.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szSmtpServer);
	m_objEditPopServer.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szPopServer);
	m_objEditPopUserName.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szPopUserName);
	m_objEditPopPasswd.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szPopPasswd);
	m_objEditRasUserName.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szRasUserName);
	m_objEditRasPasswd.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szRasPasswd);
	m_objEditSignature.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szSignature);
	m_bIsDialup          = m_stMailSettings[m_nCurrentAccountNo].m_bIsDialup;
	m_bAutoDisconnect    = m_stMailSettings[m_nCurrentAccountNo].m_bAutoDisconnect;
	m_nEncodeType        = m_stMailSettings[m_nCurrentAccountNo].m_nEncodeType;
	m_bIsAutoSetSubject  = m_stMailSettings[m_nCurrentAccountNo].m_bIsAutoSetSubject;
	m_bIsSendMessageID   = m_stMailSettings[m_nCurrentAccountNo].m_bIsSendMessageID;
	m_bIsSendDate        = m_stMailSettings[m_nCurrentAccountNo].m_bIsSendDate;
	m_nAuthorizationType = m_stMailSettings[m_nCurrentAccountNo].m_nAuthorizationType;

	//ダイアルアップエントリコンボボックス設定
	SetDialupEntryCombo();

	//メンバ変数－＞コントロール
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：アカウントコンボボックス選択変更時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnSelchangeComboAccount() 
{
	int nIndex = m_objComboAccount.GetCurSel();
	if(nIndex == CB_ERR)
		return;
	if(nIndex < 0 || nIndex >= MAIL_ACCOUNT_MAX)
		return;

	//選択されたアカウントに対応する設定情報を画面へ
	m_nCurrentAccountNo = nIndex;
	m_objEditMailAddress.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szMailAddress);
	m_objEditSmtpServer.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szSmtpServer);
	m_objEditPopServer.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szPopServer);
	m_objEditPopUserName.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szPopUserName);
	m_objEditPopPasswd.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szPopPasswd);
	m_objComboDialupEntry.SelectString(-1,m_stMailSettings[m_nCurrentAccountNo].m_szDialupEntry);
	m_objEditRasUserName.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szRasUserName);
	m_objEditRasPasswd.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szRasPasswd);
	m_objEditSignature.SetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szSignature);
	m_bIsDialup          = m_stMailSettings[m_nCurrentAccountNo].m_bIsDialup;
	m_bAutoDisconnect    = m_stMailSettings[m_nCurrentAccountNo].m_bAutoDisconnect;
	m_nEncodeType        = m_stMailSettings[m_nCurrentAccountNo].m_nEncodeType;
	m_bIsAutoSetSubject  = m_stMailSettings[m_nCurrentAccountNo].m_bIsAutoSetSubject;
	m_bIsSendMessageID   = m_stMailSettings[m_nCurrentAccountNo].m_bIsSendMessageID;
	m_bIsSendDate        = m_stMailSettings[m_nCurrentAccountNo].m_bIsSendDate;
	m_nAuthorizationType = m_stMailSettings[m_nCurrentAccountNo].m_nAuthorizationType;

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

	//メンバ変数－＞コントロール
	UpdateData(FALSE);
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：メールアドレスエディットボックス変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnChangeEditMailaddress() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_objEditMailAddress.GetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szMailAddress,
								sizeof(m_stMailSettings[m_nCurrentAccountNo].m_szMailAddress));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＳＭＴＰサーバーエディットボックス変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnChangeEditSmtpserver() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_objEditSmtpServer.GetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szSmtpServer,
								sizeof(m_stMailSettings[m_nCurrentAccountNo].m_szSmtpServer));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＰＯＰ３サーバーエディットボックス変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnChangeEditPopserver() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_objEditPopServer.GetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szPopServer,
								sizeof(m_stMailSettings[m_nCurrentAccountNo].m_szPopServer));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＰＯＰ３ユーザ名エディットボックス変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnChangeEditPopusername() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_objEditPopUserName.GetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szPopUserName,
								sizeof(m_stMailSettings[m_nCurrentAccountNo].m_szPopUserName));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＰＯＰ３パスワードエディットボックス変更時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnChangeEditPoppasswd() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_objEditPopPasswd.GetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szPopPasswd,
								sizeof(m_stMailSettings[m_nCurrentAccountNo].m_szPopPasswd));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「モデムを使用してダイアルアップ」選択時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnCheckDialup() 
{
	//コントロール－＞メンバ変数
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_stMailSettings[m_nCurrentAccountNo].m_bIsDialup = m_bIsDialup;

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
void CMyPropertyPage9::OnSelchangeComboDialupentry() 
{
	int nIndex = m_objComboDialupEntry.GetCurSel();
	if(nIndex == CB_ERR)
		return;

	m_objComboDialupEntry.GetLBText(nIndex,m_stMailSettings[m_nCurrentAccountNo].m_szDialupEntry);

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
void CMyPropertyPage9::OnChangeEditRasusername() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_objEditRasUserName.GetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szRasUserName,
								sizeof(m_stMailSettings[m_nCurrentAccountNo].m_szRasUserName));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ＲＡＳパスワードエディットボックス変更時処理							|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnChangeEditRaspasswd() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_objEditRasPasswd.GetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szRasPasswd,
								sizeof(m_stMailSettings[m_nCurrentAccountNo].m_szRasPasswd));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「メール送信終了時自動切断」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnCheckAutoDisconnect() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_stMailSettings[m_nCurrentAccountNo].m_bAutoDisconnect = m_bAutoDisconnect;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「MIME:Base64」選択時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnRadioEncodeBase64() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	if(m_nEncodeType != m_stMailSettings[m_nCurrentAccountNo].m_nEncodeType)
	{
		//変更された
		m_bIsChanged = TRUE;
		SetModified();

		m_stMailSettings[m_nCurrentAccountNo].m_nEncodeType = m_nEncodeType;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「MIME:Quoted-Printable」選択時処理									|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnRadioEncodeQuoted() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	if(m_nEncodeType != m_stMailSettings[m_nCurrentAccountNo].m_nEncodeType)
	{
		//変更された
		m_bIsChanged = TRUE;
		SetModified();

		m_stMailSettings[m_nCurrentAccountNo].m_nEncodeType = m_nEncodeType;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「uuencode」選択時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnRadioEncodeUuencode() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	if(m_nEncodeType != m_stMailSettings[m_nCurrentAccountNo].m_nEncodeType)
	{
		//変更された
		m_bIsChanged = TRUE;
		SetModified();

		m_stMailSettings[m_nCurrentAccountNo].m_nEncodeType = m_nEncodeType;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「件名自動設定」選択時処理											|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnCheckSubjectautoset() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_stMailSettings[m_nCurrentAccountNo].m_bIsAutoSetSubject = m_bIsAutoSetSubject;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「メッセージＩＤヘッダ送信」選択時処理								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnCheckSendmsgid() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_stMailSettings[m_nCurrentAccountNo].m_bIsSendMessageID = m_bIsSendMessageID;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＤＡＴＥヘッダ送信」選択時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnCheckSenddate() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_stMailSettings[m_nCurrentAccountNo].m_bIsSendDate = m_bIsSendDate;
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：署名エディットボックス変更時処理										|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnChangeEditSignature() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	m_bIsChanged = TRUE;
	SetModified();

	m_objEditSignature.GetWindowText(m_stMailSettings[m_nCurrentAccountNo].m_szSignature,
							sizeof(m_stMailSettings[m_nCurrentAccountNo].m_szSignature));
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ユーザ認証：USER/PASSコマンドによる標準」選択時処理					|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnRadioAuthStandard() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	if(m_nAuthorizationType != m_stMailSettings[m_nCurrentAccountNo].m_nAuthorizationType)
	{
		//変更された
		m_bIsChanged = TRUE;
		SetModified();

		m_stMailSettings[m_nCurrentAccountNo].m_nAuthorizationType = m_nAuthorizationType;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ユーザ認証：PASSコマンドによる標準」選択時処理						|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnRadioAuthApop() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);

	if(m_nAuthorizationType != m_stMailSettings[m_nCurrentAccountNo].m_nAuthorizationType)
	{
		//変更された
		m_bIsChanged = TRUE;
		SetModified();

		m_stMailSettings[m_nCurrentAccountNo].m_nAuthorizationType = m_nAuthorizationType;
	}
}

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：「ＯＫ」ボタン押下時処理												|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::OnOK() 
{
	//コントロール－＞メンバ変数データ取り込み
	UpdateData(TRUE);
	CPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CMyPropertyPage9 プロテクト関数

/*[2]-----------------------------------------------------------------------------[2]*/
/*|	概要	：ダイアルアップエントリコンボボックス設定								|*/
/*|	解説	：																		|*/
/*|	備考	：																		|*/
/*|	履歴	：																		|*/
/*|	I/F		：																		|*/
/*[0]-----------------------------------------------------------------------------[0]*/
void CMyPropertyPage9::SetDialupEntryCombo()
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
		m_bAutoDisconnect = FALSE;
		for(nCnt=0; nCnt<MAIL_ACCOUNT_MAX; nCnt++)
		{
			m_stMailSettings[nCnt].m_bIsDialup = FALSE;
			memset(m_stMailSettings[nCnt].m_szDialupEntry,0,sizeof(m_stMailSettings[nCnt].m_szDialupEntry));
			memset(m_stMailSettings[nCnt].m_szRasUserName,0,sizeof(m_stMailSettings[nCnt].m_szRasUserName));
			memset(m_stMailSettings[nCnt].m_szRasPasswd,0,sizeof(m_stMailSettings[nCnt].m_szRasPasswd));
			m_stMailSettings[nCnt].m_bAutoDisconnect = FALSE;
		}
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
		m_bAutoDisconnect = FALSE;
		for(nCnt=0; nCnt<MAIL_ACCOUNT_MAX; nCnt++)
		{
			m_stMailSettings[nCnt].m_bIsDialup = FALSE;
			memset(m_stMailSettings[nCnt].m_szDialupEntry,0,sizeof(m_stMailSettings[nCnt].m_szDialupEntry));
			memset(m_stMailSettings[nCnt].m_szRasUserName,0,sizeof(m_stMailSettings[nCnt].m_szRasUserName));
			memset(m_stMailSettings[nCnt].m_szRasPasswd,0,sizeof(m_stMailSettings[nCnt].m_szRasPasswd));
			m_stMailSettings[nCnt].m_bAutoDisconnect = FALSE;
		}
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
		m_bAutoDisconnect = FALSE;
		for(nCnt=0; nCnt<MAIL_ACCOUNT_MAX; nCnt++)
		{
			m_stMailSettings[nCnt].m_bIsDialup = FALSE;
			memset(m_stMailSettings[nCnt].m_szDialupEntry,0,sizeof(m_stMailSettings[nCnt].m_szDialupEntry));
			memset(m_stMailSettings[nCnt].m_szRasUserName,0,sizeof(m_stMailSettings[nCnt].m_szRasUserName));
			memset(m_stMailSettings[nCnt].m_szRasPasswd,0,sizeof(m_stMailSettings[nCnt].m_szRasPasswd));
			m_stMailSettings[nCnt].m_bAutoDisconnect = FALSE;
		}
		return;
	}

	//接続名をコンボボックスへ設定
	for(ndx=0; ndx<cEntry; ndx++)
		m_objComboDialupEntry.AddString(lpRasEntry[ndx].szEntryName);

	bFind = FALSE;
	for(ndx=0; ndx<cEntry; ndx++)
	{
		if(!stricmp(m_stMailSettings[m_nCurrentAccountNo].m_szDialupEntry,lpRasEntry[ndx].szEntryName))
		{
			m_objComboDialupEntry.SetCurSel(ndx);
			bFind = TRUE;
			break;
		}
	}
	if(bFind == FALSE)
	{
		strcpy(m_stMailSettings[m_nCurrentAccountNo].m_szDialupEntry,lpRasEntry[0].szEntryName);
		m_objComboDialupEntry.SetCurSel(0);
	}

	//ＲＡＳエントリメモリ開放
	free(lpRasEntry);
}

