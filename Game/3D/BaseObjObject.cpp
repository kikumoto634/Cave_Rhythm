#include "BaseObjObject.h"
#include "../Collision/CollisionSystem/BaseCollider.h"
#include "../Collision/CollisionSystem/CollisionManager.h"
#include "../../../Engine/math/Easing/Easing.h"

BaseObjObject::~BaseObjObject()
{
	//if(collider){
	//	//コリジョンマネージャーから登録を解除する
	//	CollisionManager::GetInstance()->RemoveCollider(collider);
	//	delete collider;
	//}
}

void BaseObjObject::Initialize(std::string filePath, bool IsSmmothing)
{
	model = new ObjModelManager();
	model->CreateModel(filePath, IsSmmothing);
	object = ObjModelObject::Create(model);
	world.Initialize();
	world.UpdateMatrix();

	//クラス名の文字列を取得
	name = typeid(*this).name();
}

void BaseObjObject::Initialize(ObjModelManager *model)
{
	IsLendModel = true;
	this->model = model;
	object = ObjModelObject::Create(model);
	world.Initialize();
	world.UpdateMatrix();

	//クラス名の文字列を取得
	name = typeid(*this).name();
}


void BaseObjObject::Update(Camera *camera)
{
	this->camera = camera;
	world.UpdateMatrix();
	object->Update(world, this->camera);

	//当たり判定更新
	if(collider){
		collider->Update();
	}
}

void BaseObjObject::Draw()
{
	object->Draw();
}

void BaseObjObject::Finalize()
{
	if(collider){
		//コリジョンマネージャーから登録を解除する
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}

	if(!IsLendModel){
		delete model;
		model = nullptr;
	}

	delete object;
	object = nullptr;

	world = {};
}

void BaseObjObject::SetPosition(const Vector3 &position)
{
	world.translation = position;
	world.UpdateMatrix();
}

void BaseObjObject::SetCollider(BaseCollider *collider)
{
	collider->SetObjObject(this);
	this->collider = collider;
	//コリジョンマネージャーに登録
	CollisionManager::GetInstance()->AddCollider(collider);
	//行列の更新
	world.UpdateMatrix();
	//コライダーを更新しておく
	collider->Update();
}

bool BaseObjObject::ScaleChange(Vector3 &sizeMax, Vector3 &sizeMin, float &EndTime)
{
	float ease = -(cosf(3.14159265f * scaleCurrentTime) - 1.f)/2.f;
	scale = Easing_Linear_Point2(sizeMin, sizeMax, ease);
	SetScale(scale);

	if(scaleCurrentTime >= 1.0f){
		scale = ScaleMax;
		scaleCurrentTime = 0.f;
		return true;
	}

	scaleCurrentTime += 1.f/(60*EndTime);
	return false;
}
