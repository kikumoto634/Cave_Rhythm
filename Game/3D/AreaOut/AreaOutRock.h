#pragma once
#include "../BaseObjObject.h"
#include "../../3D/ParticleObject.h"

class AreaOutRock : public BaseObjObject
{
private:
	const float LowestPoint = -30.f;

public:
	~AreaOutRock();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	void Initialize(std::string filePath, bool IsSmmothing = false);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera) override;

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

private:
	//重力
	void GravityFall();

	//ランダムスポーン
	void RandomSpawn();

	//リスポーンタイム
	void ReSpawnTime();

	//スケールランダム
	void ScaleRandom();

	void CreateParticle();

private:
	//落下ベクトル
	Vector3 fallV;

	//生存
	bool IsAlive = false;

	//スポーンフレーム
	int spawnFame = 200;
	int MaxSpawnFrame = 300;

	//パーティクル
	ParticleObject* PopParticle = nullptr;
};

