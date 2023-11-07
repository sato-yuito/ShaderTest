#include "RootJob.h"
//#include"../PlayScene.h"
#include"SceneManager.h"
RootJob::RootJob(GameObject*parent)
	:GameObject(parent,"Rootjob")
{
}

RootJob::~RootJob()
{
}

void RootJob::Initialize()
{
	Instantiate<SceneManager>(this);
	//Instantiate<PlayScene>(this);
}

void RootJob::Update()
{
}

void RootJob::Draw()
{
	/*for (auto& e : childList_)
		e->Draw();*/
}

void RootJob::Release()
{
}
