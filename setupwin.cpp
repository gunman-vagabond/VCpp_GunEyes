//
// ウィンドウベースのDirect3DRMアプリケーションをセットアップするモジュール
//


// 初めに「#define INITGUID」が必要
#define INITGUID
#include <windows.h>
#include <malloc.h>
#include <d3drmwin.h>
#include "setupwin.h"
#define MAX_DRIVERS 5

// DirectDraw関連
static LPDIRECTDRAWCLIPPER lpDDC=NULL;

// Direct3D Retained Mode関連
LPDIRECT3DRM lpD3DRM=NULL;
LPDIRECT3DRMDEVICE lpD3DRMDev=NULL;     // Direct3DRMデバイス
LPDIRECT3DRMVIEWPORT lpD3DRMView=NULL;  // Direct3DRMビューポート
LPDIRECT3DRMFRAME lpD3DRMScene=NULL;    // シーンフレーム
LPDIRECT3DRMFRAME lpD3DRMCamera=NULL;   // カメラフレーム

// Direct3Dドライバ関連
static GUID DriverGUID[MAX_DRIVERS];     // GUID
static char DriverName[MAX_DRIVERS][50]; // ドライバ名 
static int  NumDrivers;                  // ドライバ数
static int  CurrDriver;                  // 使用中のドライバ

// その他
static BOOL bInitialized;
static int BPP;


// 
// BPP(bits per pixel)をDirectDrawの形式に直す
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
// Direct3Dドライバを検索するためのコールバック関数
//
static HRESULT WINAPI enumDeviceFunc(
	LPGUID lpGuid, LPSTR lpDeviceDescription,
	LPSTR lpDeviceName, LPD3DDEVICEDESC lpHWDesc,
	LPD3DDEVICEDESC lpHELDesc, LPVOID lpContext) {
	static BOOL hardware=FALSE;
	static BOOL mono=FALSE;
	LPD3DDEVICEDESC lpDesc;
	int *lpStartDriver = (int *)lpContext;
	
	// ハードウェアもしくはエミュレーションの選択
	lpDesc=lpHWDesc->dcmColorModel ? lpHWDesc : lpHELDesc;
	// 指定した色数に対応していなければスキップ
	if (!(lpDesc->dwDeviceRenderBitDepth & BPPToDDBD(BPP))) return D3DENUMRET_OK;
	// ドライバ名とGUIDを記録する
	memcpy(&DriverGUID[NumDrivers],lpGuid,sizeof(GUID));
	lstrcpy(&DriverName[NumDrivers][0],lpDeviceName);
	// ソフトウェアよりもハードウェアを、mono光源よりもRGB光源を優先する
	if (*lpStartDriver==-1) {
		// 有効なドライバを初めて発見したとき
		*lpStartDriver=NumDrivers;
		hardware= (lpDesc==lpHWDesc)? TRUE : FALSE;
		mono= (lpDesc->dcmColorModel & D3DCOLOR_MONO)? TRUE : FALSE;

//#if 0
	} else if (lpDesc==lpHWDesc&&!hardware) {
		// 現在処理中のドライバはハードウェアで、
		// 発見済みのドライバはハードウェアでないとき
		*lpStartDriver=NumDrivers;
		hardware=(lpDesc==lpHWDesc)? TRUE : FALSE;
		mono=(lpDesc->dcmColorModel & D3DCOLOR_MONO)? TRUE : FALSE;
	} else if ((lpDesc==lpHWDesc && hardware )||(lpDesc==lpHELDesc && !hardware)) {
		if (lpDesc->dcmColorModel == D3DCOLOR_MONO && !mono) {
		// 現在処理中のドライバも発見済みのドライバも同じタイプであり、
		// ただし現在処理中のドライバはmono光源で、発見済みのドライバはそうでないとき
			*lpStartDriver=NumDrivers;
			hardware=(lpDesc == lpHWDesc)? TRUE : FALSE;
			mono=(lpDesc->dcmColorModel & D3DCOLOR_MONO)? TRUE : FALSE;
		}
	}
//#endif

#if 0
	} else if (lpDesc!=lpHWDesc&&hardware) {
		// 現在処理中のドライバはハードウェアではなく、
		// 発見済みのドライバはハードウェアのとき
		//我が家のHALデバイスが不調のためこうする。
		*lpStartDriver=NumDrivers;
		hardware=(lpDesc==lpHWDesc)? TRUE : FALSE;
		mono=(lpDesc->dcmColorModel & D3DCOLOR_MONO)? TRUE : FALSE;
	} else if ((lpDesc!=lpHWDesc && !hardware )||(lpDesc!=lpHELDesc && hardware)) {
		if (lpDesc->dcmColorModel != D3DCOLOR_MONO && mono) {
		// 現在処理中のドライバも発見済みのドライバも同じタイプであり、
		// ただし現在処理中のドライバはmono光源で、発見済みのドライバはそうでないとき
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
// 使用可能なDirect3Dドライバを検出する
//
static BOOL EnumDrivers() {
	LPDIRECTDRAW lpDD;
	LPDIRECT3D lpD3D;
	
	// DirectDrawオブジェクトを生成し、Direct3Dインタフェースを検索する
	DirectDrawCreate(NULL,&lpDD,NULL);
	if (lpDD->QueryInterface(IID_IDirect3D,(void**)&lpD3D)!=DD_OK) {
		lpDD->Release();
		return FALSE;
	}
	// ドライバを検出する
	CurrDriver=-1;
	lpD3D->EnumDevices(enumDeviceFunc,&CurrDriver);
	// 終了
	lpD3D->Release();
	lpDD->Release();
	if (NumDrivers==0) return FALSE;
	return TRUE;
}

static BOOL FindDrivers() {
	LPDIRECTDRAW lpDD;
	LPDIRECT3D lpD3D;
	
	// DirectDrawオブジェクトを生成し、Direct3Dインタフェースを検索する
	DirectDrawCreate(NULL,&lpDD,NULL);
	if (lpDD->QueryInterface(IID_IDirect3D,(void**)&lpD3D)!=DD_OK) {
		lpDD->Release();
		return FALSE;
	}

	//		検索用構造体を準備
	D3DFINDDEVICESEARCH		fs;
	ZeroMemory( &fs,sizeof(D3DFINDDEVICESEARCH) );
	fs.dwSize		= sizeof(D3DFINDDEVICESEARCH);
	fs.dwFlags		= D3DFDS_HARDWARE;
	fs.bHardware	= FALSE;
//	fs.dwFlags      = D3DFDS_COLORMODEL;
//	fs.dcmColorModel = D3DCOLOR_RGB;

	//		検索結果
	D3DFINDDEVICERESULT		fr;
	ZeroMemory( &fr,sizeof(D3DFINDDEVICERESULT) );
	fr.dwSize	= sizeof(D3DFINDDEVICERESULT);

	//		検索
	HRESULT			hr;
	hr	= lpD3D->FindDevice( &fs,&fr );
	lpD3D->Release();
	lpDD->Release();
	if( hr != D3D_OK )
		return FALSE;

	//		結果を格納する
	memcpy(&DriverGUID[0],&(fr.guid),sizeof(GUID));
	CurrDriver = 0;
	NumDrivers = 1;
	return	TRUE;

}


//
// デバイスとビューを作成
//
static BOOL CreateDevAndView(int driver, int width, int height) {
	// Direct3D Retained Modeオブジェクトを作成
	long err;
	if (err=lpD3DRM->CreateDeviceFromClipper(lpDDC,&DriverGUID[driver],width,height,&lpD3DRMDev)!=D3DRM_OK)
		return FALSE;

	// ビューポートを作成
	width=lpD3DRMDev->GetWidth();
	height=lpD3DRMDev->GetHeight();
	if (lpD3DRM->CreateViewport(lpD3DRMDev,lpD3DRMCamera,0,0,width,height,&lpD3DRMView)!=D3DRM_OK)
		return FALSE;
	if (lpD3DRMView->SetBack(D3DVAL(5000.0))!=D3DRM_OK) return FALSE;
	// レンダリング品質の設定
	if (lpD3DRMDev->SetQuality(D3DRMLIGHT_ON | D3DRMFILL_SOLID | D3DRMSHADE_GOURAUD)!=D3DRM_OK)
		return FALSE;
	// SetDitherやSetTextureQualityを行う場合はここで。
	// 表示色数に応じてシェーティングの設定を行う
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
// ウィンドウの作成とDirect3DRMの初期化
//
BOOL InitApp(HINSTANCE hi, int cmdshow, char name[], int width, int height) {
	// ウィンドウの作成
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

		CW_USEDEFAULT,CW_USEDEFAULT, //ウィンドウの左上座標
		width,height,                     //ウィンドウの幅と高さ
		NULL,NULL,hi,NULL);
	if (!win) return FALSE;

	// 色数を取得
	HDC hdc;
	hdc=GetDC(win);
	BPP=GetDeviceCaps(hdc,BITSPIXEL);
	ReleaseDC(win,hdc);

	// Direct3Dドライバを選択
	if (!EnumDrivers()) {
		CleanUp();
		return FALSE;
	}

//	// Direct3Dドライバの検索
//	if (!FindDrivers()) {
//		CleanUp();
//		return FALSE;
//	}

	// Direct3D Retained Modeオブジェクトを生成
	lpD3DRM = NULL;
	Direct3DRMCreate(&lpD3DRM);

	// シーンフレームとカメラフレームを作成
	lpD3DRM->CreateFrame(NULL,&lpD3DRMScene);
	lpD3DRM->CreateFrame(lpD3DRMScene,&lpD3DRMCamera);
//	lpD3DRMCamera->SetPosition(lpD3DRMScene,D3DVAL(0.0),D3DVAL(0.0),D3DVAL(0.0));

	// DirectDrawClipperオブジェクトを生成
	DirectDrawCreateClipper(0,&lpDDC,NULL);
	lpDDC->SetHWnd(0,win);

	// Direct3D Retained Modeデバイスを生成
	RECT rc;
	GetClientRect(win, &rc);
	if (!CreateDevAndView(CurrDriver,rc.right,rc.bottom)) {
		CleanUp();
		return FALSE;
	}

	// シーンの生成
	if (!CreateScene()) {
		CleanUp();
		return FALSE;
	}

	// ウィンドウを表示
	bInitialized=TRUE;
	ShowWindow(win,cmdshow);
	UpdateWindow(win);
	return TRUE;
}


//
// レンダリング
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
// 使用した変数の後片付け
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
