#pragma once
#include "Vector3.h"
#include "MapNode.h"
#include <queue>
#include <vector>

//前方宣言
class SkeltonStateManager;
class Skelton;

class SkeltonState
{
public:
	virtual void SetStateManager(SkeltonStateManager* stateManager)	{stateManager_ = stateManager;}

	void Initialize(Skelton* skelton);
	virtual void UpdateTrigger() = 0;
	virtual void Update() = 0;

protected:
	//借り物
	SkeltonStateManager* stateManager_ = nullptr;
	Skelton* skelton_ = nullptr;
};

//待機
class IdelSkeltonState : public SkeltonState{
private:
	void UpdateTrigger() override;
	void Update() override;

private:
	//待機カウント最大数
	const int WaltCountMax = 1;

	//待機カウント
	int waitCount_ = 0;
};

//追跡
class TrackSkeltonState : public SkeltonState{
private:
	const float EasingMoveTimeMax = 0.05f;

//エイリアス
private:
	template <class T> using vector = std::vector<T>;

private:
	void UpdateTrigger() override;
	void Update() override;

private:
	//経路探索
	vector<MapNode*> path_;
	vector<vector<int>> mapPath_;

	int pathRoot_ = 10;

	//移動
	//イージングの移動開始座標
	Vector3 easigStartPos_ = {};
	//イージングの移動終了座標
	Vector3 easingEndPos_ = {};
	//イージングの移動タイム
	float easingMoveTime_ = 0;

	int eX_ = 0;
	int eY_ = 0;
	int pX_ = 0;
	int pY_ = 0;
};


//死亡
class DeadSkeltonState : public SkeltonState{
private:
	//パーティクル発生時間
	const int ParticleAliveFrameMax = 50;
	//生成するときのプレイヤーとの距離
	const float PopDistance = 3.f;

private:
	void UpdateTrigger() override;
	void Update() override;

	void App();

private:
	Vector3 deadPos_ = {0,-50,0};
	Vector3 particlePos_ = {};

	int particleAliveFrame = 0;
};

