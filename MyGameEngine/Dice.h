#pragma once

#include "Engine/GameObject.h"

class Dice : public GameObject
{
    int hModel_;    //モデル番号
public:
    //コンストラクタ
    Dice(GameObject* parent);

    //デストラクタ
    ~Dice();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};

