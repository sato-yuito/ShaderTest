#include "Input.h"
namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;//デバイスオブジェクト
	BYTE keyState[256] ;//押されているかどうかの変数を配列で用意
	BYTE prevKeyState[256];    //前フレームでの各キーの状態
	XMVECTOR mousePosition;

	HRESULT Initialize(HWND hWnd)
	{
		HRESULT hr; //エラー処理用
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		hr = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);//デバイスオブジェクトの作成
		hr = pKeyDevice->SetDataFormat(&c_dfDIKeyboard);//デバイスの種類を指定
		hr = pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);//強調レベルの設定
		
		if (FAILED(hr))
		{
			MessageBox(NULL, "デバイスオブジェクトの作成に失敗しました", "エラー", MB_OK);
			
			return hr;
		}
	}

	//キーの状態を取得
	void Update()
	{
		memcpy(prevKeyState,keyState,sizeof(keyState));
		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
	}

	//特定のキーが押されたかどうか
	bool IsKey(int keyCode)
	{
		if (keyState[keyCode]&0x80)
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//今は押してて、前回は押してない
		if (IsKey(keyCode)&&!(prevKeyState[keyCode]&0x80))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//今は押していなくて前回は押した
		if (!IsKey(keyCode) && prevKeyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition = XMVectorSet((float)x, (float)y, 0, 0);
	}

	XMVECTOR GetMousePosition()
	{
		return mousePosition;
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
		SAFE_RELEASE(pKeyDevice);
	}
}