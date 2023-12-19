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
