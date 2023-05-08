﻿#pragma once
#include "BaseObjObject.h"
#include "ParticleObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

class Walls : public BaseObjObject
{
private:
	//描画範囲
	const int DrawingRange = 11;
	//描画範囲(暗めの表示)
	const int DrawingRange_Half = 7;

public:
	~Walls();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	void Initialize(ObjModelManager* model, ObjModelManager* collider);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	inline void IsNotAlive()	{IsAlive = false;}

	inline void CaveLightOn()	{IsCaveLight = true;}
	inline void CaveLightOff()	{IsCaveLight = false;}

	//Getter
	inline bool GetIsAlive()	{return IsAlive;}
	inline bool GetIsDIg()	{return IsDig;}
	inline Vector3 GetDigPosition()	{return digPosition;}

	//Setter
	void SetPlayerPos(Vector3 pos)	{PlayerPos = pos;}

private:
	void ColliderInitialize();
	void ColliderSet();
	void ColliderRemove();

private:
	Vector3 PlayerPos;
	float distance = 0;

	bool IsDig = false;
	Vector3 digPosition = {};

	//エリアで使用しない
	bool IsAlive = true;
	//距離に応じた非表示
	bool IsHide = false;

	//メッシュコライダー用モデル
	ObjModelManager* colliderModel = nullptr;
	bool IsCollision = false;

	//ライティング
	bool IsCaveLight = false;

	MeshCollider* collider = nullptr;
};

