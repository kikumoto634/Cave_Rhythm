#pragma once
#include "BaseObjObject.h"
#include "ParticleObject.h"
class Boss1 : public BaseObjObject
{
//定数
	//ビートx回数終了時にPOP
	const int POP_COUNT = 6;
	//死亡後のリセット時間
	const int AppDeadParMaxFrame = 150;

	//召喚エフェクト
	const int AppSummonParMaxFrame = 50;
	const int SummonEnemyPosNumMax = 10;

	//体力
	const int FullHP = 10;
	//無敵時間
	const int DamageResetFrame = 10;

	//移動待機
	const int MoveWaitCount = 2;

	//敵行動
	/*
	* ビート数に応じて遷移
	* 1召喚 5beat目		発生
	* 2待機 8beat		待機
	* 3接近 2待機終了後 Reset
	* 4戻る 3成功->後ろに戻る
	*/

public:
	void Initialize(std::string filePath, bool IsSmoothing = false) override;
	void Update(Camera* camera, Vector3 playerPos);
	void ParticleUpdate();
	void Draw() override;
	void ParticleDraw();
	void Finalize() override;
	void OnCollision(const CollisionInfo& info) override;

	void Pop(Vector3 pos);
	void IsSummonEnemyPopNot()	{IsSummonEnemyPop = false;}

	//Getter
	inline bool GetIsNotApp()	{return IsNotApp;}
	inline bool GetIsDeadAudio()	{return IsDeadOnceAudio;}
	inline bool GetIsDead()	{return IsDead;}
	inline Vector3 GetDeadParticlepos()	{return DeadParticlePos;}
	inline bool GetIsInvisible() {return IsInvisible;}
	inline bool GetIsSummonEnemyPop()	{return IsSummonEnemyPop;}

private:

	void Movement();
	void Summon();

	//死亡エフェクト
	void DeadParticleApp();
	//召喚エフェクト
	void SummonParticleApp();

private:

	//未出現
	bool IsNotApp = false;

	//パターンカウント
	int patternCount = 0;

	//HP
	int hp = 0;

	//ダメージ
	bool IsDamage = false;
	int damageResetCurFrame = 0;

	//追跡
	bool IsMove = true;
	Vector3 targetPos;
	bool IsMoveEasing = false;
	int moveWaitCurCount = 0;
	Vector3 currentPos;
	Vector3 movePosition;
	Vector3 OldPosition = {};
	const float MoveEasingMaxTime = 0.05f;
	float moveEasingFrame = 0;

	//召喚
	bool IsSummon = false;
	bool IsSummonEnemyPop = false;
	int summonEnemyPosNum = 0;

	//退却
	bool IsComeBack= false;
	Vector3 homePos = {};

	//コライダー
	float radius = 1.0f;

	Vector3 NotAlivePos = {50,50,50};
	//死亡
	bool IsDead = false;
	bool IsDeadOnceAudio = false;

	//距離に応じた非表示
	bool IsInvisible = false;
	float distance = 0.f;

	//スケール
	bool IsScaleEasing = false;

	//パーティクル
	std::unique_ptr<ParticleObject> DeadParticle;
	int appDeadParFrame = 0;
	bool IsDeadOnceParticle = false;
	Vector3 DeadParticlePos = {};

	std::unique_ptr<ParticleObject> SummonParticle;
	int appSummonParFrame = 0;
	Vector3 SummonParticlePos ={};
};

