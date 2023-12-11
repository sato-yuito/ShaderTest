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
    //���f���f�[�^�̃��[�h
    hModel_ = Model::Load("Assets/donuts.fbx");
    assert(hModel_ >= 0);
    transform_.position_.y = 5.2f;
}

void Ball::Update()
{
    transform_.rotate_.y += 0.5f;
}

void Ball::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void Ball::Release()
{
}
