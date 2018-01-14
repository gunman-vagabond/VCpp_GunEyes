//
// ウィンドウベースのDirect3DRMアプリケーションをセットアップするモジュール
//

// グローバル変数
extern LPDIRECT3DRM lpD3DRM;             // Direct3DRMインタフェース
extern LPDIRECT3DRMDEVICE lpD3DRMDev;    // Direct3DRMデバイス
extern LPDIRECT3DRMVIEWPORT lpD3DRMView; // Direct3DRMビューポート
extern LPDIRECT3DRMFRAME lpD3DRMScene;   // シーンフレーム
extern LPDIRECT3DRMFRAME lpD3DRMCamera;  // カメラフレーム

// マクロ
#define RELEASE(p) if (p!=NULL) {p->Release(); p=NULL;}

// ウィンドウの作成とDirect3DRMの初期化
BOOL InitApp(HINSTANCE hi, int cmdshow, char name[], int width, int height);
// レンダリング
BOOL Render();
// 使用した変数の後片付け
void CleanUp();
// Windowsメッセージハンドラ
LONG FAR PASCAL WindowProc(HWND win, UINT msg, WPARAM wparam, LPARAM lparam);
// シーンの構築
BOOL CreateScene();
