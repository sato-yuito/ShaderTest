#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"

namespace
{
    const XMFLOAT4 DEF_LightPos = { 1,2,1,0 };
}
void Stage::InitConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CBUFF_STAGESCENE);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// �R���X�^���g�o�b�t�@�̍쐬
	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pCBStageScene_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
	}
}
//�R���X�g���N�^
Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage"), hModel_(-1),lightSorcePosition(DEF_LightPos)
{
}

//�f�X�g���N�^
Stage::~Stage()
{
}


//������
void Stage::Initialize()
{
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("assets/Ground.fbx");
    assert(hModel_ >= 0);
	InitConstantBuffer();
}

//�X�V
void Stage::Update()
{
	CBUFF_STAGESCENE cb;
	cb.lightPos = lightSorcePosition;
	XMStoreFloat4(&cb.eyePos, Camera::GetEyePosition());
	Direct3D::pContext_->UpdateSubresource(pCBStageScene_, 0, NULL, &cb, 0, 0);

	Direct3D::pContext_->VSSetConstantBuffers(1, 1, &pCBStageScene_);
	Direct3D::pContext_->PSSetConstantBuffers(1, 1, &pCBStageScene_);
}

//�`��
void Stage::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Stage::Release()
{
}
