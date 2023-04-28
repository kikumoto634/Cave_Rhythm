#pragma once
#include "BaseObjObject.h"

//前方宣言
class SkeltonStateManager;

class Skelton : public BaseObjObject
{
public:
	//フレンド関数
	friend class IdelSkeltonState;

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

private:
	//アクション更新
	void ActionUpdate();

	//ビート更新
	void BeatUpdate();

	//経路探索(A*アルゴリズム)
	void PathSearch();

	//コライダー更新
	void ColliderUpdate();

private:
	//状態
	SkeltonStateManager* state_ = nullptr;

	//死亡
	bool isDead_ = false;

	//移動
	//イージングの移動開始座標
	Vector3 easigStartPos_ = {};
	//イージングの移動終了座標
	Vector3 easingEndPos_ = {};
	//イージングの移動タイム
	float easingMoveTime_ = 0;
	//移動ベクトル
	Vector3 addVector3_;

	//コライダー
	//SphereCollider* sphereCollider_ = nullptr;
	//レイキャスト
	//Ray ray_;
	Vector3 rayCastDir_ = {0,0,-1};
};