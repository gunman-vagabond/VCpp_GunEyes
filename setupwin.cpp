//
// �E�B���h�E�x�[�X��Direct3DRM�A�v���P�[�V�������Z�b�g�A�b�v���郂�W���[��
//


// ���߂Ɂu#define INITGUID�v���K�v
#define INITGUID
#include <windows.h>
#include <malloc.h>
#include <d3drmwin.h>
#include "setupwin.h"
#define MAX_DRIVERS 5

// DirectDraw�֘A
static LPDIRECTDRAWCLIPPER lpDDC=NULL;

// Direct3D Retained Mode�֘A
LPDIRECT3DRM lpD3DRM=NULL;
LPDIRECT3DRMDEVICE lpD3DRMDev=NULL;     // Direct3DRM�f�o�C�X
LPDIRECT3DRMVIEWPORT lpD3DRMView=NULL;  // Direct3DRM�r���[�|�[�g
LPDIRECT3DRMFRAME lpD3DRMScene=NULL;    // �V�[���t���[��
LPDIRECT3DRMFRAME lpD3DRMCamera=NULL;   // �J�����t���[��

// Direct3D�h���C�o�֘A
static GUID DriverGUID[MAX_DRIVERS];     // GUID
static char DriverName[MAX_DRIVERS][50]; // �h���C�o�� 
static int  NumDrivers;                  // �h���C�o��
static int  CurrDriver;                  // �g�p���̃h���C�o

// ���̑�
static BOOL bInitialized;
static int BPP;


// 
// BPP(bits per pixel)��DirectDraw�̌`���ɒ���
//
static DWORD BPPToDDBD(int bpp) {
	switch(bpp) {
		case 1: return DDBD_1;
		case 2: return DDBD_2;
		case 4: return DDBD_4;
		case 8: return DDBD_8;
		case 16: return DDBD_16;
		case 24: return DDBD_24;
		case 32: return DDBD_32;
		default: return 0;
	}
}



//
// Direct3D�h���C�o���������邽�߂̃R�[���o�b�N�֐�
//
static HRESULT WINAPI enumDeviceFunc(
	LPGUID lpGuid, LPSTR lpDeviceDescription,
	LPSTR lpDeviceName, LPD3DDEVICEDESC lpHWDesc,
	LPD3DDEVICEDESC lpHELDesc, LPVOID lpContext) {
	static BOOL hardware=FALSE;
	static BOOL mono=FALSE;
	LPD3DDEVICEDESC lpDesc;
	int *lpStartDriver = (int *)lpContext;
	
	// �n�[�h�E�F�A�������̓G�~�����[�V�����̑I��
	lpDesc=lpHWDesc->dcmColorModel ? lpHWDesc : lpHELDesc;
	// �w�肵���F���ɑΉ����Ă��Ȃ���΃X�L�b�v
	if (!(lpDesc->dwDeviceRenderBitDepth & BPPToDDBD(BPP))) return D3DENUMRET_OK;
	// �h���C�o����GUID���L�^����
	memcpy(&DriverGUID[NumDrivers],lpGuid,sizeof(GUID));
	lstrcpy(&DriverName[NumDrivers][0],lpDeviceName);
	// �\�t�g�E�F�A�����n�[�h�E�F�A���Amono��������RGB������D�悷��
	if (*lpStartDriver==-1) {
		// �L���ȃh���C�o�����߂Ĕ��������Ƃ�
		*lpStartDriver=NumDrivers;
		hardware= (lpDesc==lpHWDesc)? TRUE : FALSE;
		mono= (lpDesc->dcmColorModel & D3DCOLOR_MONO)? TRUE : FALSE;

//#if 0
	} else if (lpDesc==lpHWDesc&&!hardware) {
		// ���ݏ������̃h���C�o�̓n�[�h�E�F�A�ŁA
		// �����ς݂̃h���C�o�̓n�[�h�E�F�A�łȂ��Ƃ�
		*lpStartDriver=NumDrivers;
		hardware=(lpDesc==lpHWDesc)? TRUE : FALSE;
		mono=(lpDesc->dcmColorModel & D3DCOLOR_MONO)? TRUE : FALSE;
	} else if ((lpDesc==lpHWDesc && hardware )||(lpDesc==lpHELDesc && !hardware)) {
		if (lpDesc->dcmColorModel == D3DCOLOR_MONO && !mono) {
		// ���ݏ������̃h���C�o�������ς݂̃h���C�o�������^�C�v�ł���A
		// ���������ݏ������̃h���C�o��mono�����ŁA�����ς݂̃h���C�o�͂����łȂ��Ƃ�
			*lpStartDriver=NumDrivers;
			hardware=(lpDesc == lpHWDesc)? TRUE : FALSE;
			mono=(lpDesc->dcmColorModel & D3DCOLOR_MONO)? TRUE : FALSE;
		}
	}
//#endif

#if 0
	} else if (lpDesc!=lpHWDesc&&hardware) {
		// ���ݏ������̃h���C�o�̓n�[�h�E�F�A�ł͂Ȃ��A
		// �����ς݂̃h���C�o�̓n�[�h�E�F�A�̂Ƃ�
		//�䂪�Ƃ�HAL�f�o�C�X���s���̂��߂�������B
		*lpStartDriver=NumDrivers;
		hardware=(lpDesc==lpHWDesc)? TRUE : FALSE;
		mono=(lpDesc->dcmColorModel & D3DCOLOR_MONO)? TRUE : FALSE;
	} else if ((lpDesc!=lpHWDesc && !hardware )||(lpDesc!=lpHELDesc && hardware)) {
		if (lpDesc->dcmColorModel != D3DCOLOR_MONO && mono) {
		// ���ݏ������̃h���C�o�������ς݂̃h���C�o�������^�C�v�ł���A
		// ���������ݏ������̃h���C�o��mono�����ŁA�����ς݂̃h���C�o�͂����łȂ��Ƃ�
			*lpStartDriver=NumDrivers;
			hardware=(lpDesc == lpHWDesc)? TRUE : FALSE;
			mono=(lpDesc->dcmColorModel & D3DCOLOR_MONO)? TRUE : FALSE;
		}
	}
#endif

	NumDrivers++;
	if (NumDrivers==MAX_DRIVERS)
		return (D3DENUMRET_CANCEL);
	return (D3DENUMRET_OK);
}


//
// �g�p�\��Direct3D�h���C�o�����o����
//
static BOOL EnumDrivers() {
	LPDIRECTDRAW lpDD;
	LPDIRECT3D lpD3D;
	
	// DirectDraw�I�u�W�F�N�g�𐶐����ADirect3D�C���^�t�F�[�X����������
	DirectDrawCreate(NULL,&lpDD,NULL);
	if (lpDD->QueryInterface(IID_IDirect3D,(void**)&lpD3D)!=DD_OK) {
		lpDD->Release();
		return FALSE;
	}
	// �h���C�o�����o����
	CurrDriver=-1;
	lpD3D->EnumDevices(enumDeviceFunc,&CurrDriver);
	// �I��
	lpD3D->Release();
	lpDD->Release();
	if (NumDrivers==0) return FALSE;
	return TRUE;
}

static BOOL FindDrivers() {
	LPDIRECTDRAW lpDD;
	LPDIRECT3D lpD3D;
	
	// DirectDraw�I�u�W�F�N�g�𐶐����ADirect3D�C���^�t�F�[�X����������
	DirectDrawCreate(NULL,&lpDD,NULL);
	if (lpDD->QueryInterface(IID_IDirect3D,(void**)&lpD3D)!=DD_OK) {
		lpDD->Release();
		return FALSE;
	}

	//		�����p�\���̂�����
	D3DFINDDEVICESEARCH		fs;
	ZeroMemory( &fs,sizeof(D3DFINDDEVICESEARCH) );
	fs.dwSize		= sizeof(D3DFINDDEVICESEARCH);
	fs.dwFlags		= D3DFDS_HARDWARE;
	fs.bHardware	= FALSE;
//	fs.dwFlags      = D3DFDS_COLORMODEL;
//	fs.dcmColorModel = D3DCOLOR_RGB;

	//		��������
	D3DFINDDEVICERESULT		fr;
	ZeroMemory( &fr,sizeof(D3DFINDDEVICERESULT) );
	fr.dwSize	= sizeof(D3DFINDDEVICERESULT);

	//		����
	HRESULT			hr;
	hr	= lpD3D->FindDevice( &fs,&fr );
	lpD3D->Release();
	lpDD->Release();
	if( hr != D3D_OK )
		return FALSE;

	//		���ʂ��i�[����
	memcpy(&DriverGUID[0],&(fr.guid),sizeof(GUID));
	CurrDriver = 0;
	NumDrivers = 1;
	return	TRUE;

}


//
// �f�o�C�X�ƃr���[���쐬
//
static BOOL CreateDevAndView(int driver, int width, int height) {
	// Direct3D Retained Mode�I�u�W�F�N�g���쐬
	long err;
	if (err=lpD3DRM->CreateDeviceFromClipper(lpDDC,&DriverGUID[driver],width,height,&lpD3DRMDev)!=D3DRM_OK)
		return FALSE;

	// �r���[�|�[�g���쐬
	width=lpD3DRMDev->GetWidth();
	height=lpD3DRMDev->GetHeight();
	if (lpD3DRM->CreateViewport(lpD3DRMDev,lpD3DRMCamera,0,0,width,height,&lpD3DRMView)!=D3DRM_OK)
		return FALSE;
	if (lpD3DRMView->SetBack(D3DVAL(5000.0))!=D3DRM_OK) return FALSE;
	// �����_�����O�i���̐ݒ�
	if (lpD3DRMDev->SetQuality(D3DRMLIGHT_ON | D3DRMFILL_SOLID | D3DRMSHADE_GOURAUD)!=D3DRM_OK)
		return FALSE;
	// SetDither��SetTextureQuality���s���ꍇ�͂����ŁB
	// �\���F���ɉ����ăV�F�[�e�B���O�̐ݒ���s��
	switch (BPP) {
		case 1:
			if (lpD3DRMDev->SetShades(4)) return FALSE;
			if (lpD3DRM->SetDefaultTextureShades(4)) return FALSE;
			break;
		case 16:
			if (lpD3DRMDev->SetShades(32)) return FALSE;
			if (lpD3DRM->SetDefaultTextureColors(64)) return FALSE;
			if (lpD3DRM->SetDefaultTextureShades(32)) return FALSE;
			break;
		case 24:
		case 32:
			if (lpD3DRMDev->SetShades(256)) return FALSE;
			if (lpD3DRM->SetDefaultTextureColors(64)) return FALSE;
			if (lpD3DRM->SetDefaultTextureShades(256)) return FALSE;
			break;
	}
	return TRUE;
}


//
// �E�B���h�E�̍쐬��Direct3DRM�̏�����
//
BOOL InitApp(HINSTANCE hi, int cmdshow, char name[], int width, int height) {
	// �E�B���h�E�̍쐬
	WNDCLASS wc;
	wc.style=CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc=WindowProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=sizeof(DWORD);
	wc.hInstance=hi;
//	wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wc.hIcon=LoadIcon(hi,"GUN_ICON");
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
//	wc.hbrBackground=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
	wc.lpszMenuName=NULL;
	wc.lpszClassName=name;
	if (!RegisterClass(&wc)) return 1;
	HWND win=CreateWindow(
		name,name,
		WS_VISIBLE|WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU
//		|WS_MINIMIZEBOX|WS_MAXIMIZEBOX
		,

//		WS_POPUP|WS_VISIBLE,

//		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT,CW_USEDEFAULT, //�E�B���h�E�̍�����W
		width,height,                     //�E�B���h�E�̕��ƍ���
		NULL,NULL,hi,NULL);
	if (!win) return FALSE;

	// �F�����擾
	HDC hdc;
	hdc=GetDC(win);
	BPP=GetDeviceCaps(hdc,BITSPIXEL);
	ReleaseDC(win,hdc);

	// Direct3D�h���C�o��I��
	if (!EnumDrivers()) {
		CleanUp();
		return FALSE;
	}

//	// Direct3D�h���C�o�̌���
//	if (!FindDrivers()) {
//		CleanUp();
//		return FALSE;
//	}

	// Direct3D Retained Mode�I�u�W�F�N�g�𐶐�
	lpD3DRM = NULL;
	Direct3DRMCreate(&lpD3DRM);

	// �V�[���t���[���ƃJ�����t���[�����쐬
	lpD3DRM->CreateFrame(NULL,&lpD3DRMScene);
	lpD3DRM->CreateFrame(lpD3DRMScene,&lpD3DRMCamera);
//	lpD3DRMCamera->SetPosition(lpD3DRMScene,D3DVAL(0.0),D3DVAL(0.0),D3DVAL(0.0));

	// DirectDrawClipper�I�u�W�F�N�g�𐶐�
	DirectDrawCreateClipper(0,&lpDDC,NULL);
	lpDDC->SetHWnd(0,win);

	// Direct3D Retained Mode�f�o�C�X�𐶐�
	RECT rc;
	GetClientRect(win, &rc);
	if (!CreateDevAndView(CurrDriver,rc.right,rc.bottom)) {
		CleanUp();
		return FALSE;
	}

	// �V�[���̐���
	if (!CreateScene()) {
		CleanUp();
		return FALSE;
	}

	// �E�B���h�E��\��
	bInitialized=TRUE;
	ShowWindow(win,cmdshow);
	UpdateWindow(win);
	return TRUE;
}


//
// �����_�����O
//
BOOL Render() {
	if (!bInitialized) return FALSE;
	if (lpD3DRMScene->Move(D3DVAL(1.0))!=D3DRM_OK) return FALSE;
    if (lpD3DRMView->Clear()!=D3DRM_OK) return FALSE;
    if (lpD3DRMView->Render(lpD3DRMScene)!=D3DRM_OK) return FALSE;
    if (lpD3DRMDev->Update()!=D3DRM_OK) return FALSE;
    return TRUE;
}


//
// �g�p�����ϐ��̌�Еt��
//
void CleanUp() {
	bInitialized=FALSE;
	RELEASE(lpD3DRMScene);
	RELEASE(lpD3DRMCamera);
	RELEASE(lpD3DRMView);
	RELEASE(lpD3DRMDev);
	RELEASE(lpD3DRM);
	RELEASE(lpDDC);
}
