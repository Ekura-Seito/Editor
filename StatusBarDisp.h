// StatusBarDisp.h �X�e�[�^�X�o�[�\�����ڏ��̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(STATUSBARDISP_H)		//�Q�d�C���N���[�h�h�~
#define STATUSBARDISP_H

typedef struct 
{
	int 	m_nCurrentItemCount;
	UINT	m_nCurrentItems[STS_INDICATORS_COUNT+8];

	int		m_nAddItemCount;
	UINT	m_nAddItems[STS_INDICATORS_COUNT+8];

} STATUSBARDISP, *LPSTATUSBARDISP;

#endif // !defined(STATUSBARDISP_H)

