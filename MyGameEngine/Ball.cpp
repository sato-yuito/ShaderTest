#include "Ball.h"
#include "Engine/Model.h"

Ball::Ball(GameObject* parent) :GameObject(parent, "Ball"), hModel_(-1)
{

}

Ball::~Ball()
{
}

void Ball::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets/ball.fbx");
    assert(hModel_ >= 0);
    transform_.position_.y = 1.5f;
}

void Ball::Update()
{
    transform_.rotate_.z += 0.5f;
}

void Ball::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Ball::Release()
{
}
