#pragma once
#include "../BaseObjObject.h"
#include "../../Collision/SphereCollider.h"

class PlayerWeapon : public BaseObjObject
{
//メンバ関数
public:
	~PlayerWeapon();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision(const CollisionInfo &info);

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	//Getter
	bool GetIsEnemyContact()	{return IsEnemyContact;}

	//Setter
	void SetIsEnemyContact(bool IsFlag)	{IsEnemyContact = IsFlag;}

private:
	//武器出現
	bool IsAppear = false;

	//敵破壊
	bool IsEnemyContact = false;

	//生存時間(フレーム)
	int aliveCurrentFrame = 0;
	const int AliveFrame = 10;

	//コライダー
	SphereCollider* sphereCollider = nullptr;
};

