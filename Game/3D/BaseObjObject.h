#pragma once

#include "../../Engine/3D/ObjModelObject.h"
#include "../../Engine/base/ObjModelManager.h"

#include "../Collision/CollisionSystem/CollisionInfo.h"


class BaseCollider;

class BaseObjObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseObjObject() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseObjObject();


	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize(std::string filePath, bool IsSmmothing = false);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera"></param>
	/// <param name="matBillboard">ビルボード開始</param>
	virtual void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 後処理
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& info){};

	//Getter
	inline const Vector3& GetPosition()	{return world.translation;}
	inline const Vector3& GetRotation()	{return world.rotation;}
	inline const Vector3& GetScale()		{return world.scale;}
	inline const WorldTransform& GetmatWorld()	{return world;}
	inline const ObjModelObject* GetObjObject()	{return object;}

	//Setter
	inline void SetPosition(const Vector3& position)	{world.translation = position;}
	inline void SetRotation(const Vector3& rotation)	{world.rotation = rotation;}
	inline void SetScale(const Vector3& scale)			{world.scale = scale;}
	void SetCollider(BaseCollider* collider);
	inline void SetObject(ObjModelObject* object)	{this->object = object;}
	inline void SetModel(ObjModelManager* model)	{this->model = model;}

protected:
	//クラス名(デバック用)
	const char* name = nullptr;

	ObjModelManager* model;
	ObjModelObject* object;
	WorldTransform world;

	//コライダー
	BaseCollider* collider = nullptr;

	Camera* camera = nullptr;
};

