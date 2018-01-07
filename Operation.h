// Operation.h: COperation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(Operation_H)		//２重インクルード防止
#define Operation_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CEditorApp;
class CEditorView;
class CEditorDoc;

class COperation : public CObject
{
protected:
	DECLARE_SERIAL(COperation);

public:
	COperation();									// コンストラクタ
	virtual ~COperation();							// デストラクタ
	virtual void Serialize(CArchive& ar);

	void SetOperationKind(long);					// オペレーション種別設定
	void SetOnKeyDownParameter(UINT,UINT,UINT,int);	// CView::OnKeyDownパラメータ設定
	void SetOnKeyUpParameter(UINT,UINT,UINT);		// CView::OnKeyUpパラメータ設定
	void SetOnCharParameter(UINT,UINT,UINT);		// CView::OnCharパラメータ設定

	void Do(CEditorView*,CEditorDoc*);				// オペレーション実行
	int  MySerialize(CArchive&);					// シリアライズ
	void GetOperationString(char*);					// オペレーション内容説明テキスト取得

protected:
	void SetOperationString(UINT,UINT,UINT);		// オペレーション内容説明テキスト設定

private:
	CEditorApp*	m_pApp;								// アプリケーションクラスへのポインタ
	char	m_szOperationString[256];				// オペレーション内容を説明するテキスト
	long	m_lOperationKind;						// オペレーション種別

	//CView::OnKeyDown関連パラメータ
	UINT	m_nChar_OnKeyDown;
	UINT	m_nRepCnt_OnKeyDown;
	UINT	m_nFlags_OnKeyDown;
	int		m_nMultValueOfCaretAccelarete;

	//CView::OnKeyUp関連パラメータ
	UINT	m_nChar_OnKeyUp;
	UINT	m_nRepCnt_OnKeyUp;
	UINT	m_nFlags_OnKeyUp;

	//CView::OnChar関連パラメータ
	UINT	m_nChar_OnChar;
	UINT	m_nRepCnt_OnChar;
	UINT	m_nFlags_OnChar;
};

#endif // !defined(Operation_H)

