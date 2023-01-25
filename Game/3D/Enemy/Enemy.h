#pragma once
#include "../BaseObjObject.h"
#include "../../3D/ParticleObject.h"

class Enemy : public BaseObjObject
{
//定数
private:
	//ビートx回数終了時にPOP
	const int POP_COUNT = 4;
	//死亡後のリセット時間
	const int AppearanceResetFrame = 50;

//メンバ変数
public:
	~Enemy();

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
	/// パーティクル描画
	/// </summary>
	void ParticleDraw();

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision(const CollisionInfo& info) override;

	//出現
	void BeginAppearance();
	//出現予定地点セット
	void SetPopPoasition(Vector3 pos)	{popPosition = pos;}

	//Getter
	bool GetIsDeadAudioOnce()	{return IsDeadAudioOnce;}
	bool GetIsNotApp()	{return IsNotApp;}
	Vector3 GetDeadParticlePos()	{return DeadParticlePos;}

	//Setter
	void SetDirection(Vector3 _dir);

private:
	//Reset
	void Reset();

	//出現予定エフェクト
	void PopParticleApp();
	//死亡エフェクト
	void DeadParticleApp();

private:
	//接地フラグ
	bool IsGround = true;
	//落下ベクトル
	Vector3 fallV;

	//未出現
	bool IsNotApp = true;
	//死亡後のリセット時間
	int appearanceResetFrame = 0;

	//死亡
	bool IsDead = false;
	bool IsDeadAudioOnce = false;
	//死亡地点(パーティクル用)
	Vector3 DeadParticlePos = {};
	bool IsDeadParticleOnce = false;
	//死亡時、敵保存地点(軽量化用)
	Vector3 DeadPos = {50,50,50};

	//スケール
	bool IsScale = false;

	//待機
	bool IsWait = false;

	//移動方向
	Vector3 direction = {-1,0,0};

	//ポップ
	bool IsPop = false;
	int popCount = 0;
	Vector3 popPosition = {};

	//パーティクル
	ParticleObject* PopParticle = nullptr;
	ParticleObject* DeadParticle = nullptr;
};