/*[0]-----------------------------------------------------------------------------[0]*/
/*|	大分類		：オートダイアルライブラリ											|*/
/*|	小分類		：ＲＡＳ接続														|*/
/*|	ファイル名	：ras_dll.h															|*/
/*|	TAB			：4																	|*/
/*|	Create		：1998.12.05														|*/
/*|	Update		：2000.04.24														|*/
/*|	Author		：S.Ekura															|*/
/*|	注記		：rasapi32.lib をリンクすること。									|*/
/*[0]-----------------------------------------------------------------------------[0]*/

#if !defined(RASDLL_H)							//２重インクルード防止
#define RASDLL_H

//関数プロトタイプ宣言（公開関数）
#ifdef __cplusplus
extern "C" {
#endif
LONG	__declspec( dllexport )rasConnect(char*,char*,char*,char*,char*);//ＲＡＳ接続処理
LONG	__declspec( dllexport )rasIsConnected(LPDWORD);					 //ＲＡＳ接続完了チェック
void	__declspec( dllexport )rasDisConnect();							 //ＲＡＳ切断処理
#ifdef __cplusplus
};
#endif

#endif // !defined(RASDLL_H)					//２重インクルード防止

