// Editor.h : EDITOR アプリケーションのメイン ヘッダー ファイル
//
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#include "resource.h"       // メイン シンボル

#if !defined(EDITORAPP_H)		//２重インクルード防止
#define EDITORAPP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <ras.h>
#include <raserror.h>

//ＲＡＳＤＬＬ関数アドレス
typedef DWORD (CALLBACK* LPFNRASFUNC1)();								//rasGetEntryNeedSize
typedef LONG  (CALLBACK* LPFNRASFUNC2)(LPRASENTRYNAME,LPDWORD);			//rasGetRasEntry
typedef LONG  (CALLBACK* LPFNRASFUNC3)(char*,char*,char*,char*,char*);	//rasConnect
typedef LONG  (CALLBACK* LPFNRASFUNC4)(LPDWORD,char*);					//rasIsConnected
typedef void  (CALLBACK* LPFNRASFUNC5)();								//rasDisConnect

//ソケットＤＬＬ関数アドレス
typedef LONG  (CALLBACK* LPFNSCKFUNC1)(char*,char*);						//GetLocalHostInfo
typedef LONG  (CALLBACK* LPFNSCKFUNC2)(WORD);								//SockServerOpen
typedef LONG  (CALLBACK* LPFNSCKFUNC3)(LONG,LONG,LPVOID,DWORD,LPDWORD);		//SockServerSend
typedef LONG  (CALLBACK* LPFNSCKFUNC4)(LONG,LONG,LPVOID,LPDWORD);			//SockServerRecv
typedef LONG  (CALLBACK* LPFNSCKFUNC5)(LONG);								//SockServerClose
typedef LONG  (CALLBACK* LPFNSCKFUNC6)(char*,WORD,BOOL);					//SockClientOpen
typedef LONG  (CALLBACK* LPFNSCKFUNC7)(LONG,LPVOID,DWORD,LPDWORD);			//SockClientSend
typedef LONG  (CALLBACK* LPFNSCKFUNC8)(LONG,LPVOID,LPDWORD);				//SockClientRecv
typedef LONG  (CALLBACK* LPFNSCKFUNC9)(LONG);								//SockClientClose
typedef LONG  (CALLBACK* LPFNSCKFUNC10)(LONG,LPVOID,DWORD,DWORD,LPDWORD);	//SockClientRecv2

//正規表現ＤＬＬ関数アドレス
typedef int  (CALLBACK* LPFNREGFUNC1)(const char*,int);						//myRegcomp
typedef int  (CALLBACK* LPFNREGFUNC2)(const char*,int,int*,int*);			//myRegexec
typedef void (CALLBACK* LPFNREGFUNC3)(int,char*,size_t);					//myRegerror
typedef void (CALLBACK* LPFNREGFUNC4)(void);								//myRegfree
typedef BOOL (CALLBACK* LPFNREGFUNC5)(const char*,int);						//myIsNeedRegcomp

class CDdeServer;
class CEditorDoc;
class CKeyBind;
class CKeyBindManager;

class CEditorApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEditorApp();											// コンストラクタ

	void	MySetForegroundWindow(HWND);
	void	SetFlag2AllWindow(CString&,BOOL&,BOOL&,BOOL&,BOOL&,BOOL&,BOOL&);
	void	SetFontSettings2AllWindow(CEditorDoc*);
	void	SetColorSettings2AllWindow();
	BOOL	MakeXerxesShortcut(BOOL);
	BOOL	CreateShortCut(const char*,char*,char*);		// ショートカット作成処理
	LONG	GetRegKey(HKEY,LPCTSTR,LPTSTR);					// レジストリキー取得処理
	void	OpenHelpTopix(ULONG);							// ヘルプトピックオープン処理
	void	DoFileAllSave();								// 全ファイル保存実行処理
	void	MessagePump();									// ウインドウズメッセージ処理
	int		GetMaxMRU();									// ＭＲＵファイル数取得
	void	SetMaxMRU(int);									// ＭＲＵファイル数設定
	BOOL	IsShowSplash();									// スプラッシュスクリーン表示調査
	void	SetShowSplash(BOOL);							// スプラッシュスクリーン表示設定
	BOOL	IsOpenNewFile();								// 起動時新規子ウインドウ作成調査
	void	SetOpenNewFile(BOOL);							// 起動時新規子ウインドウ作成設定
	BOOL	IsChdirToOpenFile();							// オープンファイルディレクトリ移動調査
	void	SetChdirToOpenFile(BOOL);						// オープンファイルディレクトリ移動設定
	void	PrepareMyReadString();							// 自作アーカイブ入力準備
	int		MyReadString(CArchive&,CString&,long*,long*);	// 自作アーカイブ入力
	BOOL	CreateNewMDIChdWnd(CDocument**,CFrameWnd**);	// 新規子ウインドウ作成
	void	SendJumpMsg(const char*,long);					// 指定行ジャンプメッセージ送信
	void	CheckFileChange();								// ファイル変更チェック
	void	GetFileErrMessage(CString&, CFileException&);	// ファイルアクセスエラーメッセージ取得
	BOOL	IsDriveRoot();									// ドライブルートディレクトリチェック
	BOOL	IsDriveRoot(const char*);						// ドライブルートディレクトリチェック
	void	ExecuteLogging(const char*);					// ログ出力
	int		GetPrintOrientation();							// 印刷用紙方向取得
	void	SetPrintOrientation(int);						// 印刷用紙方向設定
	void	GetPrintMargin(CRect&);							// 印刷余白マージン取得
	void	SetPrintMargin(CRect&);							// 印刷余白マージン設定
	void	OpenFile(CString&);								// 指定ファイルオープン
	void	RegisterShellExtDll();							// シェルエクステンションＤＬＬ登録
	void	UnRegisterShellExtDll();						// シェルエクステンションＤＬＬ登録解除
	void	ChangeDirectory(const char*);					// ディレクトリ移動
	BOOL	BrowseUrl(CString&);							// 指定ＵＲＬをブラウズ
	BOOL	ExecMailer(CString&);							// メーラーを起動
	CKeyBind* FindKeyBind(UINT,BOOL,BOOL,BOOL);				// キーバインド検索
	CKeyBind* FindKeyBind(DWORD);							// キーバインド検索
	void	addKeyBind(CKeyBind&);							// キーバインド登録
	void	resetKeyBind();									// キーバインドリセット
	void	writeKeyBind();									// キーバインド書き込み
	void	readKeyBind();									// キーバインド読み込み
	BOOL	MyPlaySound(UINT,BOOL);
	BOOL	MyPlaySound(const char*,BOOL);
	int		MyMessageBox(LPCTSTR,UINT,UINT nIDHelp=0);
	void	RemoveRecentFileList(const char*);
	BOOL	SetMenuOfIE(BOOL);								// ＩＥのXerxes編集メニュー追加／削除
	BOOL	TryDDE(const char*);							// DDE通信トライ

	CString	m_strOpenFileDirectory;			// オープンしたファイルのディレクトリ
	CString	m_strModuleFileName;			// モジュールファイル名
	CString	m_strExePath;					// 起動ディレクトリパス
	CString	m_strRecvMailPath;				// 受信済みメール保存先パス
	CString	m_strRecvMailAttachPath;		// 受信済みメール添付ファイル保存先パス
	CString	m_strMediaDirPath;
	CString	m_strCashePath;					// HTMLキャッシュ用パス
	CString	m_strShelExtDllPath;			// シェルエクステンションＤＬＬ名
	UINT	m_unOrgCaretBlinkTime;							// キャレット点滅間隔オリジナル値
	HINSTANCE m_hRasDllInstance;							// ras_dll.dllインスタンスハンドル
	HINSTANCE m_hSockDllInstance;							// sock_dll.dllインスタンスハンドル
	HINSTANCE m_hRegexDllInstance;							// regex_dll.dllインスタンスハンドル
	LPFNRASFUNC1	m_lpfnRasFunc1;							// ras_dll.dll関数アドレス
	LPFNRASFUNC2	m_lpfnRasFunc2;
	LPFNRASFUNC3	m_lpfnRasFunc3;
	LPFNRASFUNC4	m_lpfnRasFunc4;
	LPFNRASFUNC5	m_lpfnRasFunc5;
	LPFNSCKFUNC1	m_lpfnSckFunc1;							// sock_dll.dll 関数アドレス
	LPFNSCKFUNC2	m_lpfnSckFunc2;
	LPFNSCKFUNC3	m_lpfnSckFunc3;
	LPFNSCKFUNC4	m_lpfnSckFunc4;
	LPFNSCKFUNC5	m_lpfnSckFunc5;
	LPFNSCKFUNC6	m_lpfnSckFunc6;
	LPFNSCKFUNC7	m_lpfnSckFunc7;
	LPFNSCKFUNC8	m_lpfnSckFunc8;
	LPFNSCKFUNC9	m_lpfnSckFunc9;
	LPFNSCKFUNC10	m_lpfnSckFunc10;
	LPFNREGFUNC1	m_lpfnRegFunc1;							// regex_dll.dll 関数アドレス
	LPFNREGFUNC2	m_lpfnRegFunc2;
	LPFNREGFUNC3	m_lpfnRegFunc3;
	LPFNREGFUNC4	m_lpfnRegFunc4;
	LPFNREGFUNC5	m_lpfnRegFunc5;

protected:
	void	getMyDirectory();								// ディレクトリ解析処理
	void	paramAnalyze(CCommandLineInfo&);				// パラメータ解析
	BOOL	IsLicenseRegistered();							// ライセンス登録済みチェック処理
	BOOL	IsExpired(BOOL);								// 評価期限等チェック処理
	void	GenerateLisenceKeys();							// ライセンスキー作成

private:
	BOOL	m_bIsDriveRoot;
	int		m_nPrintOrientation;							// 印刷の用紙方向
	CRect	m_objPrintMarginRect;							// 印刷余白のマージン
	int		m_nMaxMRU;										// 直前に使用したファイルを保持する数
	BOOL	m_bIsOpenNewFile;								// 起動時に新規子ウインドウを作成するか
	BOOL	m_bIsChdirToOpenFile;							// オープンしたファイルのディレクトリへ移動するか
	BOOL	m_bShowSplash;									// スプラッシュスクリーンを表示するか
	FILE*	m_fpLogging;									// ロギング用ファイルポインタ
	char	m_cBuf4MyReadString;							// 自作アーカイブ入力用文字一時バッファ
	CDdeServer*	m_pobjDdeServer;							// ＤＤＥサーバー
	HANDLE	m_hMutex;
	CKeyBindManager* m_pobjKeyBindManager;

	//{{AFX_VIRTUAL(CEditorApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEditorApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileAllsave();
	afx_msg void OnUpdateFileAllsave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpenonexit(CCmdUI* pCmdUI);
	afx_msg void OnFileOpenonexit();
	afx_msg void OnHelpLicense();
	afx_msg void OnHelpContent();
	afx_msg void OnUpdateHelpContent(CCmdUI* pCmdUI);
	afx_msg void OnAllsaveExit();
	afx_msg void OnUpdateAllsaveExit(CCmdUI* pCmdUI);
	afx_msg void OnHelpContent2();
	afx_msg void OnUpdateHelpContent2(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(EDITORAPP_H)

