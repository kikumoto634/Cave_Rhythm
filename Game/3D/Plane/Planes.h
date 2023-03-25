#pragma once
#include "../BaseObjObject.h"
#include "../../Collision/MeshCollider.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

class Planes : public BaseObjObject
{
private:
	//描画範囲
	const int DrawingRange = 15;

public:
	~Planes();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	void Initialize(ObjModelManager* model);

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

	/// <summary>
	/// 色変化
	/// </summary>
	void PlaneColorChange(bool IsSwitch,  bool IsColorChange);
	inline void PlaneColorReset()	{object->SetColor({1.f,1.f,1.f,1.f});}

	inline void IsNotAlive()	{IsAlive = false;}

	//Getter
	inline bool GetIsPlayerContact()	{return IsPlayerContact;}

	//Setter
	inline void SetIsPlayerContact(bool IsFlag)	{IsPlayerContact = IsFlag;}
	inline void SetPlayerPos(Vector3 pos)	{PlayerPos = pos;}

private:
	bool IsPlayerContact = false;

	Vector3 PlayerPos;
	float distance = 0;

	//エリアで使用しない
	bool IsAlive = true;
	//距離に応じた非表示
	bool IsHide = false;

	bool IsCollision = false;
};

