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
	virtual void Initialize(ObjModelManager* model);

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

	inline void IsBeatEndOn()	{IsBeatEnd = true;}

	//Getter
	inline const Vector3& GetPosition()	{return world.translation;}
	inline const Vector3& GetRotation()	{return world.rotation;}
	inline const Vector3& GetScale()		{return world.scale;}
	inline WorldTransform& GetmatWorld()	{return world;}
	inline const ObjModelObject* GetObjObject()	{return object;}
	inline const char* GetName()	{return name;}

	//Setter
	void SetPosition(const Vector3& position);
	inline void SetRotation(const Vector3& rotation)	{world.rotation = rotation, world.UpdateMatrix();}
	inline void SetScale(const Vector3& scale)			{world.scale = scale, world.UpdateMatrix();}
	void SetCollider(BaseCollider* collider);
	inline void SetObject(ObjModelObject* object)	{this->object = object;}
	inline void SetModel(ObjModelManager* model)	{this->model = model;}

protected:
	/// <summary>
	/// ���Y���ɂ��킹�ăT�C�Y�ύX 
	/// </summary>
	/// <param name="sizeMax">�ő�T�C�Y</param>
	/// <param name="sizeMin">�ŏ��T�C�Y</param>
	/// <param name="EndTime">�I������ (x:�b)</param>
	/// <returns></returns>
	bool ScaleChange(Vector3& sizeMax, Vector3& sizeMin, float& EndTime);

protected:
	//�N���X��(�f�o�b�N�p)
	const char* name = nullptr;

	bool IsLendModel = false;
	ObjModelManager* model = nullptr;
	ObjModelObject* object = nullptr;
	WorldTransform world = {};

	//�R���C�_�[
	BaseCollider* collider = nullptr;

	//�J����(�؂蕨)
	Camera* camera = nullptr;


	//���Y���X�V�p
	//���I���t���O
	bool IsBeatEnd = false;

	//�T�C�Y�ύX�p
	//�X�P�[��
	Vector3 scale = {ScaleMax};
	Vector3 ScaleMax = {1, 1, 1};
	Vector3 ScaleMin = {0.7f, 1, 0.7f};
	//���ݎ���
	float scaleCurrentTime = 0.f;
	//�I������(x(s/�b))
	float scaleEndTime = 0.25f;
};

