#pragma once
#include "Engine/GameObject.h"
#include <d3d11.h>

struct CBUFF_STAGESCENE
{
    XMFLOAT4 lightPos;
    XMFLOAT4 eyePos;
};

//◆◆◆を管理するクラス
class Stage : public GameObject
{
    int hModel_;    //モデル番号
    int hLightBall_;

    Transform trLightBall;

    ID3D11Buffer* pCBStageScene_;
    void InitConstantBuffer();
    XMFLOAT4 lightSorcePosition;
public:
    //コンストラクタ
    Stage(GameObject* parent);

    //デストラクタ
    ~Stage();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    void SetLightPos(XMFLOAT4& _pos) { lightSorcePosition = _pos; }
    XMFLOAT4 GetLightPos() { return(lightSorcePosition); }
};

