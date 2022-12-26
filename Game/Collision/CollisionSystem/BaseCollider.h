#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "../../3D/BaseObjObject.h"

/// <summary>
/// �R���C�_�[���N���X
/// </summary>
class BaseCollider
{
public:
	friend class CollisionManager;

public:
	BaseCollider() = default;
	//���z�f�X�g���N�^
	virtual ~BaseCollider() = default;

public:
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	inline void OnCollision(const CollisionInfo& info){
		objObject->OnCollision(info);
	}


	//�����蔻�葮��
	inline void SetAttribute(unsigned short attribute)	{this->attribute = attribute;}
	inline unsigned short GetAttribute()	{return attribute;}
	inline void AddAttribute(unsigned short attribute)	{this->attribute |= attribute;}
	inline void RemoveAttribute(unsigned short attribute)	{this->attribute &= !attribute;}


	//Getter
	inline BaseObjObject* GetObjObject()	{return objObject;}
	inline CollisionShapeType GetShapeType()	{return shapeType;}

	//Setter
	inline void SetObjObject(BaseObjObject* objObject)	{this->objObject = objObject;}

protected:
	//�I�u�W�F�N�g
	BaseObjObject* objObject = nullptr;
	//�`��^�C�v
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	//�����蔻�葮��
	unsigned short attribute = 0b1111111111111111;
};