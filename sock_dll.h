////////////////////////////////////////////////////////////////////////////////
//
// Name:			sock_dll.h
// Descriptions：	Windows Socket Access DLL Header File
// Version:			1.0
// Create:			1998.12.07
// Update:			2000.04.24
// Author:			S.Ekura
//
////////////////////////////////////////////////////////////////////////////////

#if !defined(SOCK_DLL_H)							//２重インクルード防止
#define SOCK_DLL_H

//関数プロトタイプ宣言（公開関数）
#ifdef __cplusplus
extern "C" {
#endif
LONG __declspec( dllexport )GetLocalHostInfo(char*,char*);					// ローカルマシンのホスト名、ＩＰアドレス取得
LONG __declspec( dllexport )SockServerOpen(WORD);							// 通信回線サーバーオープン
LONG __declspec( dllexport )SockServerSend(LONG,LONG,LPVOID,DWORD,LPDWORD);	// サーバーセンド
LONG __declspec( dllexport )SockServerRecv(LONG,LONG,LPVOID,LPDWORD);		// サーバーレシーブ
LONG __declspec( dllexport )SockServerClose(LONG);							// 通信回線サーバークローズ
LONG __declspec( dllexport )SockClientOpen(char*,WORD);						// 通話回線クライアントオープン
LONG __declspec( dllexport )SockClientSend(LONG,LPVOID,DWORD,LPDWORD);		// クライアントセンド
LONG __declspec( dllexport )SockClientRecv(LONG,LPVOID,LPDWORD);			// クライアントレシーブ
LONG __declspec( dllexport )SockClientClose(LONG);							// 通信回線クライアントクローズ
#ifdef __cplusplus
};
#endif

#endif // !defined(SOCK_DLL_H)				//２重インクルード防止

