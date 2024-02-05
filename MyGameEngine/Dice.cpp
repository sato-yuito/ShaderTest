#include "Dice.h"
#include "Engine/Model.h"
Dice::Dice(GameObject* parent) :GameObject(parent, "Dice"), hModel_(-1)
{
}

Dice::~Dice()
{
}

void Dice::Initialize()
{
	/*hModel_ = Model::Load("Assets/Dice.fbx");
	assert(hModel_ >= 0);
	transform_.position_  = { 0,3,0 };*/
}

void Dice::Update()
{
}

void Dice::Draw()
{
	/*Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);*/
}

void Dice::Release()
{
}
