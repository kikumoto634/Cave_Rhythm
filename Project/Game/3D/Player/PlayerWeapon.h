#pragma once
#include "BaseObjObject.h"
#include "SphereCollider.h"

class PlayerWeapon : public BaseObjObject
{
private:
	//コライダー
	const float ColliderRadius = 0.6f;
	const DirectX::XMVECTOR ColliderOffSet = {0,ColliderRadius,0,0};

	//非表示時の位置
	const Vector3 HidePos = {0.f,-10.f,0.f};

	//生存フレーム
	const float AliveTime = 0.2f;

	//出現から消滅までのサイズ変化
	const Vector3 StartSize = {1,1,1};
	const Vector3 EndSize = {1,1,1};

//メンバ関数
public:

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
	/// 衝突時コールバック関数
	/// </summary>
	void OnCollision(const CollisionInfo &info) override;

	/// <summary>
	/// 攻撃
	/// </summary>
	inline void Attack()	{isAppear_ = true;}


private:
	//武器出現
	bool isAppear_ = false;

	//生存時間(フレーム)
	float aliveCurrentTime_ = 0;
};

