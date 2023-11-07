#pragma once
#include"Fbx.h"
#include"Transform.h"
#include"Texture.h"
#include<string>
#include<vector>

using std::string;
using std::vector;

namespace Model
{
	struct ModelData
	{
		//FBX
		Fbx *fbx_;

		//トランスフォーム
		Transform transform_;
		//ファイル名
		string filename_;
	};

	int Load(string filename);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);
	void Release();
};

