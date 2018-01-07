// StatusBarDisp.h ステータスバー表示項目情報の定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(STATUSBARDISP_H)		//２重インクルード防止
#define STATUSBARDISP_H

typedef struct 
{
	int 	m_nCurrentItemCount;
	UINT	m_nCurrentItems[STS_INDICATORS_COUNT+8];

	int		m_nAddItemCount;
	UINT	m_nAddItems[STS_INDICATORS_COUNT+8];

} STATUSBARDISP, *LPSTATUSBARDISP;

#endif // !defined(STATUSBARDISP_H)

