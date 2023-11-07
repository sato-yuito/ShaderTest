#include "Model.h"

#include "Direct3D.h"
namespace Model
{
	vector<ModelData*>modelList;
}

int Model::Load(string filename)
{
	ModelData* pData;
	pData = new ModelData;
	pData->filename_ = filename;
	pData->fbx_ = nullptr;

	for (auto &e : modelList)
	{
		if (e->filename_ == filename)
		{
			pData->fbx_ = e->fbx_;
			break;
		}
			
	}

	
	if (pData->fbx_ == nullptr)
	{
		pData->fbx_ = new Fbx;
		pData->fbx_->Load(filename);
	}
	modelList.push_back(pData);
	return(modelList.size() - 1);
}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList[hModel]->transform_ = transform;
}

void Model::Draw(int hModel)
{
	modelList[hModel]->fbx_->Draw(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReffered = false;
	for (int i = 0; i < modelList.size(); i++)
	{
		for (int j = i + 1; j < modelList.size(); j++)
		{
			if (modelList[i]->fbx_ == modelList[j]->fbx_)
			{
				isReffered = true;
				break;
			}
		}
		if (isReffered == false)
		{
			SAFE_DELETE(modelList[i]->fbx_);
		}
		SAFE_DELETE(modelList[i]);
	}
	modelList.clear();
}
