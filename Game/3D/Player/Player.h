#pragma once
#include "../BaseObjObject.h"
#include "../../../Engine/input/Input.h"
#include "../../Collision/SphereCollider.h"

#include "PlayerWeapon.h"

class Player : public BaseObjObject
{
//メンバ関数
public:
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	//Getter
	inline int GetHP()	{return HP;}

	//Setter
	inline void SetHP(int _HP)	{HP = _HP;}

private:
	//移動
	bool Movement();

	//攻撃
	bool Attack();

	//ダメージ
	void Damage();
	//ダメージ更新
	void DamageUpdate();

	//重力
	void GravityFall();

	//コライダー
	//地面接触判定
	void GroundCollider();

private:
	//接地フラグ
	bool IsGround = true;
	//落下ベクトル
	Vector3 fallV;
	
	//HP
	int HP = 5;

	//ダメージ
	bool IsDamage = false;
	//無敵時間(フレーム)
	const int DamageFrame = 120;
	int damageCurrentFrame = 0;

	//入力ビート
	bool IsInputBeat = false;

	//入力
	Input* input = nullptr;

	//コライダー
	SphereCollider* sphereCollider = nullptr;

	//武器
	PlayerWeapon* weapon = nullptr;
	Vector3 offSetWeaponPos = {0,1,2.5};
};

