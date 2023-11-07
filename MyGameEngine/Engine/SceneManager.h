#pragma once
#include "GameObject.h"

//ゲームのシーンを列挙
enum SCENE_ID
{
    SCENE_ID_TEST = 0,
    SCENE_ID_PLAY,
};
class SceneManager :
    public GameObject
{
    SCENE_ID currentScneID_;
    SCENE_ID nextSceneID_;
public:
    SceneManager(GameObject* parent);
    ~SceneManager();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release()override;
    
    //シーンを切り替える
    void ChangeScene(SCENE_ID _next);
};
