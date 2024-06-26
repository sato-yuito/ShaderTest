#pragma once
#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include<vector>
#include "Transform.h"
#include"Texture.h"
#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")


class Texture;
class Fbx
{
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		Texture* pNormalTexture;
		XMFLOAT4 diffuse;
		XMFLOAT4 ambient;
		XMFLOAT4 specular;
		float shininess;
	};

	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;//ワールドビュープロジェクションの行列を渡している変数
		XMMATRIX    matW;//ワールド変換のみ
		XMMATRIX	matNormal;//スケール*並行行列の逆行列
		XMFLOAT4	diffuseColor;		// ディフューズカラー（マテリアルの色）
		XMFLOAT4    ambientColor;
		XMFLOAT4    speculerColor;
		float       shininess;
		BOOL 	    isTexture;		// テクスチャ貼ってあるかどうか
		BOOL        hasNormalMap;
	};

	struct VERTEX
	{
		XMVECTOR position;//位置
		XMVECTOR uv;//テクスチャ座標
		XMVECTOR normal;//法線ベクトル
		XMVECTOR tangent;//接線
	};

	

	int vertexCount_;	//超点数
	int polygonCount_;	//ポリゴンの数
	int materialCount_;	//マテリアルの個数


	ID3D11Buffer* pVertexBuffer_;
	ID3D11Buffer** pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;
	MATERIAL* pMaterialList_;
	std::vector<int>indexCount_;
	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void IntConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
	Texture* pToonTex_;
public:

	Fbx();
	HRESULT Load(std::string fileName);
	void    Draw(Transform& transform);
	void    Release();
	
};

