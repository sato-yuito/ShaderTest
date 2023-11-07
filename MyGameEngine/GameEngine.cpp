#include <Windows.h>
#include <stdlib.h>
#include"Engine/Direct3D.h"
#include"Engine/Camera.h"
#include"Engine/Input.h"
#include"Engine/RootJob.h"
#include"Engine/Model.h"
#pragma comment(lib, "winmm.lib")

//定数宣言
const char* WIN_CLASS_NAME = "SampleGame";  //ウィンドウクラス名
const char* MENU_BAR_NAME = "Let's make game";
const int WINDOW_WIDTH = 800;  //ウィンドウの幅
const int WINDOW_HEIGHT = 600; //ウィンドウの高さ

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

RootJob * pRootJob = nullptr;

//エントリーポイント
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{  
	//ウィンドウクラス（設計図）を作成
   WNDCLASSEX wc;
   wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
   wc.hInstance = hInstance;                   //インスタンスハンドル
   wc.lpszClassName = WIN_CLASS_NAME;            //ウィンドウクラス名
   wc.lpszMenuName = MENU_BAR_NAME;
   wc.lpfnWndProc = WndProc;                   //ウィンドウプロシージャ
   wc.style = CS_VREDRAW | CS_HREDRAW;         //スタイル（デフォルト）
   wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //アイコン
   wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //小さいアイコン
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //マウスカーソル
   wc.lpszMenuName = NULL;                     //メニュー（なし）
   wc.cbClsExtra = 0;
   wc.cbWndExtra = 0;
   wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）
   RegisterClassEx(&wc);  //クラスを登録

    //ウィンドウサイズの計算
    RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
    int winW = winRect.right - winRect.left;     //ウィンドウ幅
    int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ

    //ウィンドウを作成
    HWND hWnd = CreateWindow(
        WIN_CLASS_NAME,         //ウィンドウクラス名
        MENU_BAR_NAME,     //タイトルバーに表示する内容
        WS_OVERLAPPEDWINDOW , //スタイル（普通のウィンドウ）
        CW_USEDEFAULT,       //表示位置左（おまかせ）
        CW_USEDEFAULT,       //表示位置上（おまかせ）
        winW,              //ウィンドウ幅
        winH,        //ウィンドウ高さ
        NULL,                //親ウインドウ（なし）
        NULL,                //メニュー（なし）
        hInstance,           //インスタンス
        NULL                 //パラメータ（なし）
    );
    //ウィンドウを表示
    ShowWindow(hWnd, nCmdShow);

    //Direct3D初期化
    HRESULT hr;
    hr =Direct3D::Initialize(winW, winH, hWnd);
    if (FAILED(hr))
    {
        PostQuitMessage(0);  //プログラム終了
    }

   Camera::Initialize();
    
   //DirectInputの初期化
   hr =  Input::Initialize(hWnd);
   if (FAILED(hr))
   {
       PostQuitMessage(0);  //プログラム終了
   }

   pRootJob = new RootJob(nullptr);
   pRootJob->Initialize();
    //メッセージループ（何か起きるのを待つ）
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        //メッセージあり
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //メッセージなし
        else
        {
            timeBeginPeriod(1);
            static DWORD countFps = 0;
            static DWORD startTime = timeGetTime();
            DWORD nowTime = timeGetTime();
            static DWORD lastUpdateTime = nowTime;

            if (nowTime - startTime >= 1000)
            {
                char str[16];
                wsprintf(str, "%u", countFps);
                SetWindowText(hWnd, str);

                countFps = 0;
                startTime = nowTime;
            }

            if ((nowTime - lastUpdateTime)*60 <= 1000.0f)
            {
                continue;
            }
            lastUpdateTime = nowTime;

            countFps++;

            timeEndPeriod(1);
            //カメラの処理
            Camera::Update();
            
            //入力情報の更新
            Input::Update();

            pRootJob->UpdateSub();
            //ゲームの処理
            
            //描画処理
            Direct3D::BeginDraw();

            pRootJob->DrawSub();

            Direct3D::EndDraw();
        }
    }

    //それぞれのクラスの開放処理
    pRootJob->ReleaseSub();

    Input::Release();
    
    Direct3D::Release();
    
    Model::Release();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
       case WM_MOUSEMOVE:
       Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
       return 0;
       case WM_DESTROY:
       PostQuitMessage(0);  //プログラム終了
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}