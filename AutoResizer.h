// AutoResizer.h : CAutoResizer クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(AUTORESIZER_H)
#define AUTORESIZER_H

#define CP_MOVE_HORIZONTAL					1
#define CP_MOVE_VERTICAL					2
#define CP_RESIZE_HORIZONTAL				4
#define CP_RESIZE_VERTICAL					8

// internal structure used to hold all information
// about a CWnd* control
typedef struct tagCONTROLDATA
{
	HWND  hControl;	// HWND's never change; some MFC functions return temporary CWnd *'s
	DWORD dwStyle;		// check the #define's above
} CONTROLDATA, *LPCONTROLDATA;

class CAutoResizer
{
public:
	CAutoResizer(CWnd* pParent = NULL);
	virtual ~CAutoResizer();

public:
	void SetParent(CWnd* pParent);
	void SetParent(CWnd* pParent, int, int);

	BOOL AddControl(CWnd* pControl, const DWORD& dwStyle = CP_MOVE_HORIZONTAL);
	BOOL AddControl(const UINT& unId, const DWORD& dwStyle = CP_MOVE_HORIZONTAL);
	BOOL RemoveControl(CWnd* pControl);
	BOOL RemoveControl(const UINT& unId);
	void ResetControls(void);
	virtual void MoveControls(void);
	void SetNegativeMoves(const BOOL& fNegativeMoves = TRUE);
	BOOL GetNegativeMoves(void) const;

protected:
	virtual void UpdateParentSize(void);

private:
	CWnd*  m_pParent;
	int    m_nOldParentWidth;
	int    m_nOldParentHeight;
	int    m_nOriginalParentWidth;
	int    m_nOriginalParentHeight;
	BOOL   m_fNegativeMoves;

	CObList	m_awndControls;
};

#endif // !defined(AUTORESIZER_H)


