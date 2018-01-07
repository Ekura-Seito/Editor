#include "stdafx.h"
#include <Afxtempl.h>			//ＭＦＣのＣＭａｐを使用する
#include <afxole.h>				//for COleDataSource, COleDropTarget
#include <io.h>					//for _access()
//#include "zmouse.h"				//for インテリマウス
#include "Editor_Def.h"			//共通ＤＥＦＩＮＥ
#include "StatusBarDisp.h"		//ステータスバー表示情報構造体
#include "ColorManager.h"
#include "Editor.h"
#include "ChildFrm.h"
#include "BitmapClient.h"
#include "Operation.h"
#include "OperationManager.h"
#include "MainFrm.h"

#ifdef AFX_TARG_JPN

//ツールバーのカスタマイズ用
CToolBarInfo CMainFrame::m_wndToolBar_Info[] =
{
	{{0 , ID_FILE_NEW          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0} , "新規作成"     },
	{{1 , ID_FILE_OPEN         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1} , "開く"         },
	{{2 , ID_FILE_CLOSE        , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2} , "閉じる"       },
	{{3 , ID_FILE_SAVE         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3} , "上書き保存"   },
	{{4 , ID_FILE_ALLSAVE      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4} , "すべて保存"   },
	{{5 , ID_FILE_PRINT        , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5} , "印刷"         },
	{{6 , ID_FILE_PRINT_PREVIEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6} , "印刷プレビュー"  },
	{{7 , ID_FILE_PRINT_SETUP  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 7} , "プリンタ設定" },
	{{0 , ID_SEPARATOR         , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 8} , ""             },
	{{8 , ID_SETTINGS          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 9} , "設定"         },
	{{0 , ID_SEPARATOR         , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 10}, ""             },
	{{9 , ID_HELP_CONTENT      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 11}, "ヘルプ目次"   },
	{{10, ID_HELP_CONTENT2     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 12}, "HTMLヘルプ目次" },
	{{11, ID_APP_ABOUT         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 13}, "バージョン情報"  }
};
CToolBarInfo CMainFrame::m_wndToolBarEdit_Info[] = 
{
	{{0 , ID_EDIT_UNDO         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0} , "元に戻す"     },
	{{1 , ID_EDIT_REDO         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1} , "繰り返し"     },
	{{2 , ID_EDIT_CUT          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2} , "切り取り"     },
	{{3 , ID_EDIT_COPY         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3} , "コピー"     	 },
	{{4 , ID_EDIT_PASTE        , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4} , "貼り付け"  	 },
	{{5 , ID_EDIT_DEL          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5} , "削除"     	 },
	{{0 , ID_SEPARATOR         , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 6} , ""             },
	{{6 , ID_EDIT_CLIPVIEW     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 7} , "ｸﾘｯﾌﾟﾎﾞｰﾄﾞの中を見る" },
	{{7 , ID_EDIT_BROWSEURL    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 8} , "URLをブラウズ"  },
	{{8 , ID_EDIT_EXECMAILER   , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 9} , "メーラーを起動" },
	{{9 , ID_EDIT_SETREADONLY  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 10}, "書込み禁止設定" },
	{{10, ID_EDIT_UNSETREADONLY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 11}, "書込み禁止解除" },
	{{11, ID_EDIT_INSDAY       , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 12}, "日付を挿入"   },
	{{12, ID_EDIT_INSTIME      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 13}, "時刻を挿入"   },
	{{13, ID_EDIT_INSTAB       , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 14}, "タブを挿入"   },
	{{14, ID_EDIT_DELTAB       , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 15}, "タブを削除"   },
	{{15, ID_EDIT_INSCPPCOMMENT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 16}, "C/C++行コメント挿入" },
	{{16, ID_EDIT_INSVBCOMMENT , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 17}, "VB行コメント挿入" },
	{{17, ID_EDIT_INSASMCOMMENT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 18}, "ｱｾﾝﾌﾞﾗ行ｺﾒﾝﾄ挿入" },
	{{18, ID_EDIT_INSQUOTE     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 19}, "引用符号挿入" },
	{{19, ID_EDIT_INSQUOTE     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 19}, "引用符号挿入" }
};
CToolBarInfo CMainFrame::m_wndToolBarFind_Info[] = 
{
	{{0,  ID_EDIT_FIND         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0} , "検索"         },
	{{1,  ID_EDIT_FINDCONTINUE , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1} , "次を検索"     },
	{{2,  ID_EDIT_FINDREVCONTINUE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2}, "前を検索"    },
	{{3,  ID_EDIT_REPLACE      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3},  "置換"         },
	{{4,  ID_EDIT_GREP         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4},  "ファイルから検索" },
	{{5,  ID_EDIT_FILECOMPARE  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5},  "ファイル比較" },
	{{6,  ID_EDIT_DIRCOMPARE   , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6},  "ディレクトリ比較" }
};
CToolBarInfo CMainFrame::m_wndToolBarJmp_Info[] = 
{
	{{0,  ID_EDIT_JUMP2TOP     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},  "先頭行ジャンプ" },
	{{1,  ID_EDIT_JUMP2BOTTOM  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1},  "最終行ジャンプ" },
	{{2,  ID_EDIT_JUMP2LINE    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2},  "指定行ジャンプ" },
	{{3,  ID_JUMP_SETMARK      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3},  "現在行マーク"   },
	{{4,  ID_JUMP_DELMARK      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4},  "マーク解除"     },
	{{5,  ID_JUMP_NEXTMARK     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5},  "次のマーク位置へ" },
	{{6,  ID_JUMP_PREVMARK     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6},  "前のマーク位置へ" }
};
CToolBarInfo CMainFrame::m_wndToolBarTools_Info[] = 
{
	{{0,  ID_FILE_SENDMAIL     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},  "メール送信"     },
	{{1,  ID_EDIT_SJISDUMP     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1},  "Shift-JISダンプ" },
	{{2,  ID_EDIT_CODECONV     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2},  "文字コード変換" },
	{{3,  ID_EDIT_KAIGYOCONV   , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3},  "改行コード変換" },
	{{4,  ID_EDIT_WORDCOUNT    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4},  "ワードカウント" },
	{{5,  ID_EDIT_FILESEARCH   , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5},  "ファイル検索"   },
	{{6,  ID_EDIT_FILETOUCH    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6},  "ファイルタッチ" },
	{{7,  ID_SECURITY_ENCRIPT  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 7},  "ファイル暗号化" },
	{{8,  ID_SECURITY_DECRIPT  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 8},  "ファイル復号化" },
	{{9,  ID_TOOL_OPE_STARTENDRECORD, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 9}, "キー操作記録開始／終了" }
};
CToolBarInfo CMainFrame::m_wndToolBarWnd_Info[] = 
{
	{{0,  ID_WINDOW_NEW        , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},  "新規ウインドウ" },
	{{1,  ID_WINDOW_SPLIT      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1},  "分割"           },
	{{2,  ID_WINDOW_ARRANGE    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2},  "アイコンの整列" },
	{{3,  ID_WND_NEXT          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3},  "次のウインドウ" },
	{{4,  ID_WND_PREV          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4},  "前のウインドウ" },
	{{5,  ID_WINDOW_CASCADE    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5},  "重ねて表示"     },
	{{6,  ID_WINDOW_TILE_HORZ  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6},  "上下に並べて表示" },
	{{7,  ID_WINDOW_TILE_VERT  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 7},  "左右に並べて表示" },
	{{8,  ID_WINDOW_CLOSE      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 8},  "閉じる"         },
	{{9,  ID_WINDOW_ALLCLOSE   , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 9},  "すべて閉じる"   }
};

#endif

#ifdef AFX_TARG_ENU

//ツールバーのカスタマイズ用
CToolBarInfo CMainFrame::m_wndToolBar_Info[] =
{
	{{0 , ID_FILE_NEW          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0} , "New"             },
	{{1 , ID_FILE_OPEN         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1} , "Open"            },
	{{2 , ID_FILE_CLOSE        , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2} , "Close"           },
	{{3 , ID_FILE_SAVE         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3} , "Save"            },
	{{4 , ID_FILE_ALLSAVE      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4} , "Save all files"  },
	{{5 , ID_FILE_PRINT        , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5} , "Print"           },
	{{6 , ID_FILE_PRINT_PREVIEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6} , "Print preview"   },
	{{7 , ID_FILE_PRINT_SETUP  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 7} , "Print setup"     },
	{{0 , ID_SEPARATOR         , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 8} , ""                },
	{{8 , ID_SETTINGS          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 9} , "Settings"        },
	{{0 , ID_SEPARATOR         , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 10}, ""             },
	{{9 , ID_HELP_CONTENT      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 11}, "Table of contents"},
	{{10, ID_HELP_CONTENT2     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 12}, "HTML Help table of contents" },
	{{11, ID_APP_ABOUT         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 13}, "Version information"}
};
CToolBarInfo CMainFrame::m_wndToolBarEdit_Info[] = 
{
	{{0 , ID_EDIT_UNDO         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0} , "Undo"         },
	{{1 , ID_EDIT_REDO         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1} , "Redo"         },
	{{2 , ID_EDIT_CUT          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2} , "Cut"          },
	{{3 , ID_EDIT_COPY         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3} , "Copy"     	 },
	{{4 , ID_EDIT_PASTE        , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4} , "Paste"    	 },
	{{5 , ID_EDIT_DEL          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5} , "Delete"     	 },
	{{0 , ID_SEPARATOR         , TBSTATE_ENABLED, TBSTYLE_SEP   , 0, 6} , ""             },
	{{6 , ID_EDIT_CLIPVIEW     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 7} , "View clipboard" },
	{{7 , ID_EDIT_BROWSEURL    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 8} , "Browse URL"   },
	{{8 , ID_EDIT_EXECMAILER   , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 9} , "Start a mailer" },
	{{9 , ID_EDIT_SETREADONLY  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 10}, "Make the ban on writing" },
	{{10, ID_EDIT_UNSETREADONLY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 11}, "Cancel the ban on writing" },
	{{11, ID_EDIT_INSDAY       , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 12}, "Insert date"   },
	{{12, ID_EDIT_INSTIME      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 13}, "Insert time"   },
	{{13, ID_EDIT_INSTAB       , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 14}, "Insert a tab"   },
	{{14, ID_EDIT_DELTAB       , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 15}, "Delete a tab"   },
	{{15, ID_EDIT_INSCPPCOMMENT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 16}, "Insert C/C++ line comment" },
	{{16, ID_EDIT_INSVBCOMMENT , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 17}, "Insert VB line comment" },
	{{17, ID_EDIT_INSASMCOMMENT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 18}, "Insert Assembler line comment" },
	{{18, ID_EDIT_INSQUOTE     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 19}, "Insert quotation-mark" },
};
CToolBarInfo CMainFrame::m_wndToolBarFind_Info[] = 
{
	{{0,  ID_EDIT_FIND         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0} , "Find the specified text" },
	{{1,  ID_EDIT_FINDCONTINUE , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1} , "Continue to find downward" },
	{{2,  ID_EDIT_FINDREVCONTINUE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2}, "Continue to find upward"  },
	{{3,  ID_EDIT_REPLACE      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3},  "Replace the specified text" },
	{{4,  ID_EDIT_GREP         , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4},  "Grep" },
	{{5,  ID_EDIT_FILECOMPARE  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5},  "File comparison" },
	{{6,  ID_EDIT_DIRCOMPARE   , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6},  "Directory comparison" }
};
CToolBarInfo CMainFrame::m_wndToolBarJmp_Info[] = 
{
	{{0,  ID_EDIT_JUMP2TOP     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},  "Jump to a head line" },
	{{1,  ID_EDIT_JUMP2BOTTOM  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1},  "Jump to a last line" },
	{{2,  ID_EDIT_JUMP2LINE    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2},  "Jump to a specified line" },
	{{3,  ID_JUMP_SETMARK      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3},  "Mark the current line" },
	{{4,  ID_JUMP_DELMARK      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4},  "Mark release the current line" },
	{{5,  ID_JUMP_NEXTMARK     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5},  "Jump to the marked line under" },
	{{6,  ID_JUMP_PREVMARK     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6},  "Jump to the marked line above" }
};
CToolBarInfo CMainFrame::m_wndToolBarTools_Info[] = 
{
	{{0,  ID_FILE_SENDMAIL     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},  "Send mail"     },
	{{1,  ID_EDIT_SJISDUMP     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1},  "Dumping in Shift-JIS code" },
	{{2,  ID_EDIT_CODECONV     , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2},  "Character code conversion" },
	{{3,  ID_EDIT_KAIGYOCONV   , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3},  "New-line code conversion" },
	{{4,  ID_EDIT_WORDCOUNT    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4},  "WORD count" },
	{{5,  ID_EDIT_FILESEARCH   , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5},  "File search"   },
	{{6,  ID_EDIT_FILETOUCH    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6},  "File touch" },
	{{7,  ID_SECURITY_ENCRIPT  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 7},  "File encryption" },
	{{8,  ID_SECURITY_DECRIPT  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 8},  "File decryption" },
	{{9,  ID_TOOL_OPE_STARTENDRECORD, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 9}, "Recording key operations" }
};
CToolBarInfo CMainFrame::m_wndToolBarWnd_Info[] = 
{
	{{0,  ID_WINDOW_NEW        , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0},  "New window" },
	{{1,  ID_WINDOW_SPLIT      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 1},  "Split window"   },
	{{2,  ID_WINDOW_ARRANGE    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 2},  "Arrange icons" },
	{{3,  ID_WND_NEXT          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 3},  "Next window" },
	{{4,  ID_WND_PREV          , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 4},  "Prev window" },
	{{5,  ID_WINDOW_CASCADE    , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 5},  "Cascade"     },
	{{6,  ID_WINDOW_TILE_HORZ  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 6},  "Arrange up and down" },
	{{7,  ID_WINDOW_TILE_VERT  , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 7},  "Arrange right and left" },
	{{8,  ID_WINDOW_CLOSE      , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 8},  "Close"         },
	{{9,  ID_WINDOW_ALLCLOSE   , TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 9},  "Close all"   }
};

#endif
