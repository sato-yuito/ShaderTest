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
    transform_.position_ = { 0,2,0 };
    transform_.rotate_ = { 0,0,0 };
    transform_.scale_ = { 1,1,1 };
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
