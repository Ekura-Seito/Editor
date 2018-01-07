////////////////////////////////////////////////////////////////////////////////
//
// Name:			sock_dll.h
// Descriptions�F	Windows Socket Access DLL Header File
// Version:			1.0
// Create:			1998.12.07
// Update:			2000.04.24
// Author:			S.Ekura
//
////////////////////////////////////////////////////////////////////////////////

#if !defined(SOCK_DLL_H)							//�Q�d�C���N���[�h�h�~
#define SOCK_DLL_H

//�֐��v���g�^�C�v�錾�i���J�֐��j
#ifdef __cplusplus
extern "C" {
#endif
LONG __declspec( dllexport )GetLocalHostInfo(char*,char*);					// ���[�J���}�V���̃z�X�g���A�h�o�A�h���X�擾
LONG __declspec( dllexport )SockServerOpen(WORD);							// �ʐM����T�[�o�[�I�[�v��
LONG __declspec( dllexport )SockServerSend(LONG,LONG,LPVOID,DWORD,LPDWORD);	// �T�[�o�[�Z���h
LONG __declspec( dllexport )SockServerRecv(LONG,LONG,LPVOID,LPDWORD);		// �T�[�o�[���V�[�u
LONG __declspec( dllexport )SockServerClose(LONG);							// �ʐM����T�[�o�[�N���[�Y
LONG __declspec( dllexport )SockClientOpen(char*,WORD);						// �ʘb����N���C�A���g�I�[�v��
LONG __declspec( dllexport )SockClientSend(LONG,LPVOID,DWORD,LPDWORD);		// �N���C�A���g�Z���h
LONG __declspec( dllexport )SockClientRecv(LONG,LPVOID,LPDWORD);			// �N���C�A���g���V�[�u
LONG __declspec( dllexport )SockClientClose(LONG);							// �ʐM����N���C�A���g�N���[�Y
#ifdef __cplusplus
};
#endif

#endif // !defined(SOCK_DLL_H)				//�Q�d�C���N���[�h�h�~

