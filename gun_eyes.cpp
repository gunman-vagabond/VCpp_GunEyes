//
// 最初のDirect3Dアプリケーション
//

#include <windows.h>
#include <d3drmwin.h>
#include <math.h>
#include "setupwin.h"

static BOOL bQuit=FALSE;

// Direct3DRMオブジェクト
LPDIRECT3DRMFRAME lpD3DRMFrameObject;
LPDIRECT3DRMMESHBUILDER lpD3DRMMeshObject;


//
// シーンの構築
//
BOOL CreateScene() {
	lpD3DRMScene->SetSceneBackground( D3DRGB(0.7, 0.7, 0.7) );
	//メッシュの読み込み
	lpD3DRM->CreateMeshBuilder(&lpD3DRMMeshObject);
	lpD3DRMMeshObject->Load("gun_face.x",NULL,D3DRMLOAD_FROMFILE,NULL,NULL);
//	lpD3DRMMeshObject->SetColorRGB(D3DVAL(1.0),D3DVAL(1.0),D3DVAL(1.0));
	//物体用フレームの作成
	lpD3DRM->CreateFrame(lpD3DRMScene,&lpD3DRMFrameObject);
	lpD3DRMFrameObject->SetPosition(lpD3DRMScene,D3DVAL(0),D3DVAL(0),D3DVAL(0));
	lpD3DRMFrameObject->AddVisual(lpD3DRMMeshObject);
	//回転の設定
//	lpD3DRMFrameObject->SetRotation(lpD3DRMScene,D3DVAL(0),D3DVAL(1.0),D3DVAL(0),D3DVAL(-3.14159/50));

	//光源の作成
	LPDIRECT3DRMFRAME light=NULL;
	LPDIRECT3DRMLIGHT light1=NULL; //特定方向からの光
	lpD3DRM->CreateFrame(lpD3DRMScene,&light);
	lpD3DRM->CreateLightRGB(D3DRMLIGHT_DIRECTIONAL,D3DVAL(1.0),D3DVAL(1.0),D3DVAL(1.0),&light1);
	light->AddLight(light1);
	light->SetPosition(lpD3DRMScene,D3DVAL(2.0),D3DVAL(2.0),D3DVAL(-1.0));
	light->SetOrientation(lpD3DRMScene,
		D3DVAL(-1.0),D3DVAL(-1.0),D3DVAL(3.0),D3DVAL(0.0),D3DVAL(1.0),D3DVAL(0.0));
	LPDIRECT3DRMLIGHT light2=NULL; //環境光
	lpD3DRM->CreateLightRGB(D3DRMLIGHT_AMBIENT,D3DVAL(0.7),D3DVAL(0.7),D3DVAL(0.7),&light2);
	lpD3DRMScene->AddLight(light2);
	RELEASE(light);
	RELEASE(light1);
	RELEASE(light2);
	
	LPDIRECT3DRMFRAME lpLookat=NULL;
	lpD3DRM->CreateFrame(lpD3DRMScene,&lpLookat);
	lpLookat->SetPosition(lpD3DRMScene,D3DVAL(0),D3DVAL(0),D3DVAL(0));

	//カメラの配置
	lpD3DRMCamera->SetPosition(lpD3DRMScene,D3DVAL(0),D3DVAL(0),D3DVAL(-1.5));
	lpD3DRMCamera->LookAt(lpLookat,lpD3DRMScene,D3DRMCONSTRAIN_Z);
//	lpD3DRMCamera->LookAt(lpD3DRMFrameObject,lpD3DRMScene,D3DRMCONSTRAIN_Z);
	return TRUE;
}


//
// WindowProc関数
//
LONG FAR PASCAL WindowProc(HWND win, UINT msg, WPARAM wparam, LPARAM lparam) {
	RECT rc;
	PAINTSTRUCT ps;
	LPDIRECT3DRMWINDEVICE lpD3DRMWinDev;
	POINT pt;
	RECT rect;
	int center_x, center_y, mouce_x, mouce_y;

	switch (msg) {
		case WM_DESTROY:
			CleanUp();
			bQuit=TRUE;
			return TRUE;
		case WM_CREATE:
			SetTimer(win, 12345, 10, NULL);
			return TRUE;
		case WM_TIMER:
			GetCursorPos(&pt);
			mouce_x = pt.x;
			mouce_y = pt.y;

			GetWindowRect(win,&rect);
			center_x = (rect.left + rect.right)/2;
			center_y = (rect.top + rect.bottom)/2;

			lpD3DRMFrameObject->SetOrientation(lpD3DRMScene,
				D3DVAL(center_x - mouce_x),D3DVAL(mouce_y - center_y),D3DVAL(50),
				D3DVAL(0),D3DVAL(1),D3DVAL(0));
			return TRUE;
		case WM_ACTIVATE:
			if (!lpD3DRMDev) break;
			lpD3DRMDev->QueryInterface(IID_IDirect3DRMWinDevice,(void **)&lpD3DRMWinDev);
			lpD3DRMWinDev->HandleActivate((WORD)wparam);
			lpD3DRMWinDev->Release();
			return TRUE;
		case WM_PAINT:
			if (!lpD3DRMDev) break;
			if (GetUpdateRect(win,&rc,FALSE)) {
				BeginPaint(win,&ps);
				lpD3DRMDev->QueryInterface(IID_IDirect3DRMWinDevice,(void **) &lpD3DRMWinDev);
				lpD3DRMWinDev->HandlePaint(ps.hdc);
				lpD3DRMWinDev->Release();
				EndPaint(win,&ps);
			}
			return TRUE;
	}
	return DefWindowProc(win, msg, wparam, lparam);
}


//
// WinMain関数
//
int PASCAL WinMain(HINSTANCE hi, HINSTANCE prev, LPSTR cmd, int cmdshow) {
	MSG msg;

	// ウィンドウの作成とDirect3DRMの初期化
	if (!InitApp(hi,cmdshow,"Gun-Eyes",100,100)) {
		CleanUp();
		return 1;
	}	

	//
	// メインループ
	//
	while (!bQuit) {
		// メッセージキュー内のメッセージの有無を確認
		while (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
			if (!TranslateAccelerator(msg.hwnd,NULL,&msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		// レンダリング
		if (!bQuit) Render();
	}
	return msg.wParam;
}
