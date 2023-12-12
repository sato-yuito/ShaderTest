#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include"Engine/Input.h"
namespace
{
    const XMFLOAT4 DEF_LightPos = { 1,2,1,0 };
}
void Stage::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CBUFF_STAGESCENE);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBStageScene_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
	}
}
//コンストラクタ
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), hModel_(-1),lightSorcePosition(DEF_LightPos)
{
}

//デストラクタ
Stage::~Stage()
{
}


//初期化
void Stage::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("assets/Ground.fbx");
    assert(hModel_ >= 0);
	hLightBall_ = Model::Load("assets/LightBall.fbx");
	assert(hLightBall_ >= 0);

	Camera::SetPosition(XMVECTOR{ 0, 0, -5, 0 });
	Camera::SetTarget(XMVECTOR{ 0, 0, 0, 0 });

	trLightBall.position_ = { 0, 0, 0 };
	trLightBall.rotate_ = { 0, 0, 0 };
	trLightBall.scale_ = { 0.4, 0.4, 0.4 };

	InitConstantBuffer();
}

//更新
void Stage::Update()
{

	if (Input::IsKey(DIK_W))
	{
		XMFLOAT4 p = GetLightPos();
		XMFLOAT4 margin{ p.x - 0.0f, p.y - 0.0f, p.z + 0.1f, p.w + 0.0f };

	
		SetLightPos(margin);
	}

	if (Input::IsKey(DIK_S))
	{
		XMFLOAT4 p = GetLightPos();
		XMFLOAT4 margin{ p.x - 0.0f, p.y - 0.0f, p.z - 0.1f, p.w - 0.0f };

		//Model::GetModel(hModel_)->SetLightPos(margin);
		SetLightPos(margin);
	}
	XMFLOAT4 tmp{ GetLightPos() };
	trLightBall.position_ = { tmp.x, tmp.y,tmp.z };


	CBUFF_STAGESCENE cb;
	cb.lightPos = lightSorcePosition;
	XMStoreFloat4(&cb.eyePos, Camera::GetEyePosition());
	Direct3D::pContext_->UpdateSubresource(pCBStageScene_, 0, NULL, &cb, 0, 0);

	Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStageScene_);
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStageScene_);
}

//描画
void Stage::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
	Model::SetTransform(hLightBall_, trLightBall);
	Model::Draw(hLightBall_);
}

//開放
void Stage::Release()
{
}
