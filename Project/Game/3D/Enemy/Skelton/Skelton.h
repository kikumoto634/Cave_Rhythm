#pragma once
#include "BaseObjObject.h"
#include "SphereCollider.h"

#include "MapNode.h"
#include <vector>

#include "ParticleObject.h"

//前方宣言
class SkeltonStateManager;

class Skelton : public BaseObjObject
{
public:
	//フレンド関数
	friend class IdelSkeltonState;
	friend class TrackSkeltonState;
	friend class DeadSkeltonState;
	friend class PopSkeltonState;

//エイリアス
private:
	template <class T> using vector = std::vector<T>;
	template <class T> using unique_ptr = std::unique_ptr<T>;

//定数
private:
	//描画範囲(非表示)
	const int DrawingRange_Not = 11;
	//描画範囲(暗めの表示)
	const int DrawingRange_Half = 7;


public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera, const Vector3& playerPos);

	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// パーティクル描画
	/// </summary>
	void ParticleDraw();

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;


	//出現
	void Pop(Vector3 pos);

	//Getter
	inline bool GetIsDead() {return isDead_;}
	inline bool GetIsPosImposibble_() {return isPosImposibble_;}

	//Setter
	inline void SetMapInfo(vector<vector<int>> info) {mapInfo_ = info;}

private:
	//アクション更新
	void ActionUpdate();

	//ビート更新
	void BeatUpdate();

	//距離に応じた処理
	void DistanceUpdate();


	//経路探索(A*アルゴリズム)
	vector<MapNode*> PathSearch(
		vector<vector<int>>& grid, 
		int start_x, int start_y, 
		int end_x, int end_y);
	//ヒューリスティック推定値計算
	int Heuristic(int x1,int y1, int x2,int y2){
		return abs(x1-x2) + abs(y1-y2);
	}

	//コライダー
	void ColliderInitialize();
	void ColliderSet();
	void ColliderRemove();

	//パーティクル
	void ParticleInitialize();


private:
	//借り物
	//プレイヤー座標
	Vector3 playerPos_;

	//状態
	SkeltonStateManager* state_ = nullptr;

	//生成可能
	bool isPosImposibble_ = true;

	//死亡
	bool isDead_ = false;

	//非表示
	bool isInvisible_ = false;

	//ビート時の拡縮
	bool isScaleChange_ = false;

	//プレイヤーとの距離
	float distance_ = 0.f;
	bool isLightCal = false;

	//経路探索
	vector<vector<int>> mapInfo_;

	//コライダー
	SphereCollider* sphereCollider_ = nullptr;
	float colliderRadius_ = 1.0f;

	//パーティクル
	unique_ptr<ParticleObject> particle_;
};