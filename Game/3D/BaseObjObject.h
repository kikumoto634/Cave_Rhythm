#pragma once

#include "../../Engine/3D/ObjModelObject.h"
#include "../../Engine/base/ObjModelManager.h"

#include "../Collision/CollisionSystem/CollisionInfo.h"


class BaseCollider;

class BaseObjObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	BaseObjObject() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~BaseObjObject();


	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize(std::string filePath, bool IsSmmothing = false);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="matBillboard">�r���{�[�h�J�n</param>
	virtual void Update(Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// �㏈��
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	virtual void OnCollision(const CollisionInfo& info){};

	//Getter
	const Vector3& GetPosition()	{return world.translation;}
	const Vector3& GetRotation()	{return world.rotation;}
	const WorldTransform& GetmatWorld()	{return world;}
	const ObjModelObject* GetObjObject()	{return object;}

	//Setter
	void SetPosition(const Vector3& position)	{world.translation = position;}
	void SetRotation(const Vector3& rotation)	{world.rotation = rotation;}
	void SetCollider(BaseCollider* collider);
	void SetObject(ObjModelObject* object)	{this->object = object;}
	void SetModel(ObjModelManager* model)	{this->model = model;}

protected:
	//�N���X��(�f�o�b�N�p)
	const char* name = nullptr;

	ObjModelManager* model;
	ObjModelObject* object;
	WorldTransform world;

	//�R���C�_�[
	BaseCollider* collider = nullptr;

	Camera* camera = nullptr;
};

