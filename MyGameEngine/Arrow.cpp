#include "Arrow.h"
#include"Engine/Model.h"
Arrow::Arrow(GameObject* parent) :GameObject(parent, "Arrow"), hModel_(-1)
{
}

Arrow::~Arrow()
{
}

void Arrow::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/arrow.fbx");
    assert(hModel_ >= 0);
}

void Arrow::Update()
{
}

void Arrow::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Arrow::Release()
{
}
