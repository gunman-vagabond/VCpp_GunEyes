//
// �E�B���h�E�x�[�X��Direct3DRM�A�v���P�[�V�������Z�b�g�A�b�v���郂�W���[��
//

// �O���[�o���ϐ�
extern LPDIRECT3DRM lpD3DRM;             // Direct3DRM�C���^�t�F�[�X
extern LPDIRECT3DRMDEVICE lpD3DRMDev;    // Direct3DRM�f�o�C�X
extern LPDIRECT3DRMVIEWPORT lpD3DRMView; // Direct3DRM�r���[�|�[�g
extern LPDIRECT3DRMFRAME lpD3DRMScene;   // �V�[���t���[��
extern LPDIRECT3DRMFRAME lpD3DRMCamera;  // �J�����t���[��

// �}�N��
#define RELEASE(p) if (p!=NULL) {p->Release(); p=NULL;}

// �E�B���h�E�̍쐬��Direct3DRM�̏�����
BOOL InitApp(HINSTANCE hi, int cmdshow, char name[], int width, int height);
// �����_�����O
BOOL Render();
// �g�p�����ϐ��̌�Еt��
void CleanUp();
// Windows���b�Z�[�W�n���h��
LONG FAR PASCAL WindowProc(HWND win, UINT msg, WPARAM wparam, LPARAM lparam);
// �V�[���̍\�z
BOOL CreateScene();
