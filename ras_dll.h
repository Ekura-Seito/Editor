/*[0]-----------------------------------------------------------------------------[0]*/
/*|	�啪��		�F�I�[�g�_�C�A�����C�u����											|*/
/*|	������		�F�q�`�r�ڑ�														|*/
/*|	�t�@�C����	�Fras_dll.h															|*/
/*|	TAB			�F4																	|*/
/*|	Create		�F1998.12.05														|*/
/*|	Update		�F2000.04.24														|*/
/*|	Author		�FS.Ekura															|*/
/*|	���L		�Frasapi32.lib �������N���邱�ƁB									|*/
/*[0]-----------------------------------------------------------------------------[0]*/

#if !defined(RASDLL_H)							//�Q�d�C���N���[�h�h�~
#define RASDLL_H

//�֐��v���g�^�C�v�錾�i���J�֐��j
#ifdef __cplusplus
extern "C" {
#endif
LONG	__declspec( dllexport )rasConnect(char*,char*,char*,char*,char*);//�q�`�r�ڑ�����
LONG	__declspec( dllexport )rasIsConnected(LPDWORD);					 //�q�`�r�ڑ������`�F�b�N
void	__declspec( dllexport )rasDisConnect();							 //�q�`�r�ؒf����
#ifdef __cplusplus
};
#endif

#endif // !defined(RASDLL_H)					//�Q�d�C���N���[�h�h�~

