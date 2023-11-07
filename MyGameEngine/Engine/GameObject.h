#pragma once

#include"Transform.h"
#include <list>
#include <string>

class   SphereCollider;

using std::string;
class GameObject
{
	bool dead_; //消去フラグ

protected:
	std::list<GameObject*>	childList_;
	Transform				transform_;
	GameObject*				pParent_;
	std::string				objectName_;
	SphereCollider*         pCollider_;

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	virtual void Initialize()	= 0;
	virtual void Update()  = 0;
	virtual void Draw()  = 0;
	virtual void Release()  = 0;
	
	void KillMe();
	
	void DrawSub();
	
	void UpdateSub();
	
	void ReleaseSub();
	
	GameObject* FindChildObject(string _objName);
	
	GameObject* GetRootJob();
	
	GameObject* FindObject(string _objName);

	
	void AddCollider(SphereCollider* pCollider);

	void Collision(GameObject* pTarget);

	void RoundRobin(GameObject* pTarget);

	virtual void onCollision(GameObject* pTarget){}

	template <class T>
	GameObject* Instantiate(GameObject* parent)
	{
		T* p;
		p = new T(parent);
		p->Initialize();
		parent->childList_.push_back(p);
		return p;
	}

	//アクセス関数
	XMFLOAT3 GetPosition() { return transform_.position_; }
	XMFLOAT3 GetRotate() { return transform_.rotate_; }
	XMFLOAT3 GetScale() { return transform_.scale_; }
	void SetPosition(XMFLOAT3 position) { transform_.position_ = position; }
	void SetPosition(float x, float y, float z) { SetPosition(XMFLOAT3(x, y, z)); }
	void SetRotate(XMFLOAT3 rotate) { transform_.rotate_ = rotate; }
	void SetRotate(float x, float y, float z) { SetRotate(XMFLOAT3(x, y, z)); }
	void SetRotateX(float x) { SetRotate(x, transform_.rotate_.y, transform_.rotate_.z); }
	void SetRotateY(float y) { SetRotate(transform_.rotate_.x, y, transform_.rotate_.z); }
	void SetRotateZ(float z) { SetRotate(transform_.rotate_.x, transform_.rotate_.y, z); }
	void SetScale(XMFLOAT3 scale) { transform_.scale_ = scale; }
	void SetScale(float x, float y, float z) { SetScale(XMFLOAT3(x, y, z)); }

};

