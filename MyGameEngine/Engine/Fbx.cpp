#include <assert.h>
#include "Fbx.h"
#include "Direct3D.h"
#include "Camera.h"
#include "Texture.h"

Fbx::Fbx()
{
}

HRESULT Fbx::Load(std::string fileName)
{
	//マネージャを生成
	FbxManager* pFbxManager = FbxManager::Create();

	//インポーターを生成
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//シーンオブジェクトにFBXファイルの情報を流し込む
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//メッシュ情報を取得
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	//各情報の個数を取得
	vertexCount_ = mesh->GetControlPointsCount();	//頂点の数
	polygonCount_ = mesh->GetPolygonCount();	//ポリゴンの数
	materialCount_ = pNode->GetMaterialCount();

	//現在のカレントディレクトリを取得
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	//引数のfileNameからディレクトリ部分を取得
	char dir[MAX_PATH];
	_splitpath_s(fileName.c_str(), nullptr, 0, dir, MAX_PATH, nullptr, 0, nullptr, 0);

	//カレントディレクトリ変更
	SetCurrentDirectory(dir);

	InitVertex(mesh);		//頂点バッファ準備
	InitIndex(mesh);		//インデックスバッファ準備
	IntConstantBuffer();	//コンスタントバッファ準備
	InitMaterial(pNode);//マテリアルの準備

	//カレントディレクトリを元に戻す
	SetCurrentDirectory(defaultCurrentDir);

	//マネージャ解放
	pFbxManager->Destroy();

	pToonTex_ = new Texture;
	pToonTex_->Load("Assets\\toon.png");
	return S_OK;
}

//頂点バッファ準備
void Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
{
	//頂点情報を入れる配列
	VERTEX* vertices = new VERTEX[vertexCount_];

	//全ポリゴン
	for (DWORD poly = 0; poly < polygonCount_; poly++)
	{
		//3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//調べる頂点の番号
			int index = mesh->GetPolygonVertex(poly, vertex);

			//頂点の位置
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertices[index].position = XMVectorSet((float)pos[0], (float)pos[1], (float)pos[2], 0.0f);

			//頂点のUV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertices[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);

			//頂点の法線
			FbxVector4 Normal;
			mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//ｉ番目のポリゴンの、ｊ番目の頂点の法線をゲット
			vertices[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], (float)Normal[2], 0.0f);
		}
	}
	FbxGeometryElementTangent* t = mesh->GetElementTangent(0);
	//tangent取得
	for (int poly = 0; poly < polygonCount_; poly++) 
	{
	
		FbxVector4 tangent{ 0,0,0,0 };
		int sIndex = mesh->GetPolygonVertexIndex(poly);
		if (t)
		{
			tangent = t->GetDirectArray().GetAt(sIndex).mData;
		}
		for (int j = 0; j < 3; j++)
		{
			int index = mesh->GetPolygonVertices()[sIndex + j];
			vertices[index].tangent = XMVectorSet((float)tangent[0], (float)tangent[1], (float)tangent[2],0.0f);
		}
	}


	//頂点バッファ
	HRESULT hr;
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
	}
}

//インデックスバッファ準備
void Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer * [materialCount_];
	indexCount_ = std::vector<int>(materialCount_);

	
	int* index = new int[polygonCount_ * 3];
	

	for (int i = 0; i < materialCount_; i++)
	{

		int count = 0;

		//全ポリゴン
		for (DWORD poly = 0; poly < polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);

			if (mtlId == i)
			{
				//3頂点分
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					index[count] = mesh->GetPolygonVertex(poly, vertex);
					count++;
				}
			}
			
		}
		indexCount_[i] = count;

		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * polygonCount_ * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = index;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		HRESULT hr;
		hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, "インデックスバッファの作成に失敗しました", "エラー", MB_OK);
		}
	}
}

void Fbx::IntConstantBuffer()
{
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DEFAULT;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = 0;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	HRESULT hr;
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
	}
}

void Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_ = new MATERIAL[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		//i番目のマテリアル情報を取得
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);

		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pMaterial;

		FbxDouble3 diffuse = pPhong->Diffuse;

		FbxDouble3  ambient = pPhong->Ambient;

		pMaterialList_[i].diffuse = XMFLOAT4{ (float)diffuse[0],(float)diffuse[1] ,(float)diffuse[2] ,1.0f };
		pMaterialList_[i].ambient = XMFLOAT4{ (float)ambient[0],(float)ambient[1] ,(float)ambient[2] ,1.0f };
		pMaterialList_[i].specular = XMFLOAT4(0, 0, 0, 0);
		pMaterialList_[i].shininess = 1;

		if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			//Mayaで取得したSpecularの情報
			FbxDouble3 specular = pPhong->Specular;
			pMaterialList_[i].specular = XMFLOAT4{ (float)specular[0],(float)specular[1] ,(float)specular[2] ,1.0f };

			FbxDouble shininess = pPhong->Shininess;
			pMaterialList_[i].shininess = (float)shininess;
		}
		//通常テクスチャ
		{
			//テクスチャ情報
			FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

			//テクスチャの数数
			int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

			//テクスチャあり
			if (fileTextureCount)
			{
				FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
				const char* textureFilePath = textureInfo->GetRelativeFileName();

				//ファイル名+拡張だけにする
				char name[_MAX_FNAME];	//ファイル名
				char ext[_MAX_EXT];	//拡張子
				_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				//ファイルからテクスチャ作成
				pMaterialList_[i].pTexture = new Texture;
				HRESULT hr = pMaterialList_[i].pTexture->Load(name);
				if (hr == S_OK);
			}

			//テクスチャ無し
			else
			{
				pMaterialList_[i].pTexture = nullptr;
			}
		}
		//ノーマルマップ用
		{

			//テクスチャ情報
			FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);

			//テクスチャの数数
			int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

			//テクスチャあり
			if (fileTextureCount)
			{
				FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
				const char* textureFilePath = textureInfo->GetRelativeFileName();

				//ファイル名+拡張だけにする
				char name[_MAX_FNAME];	//ファイル名
				char ext[_MAX_EXT];	//拡張子
				_splitpath_s(textureFilePath, nullptr, 0, nullptr, 0, name, _MAX_FNAME, ext, _MAX_EXT);
				wsprintf(name, "%s%s", name, ext);

				//ファイルからテクスチャ作成
				pMaterialList_[i].pNormalTexture = new Texture;
				HRESULT hr = pMaterialList_[i].pNormalTexture->Load(name);
				if (hr == S_OK);
			}

			//テクスチャ無し
			else
			{
				pMaterialList_[i].pNormalTexture = nullptr;

				//マテリアルの色
			}
		}
	}
}

void Fbx::Draw(Transform& transform)
{
	
	Direct3D::SetShader(SHADER_NORMALMAP);

	transform.Calclation();//トランスフォームを計算
    for (int i = 0; i < materialCount_; i++)
  {
	  //コンスタントバッファに情報を渡す
	  CONSTANT_BUFFER cb;
	  cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	  cb.matW = XMMatrixTranspose(transform.GetWorldMatrix());
	  cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());
	  
	  cb.ambientColor = pMaterialList_[i].ambient;
	  cb.speculerColor = pMaterialList_[i].specular;
	  cb.shininess = pMaterialList_[i].shininess;
	  cb.diffuseColor = pMaterialList_[i].diffuse;
	
	  cb.isTexture = pMaterialList_[i].pTexture != nullptr;
	  cb.hasNormalMap = pMaterialList_[i].pNormalTexture != nullptr;
	
	 Direct3D::pContext_->UpdateSubresource(pConstantBuffer_, 0,NULL,&cb,0,0);	//再開

	  //頂点バッファ、インデックスバッファ、コンスタントバッファをパイプラインにセット
	  //頂点バッファ
	   UINT stride = sizeof(VERTEX);
	   UINT offset = 0;
	   Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

		// インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		//コンスタントバッファ
		Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
		Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

		if (pMaterialList_[i].pTexture)
		{
			ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
			Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);
			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
			Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
		}
		if (pMaterialList_[i].pNormalTexture)
		{
			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pNormalTexture->GetSRV();
			Direct3D::pContext_->PSSetShaderResources(1, 1, &pSRV);
		}
		

		//描画
		Direct3D::pContext_->DrawIndexed(indexCount_[i], 0, 0);

		//Direct3D::SetShader(SHADER_TOON);

   }
}

void Fbx::Release()
{
}