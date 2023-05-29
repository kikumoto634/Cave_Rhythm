#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "MapNode.h"
#include <vector>

#include "AreaManager.h"

class BossStateManager;
class Boss1;

class BossState
{
public:
	virtual void SetStateManager(BossStateManager* stateManager)	{stateManager_ = stateManager;}

	void Initialize(Boss1* boss);
	virtual void UpdateTrigger() = 0;
	virtual void Update() = 0;
	virtual void ParticleDraw() = 0;

protected:
	BossStateManager* stateManager_ = nullptr;
	Boss1* boss_ = nullptr;
};

//待機
class IdelBossState : public BossState{
private:
	void UpdateTrigger() override;
	void Update() override;
	void ParticleDraw() override;

private:
	const int WaitCountMax = 2;
	int waitCount_ = 0;
};

//召喚
class SummonBossState : public BossState{
private:
	void UpdateTrigger() override;
	void Update() override;
	void ParticleDraw() override;
};

//追跡
class TrackBossState : public BossState{
private:
	//エリアブロック数の半分
	const int AreaBlocksHalfNum = AreaManager::DIV_NUM_HALF;
	//エリアブロックサイズ
	const float AreaBlockSize = AreaManager::Block_Size;

	//マップ開始番号
	const int RootPathStartNumber = 9;
	//マップスケルトン番号
	const int RootPathSkeltonNumber = 5;
	//マッププレイヤー番号
	const int RootPathPlayerNumber = 6;
	//マップ プレイヤー到達時番号
	const int RootPathGoalNumber = 10;

	//移動方向数
	static const int PathDirection = 4;
	//角度
	const float LeftAngle  = DirectX::XMConvertToRadians(-90);
	const float RightAngle = DirectX::XMConvertToRadians(+90);
	const float DownAngle  = 0;
	const float UpAngle    = DirectX::XMConvertToRadians(+180);

	//イージング完了フレーム
	const float EasingFrameMax = 1.0f;
	//イージング移動時間
	const float EasingMoveTimeMax = 0.05f;


//エイリアス
private:
	template <class T> using vector = std::vector<T>;

private:
	void UpdateTrigger() override;
	void Update() override;
	void ParticleDraw() override;

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

//退避
class RunAwayBossState : public BossState{
private:
	void UpdateTrigger() override;
	void Update() override;
	void ParticleDraw() override;
};

//死亡
class DeadBossState : public BossState{
private:
	//パーティクル生存時間
	const int ParticleAliveFrameMax = 50;

	//パーティクル生成
	const size_t CreateNum = 10;
	//速度
	const float Rand_Vel = 0.08f;
	const float Rand_Vel_Half = 0.04f;
	const float VelY = 0.06f;
	//加速度
	const float AccY = -0.005f;
	//サイズ
	const float SizeStart = 0.4f;
	const float SizeEnd = 0.0f;
	//テクスチャ番号
	const uint32_t TextureNumber = 1;
	//色
	Vector4 Color = {1.0f,0.0f,0.0f,1.0f};

private:
	void UpdateTrigger() override;
	void Update() override;
	void ParticleDraw() override;

	void App();

private:
	//パーティクル生成
	//座標
	Vector3 pos{};
	//速度
	Vector3 vel{};
	//加速度
	Vector3 acc{};
};
