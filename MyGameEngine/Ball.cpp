#include "Ball.h"
#include "Engine/Model.h"
#include"Engine/Input.h"
Ball::Ball(GameObject* parent) :GameObject(parent, "Ball"), hModel_(-1)
{

}

Ball::~Ball()
{
}

void Ball::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/donuts.fbx");
    assert(hModel_ >= 0);
    transform_.position_.y = 5.2f;
}

void Ball::Update()
{
    if (Input::IsKey(DIK_LEFT))
    {
        transform_.rotate_.y -= 0.5f;
    }

    if (Input::IsKey(DIK_RIGHT))
    {
        transform_.rotate_.y += 0.5f;
    }
    
}

void Ball::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Ball::Release()
{
}
