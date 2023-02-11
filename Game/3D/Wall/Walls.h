#pragma once
#include "../BaseObjObject.h"
#include "../../Collision/MeshCollider.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

class Walls : public BaseObjObject
{
public:
	~Walls();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	void Initialize(std::string filePath, bool IsSmmothing = false);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	void IsNotAlive()	{IsAlive = false;}

	//Getter
	inline bool GetIsAlive()	{return IsAlive;}
	inline bool GetIsDigSound()	{return IsDigSound;}

	//Setter
	inline void SetPlayerPos(Vector3 pos)	{PlayerPos = pos;}

private:
	Vector3 PlayerPos;
	float distance = 0;

	bool IsDigSound = false;

	//エリアで使用しない
	bool IsAlive = true;
	//距離に応じた非表示
	bool IsHide = false;

	//メッシュコライダー用モデル
	ObjModelManager* colliderModel = nullptr;
};

