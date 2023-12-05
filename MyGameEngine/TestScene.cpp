#include "TestScene.h"
#include"Engine/Input.h"
#include"Engine/SceneManager.h"
#include"Stage.h"
#include"Ball.h"
#include"Arrow.h"
#include"Engine/Camera.h"
TestScene::TestScene(GameObject* parent)
	:GameObject(parent,"TestScene")
{
}

void TestScene::Initialize()
{
	Instantiate<Stage>(this);
	Instantiate<Ball>(this);
	Instantiate<Arrow>(this);

	Camera::SetPosition(XMFLOAT3(7.5, 10, -5));
}

void TestScene::Update()
{
	
	
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
