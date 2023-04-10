#pragma once
#include "BaseObjObject.h"
#include "SphereCollider.h"

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
	void Update(Camera* camera) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision(const CollisionInfo &info) override;

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();


private:
	//武器出現
	bool IsAppear = false;

	//生存時間(フレーム)
	float aliveCurrentTime = 0;
	const float AliveTime = 0.2f;
};

