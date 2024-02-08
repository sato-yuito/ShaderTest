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
	//�}�e���A��
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
		XMMATRIX	matWVP;//���[���h�r���[�v���W�F�N�V�����̍s���n���Ă���ϐ�
		XMMATRIX    matW;//���[���h�ϊ��̂�
		XMMATRIX	matNormal;//�X�P�[��*���s�s��̋t�s��
		XMFLOAT4	diffuseColor;		// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
		XMFLOAT4    ambientColor;
		XMFLOAT4    speculerColor;
		float       scrollX;
		float       scrollY;
		float       shininess;
		BOOL 	    isTexture;		// �e�N�X�`���\���Ă��邩�ǂ���
		BOOL        hasNormalMap;
	};

	struct VERTEX
	{
		XMVECTOR position;//�ʒu
		XMVECTOR uv;//�e�N�X�`�����W
		XMVECTOR normal;//�@���x�N�g��
		XMVECTOR tangent;//�ڐ�
	};

	

	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S���̐�
	int materialCount_;	//�}�e���A���̌�

	float scrollValX_;
	float scrollValY_;

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

