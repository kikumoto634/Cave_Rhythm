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
	virtual void Initialize(ObjModelManager* model);

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
	/// リズムにあわせてサイズ変更 
	/// </summary>
	/// <param name="sizeMax">最大サイズ</param>
	/// <param name="sizeMin">最小サイズ</param>
	/// <param name="EndTime">終了時間 (x:秒)</param>
	/// <returns></returns>
	bool ScaleChange(Vector3& sizeMax, Vector3& sizeMin, float& EndTime);

protected:
	//クラス名(デバック用)
	const char* name = nullptr;

	bool IsLendModel = false;
	ObjModelManager* model = nullptr;
	ObjModelObject* object = nullptr;
	WorldTransform world = {};

	//コライダー
	BaseCollider* collider = nullptr;

	//カメラ(借り物)
	Camera* camera = nullptr;


	//リズム更新用
	//拍終了フラグ
	bool IsBeatEnd = false;

	//サイズ変更用
	//スケール
	Vector3 scale = {ScaleMax};
	Vector3 ScaleMax = {1, 1, 1};
	Vector3 ScaleMin = {0.7f, 1, 0.7f};
	//現在時間
	float scaleCurrentTime = 0.f;
	//終了時間(x(s/秒))
	float scaleEndTime = 0.25f;
};

