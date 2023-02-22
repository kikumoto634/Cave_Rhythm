#pragma once
#include "../BaseObjObject.h"
#include "../ParticleObject.h"
#include "../../Collision/SphereCollider.h"

class Skelton : public BaseObjObject
{
//定数
	//ビートx回数終了時にPOP
	const int POP_COUNT = 6;
	//死亡後のリセット時間
	const int AppearanceResetFrame = 50;
	//移動待機
	const int WaitCount = 2;

public:
	~Skelton();
	void Initialize(std::string filePath, bool IsSmoothing = false) override;
	void Update(Camera* camera, Vector3 playerPos);
	void Draw() override;
	void ParticleDraw();
	void Finalize() override;
	void OnCollision(const CollisionInfo& info) override;

	void Pop(Vector3 pos);

	//Getter
	inline bool GetIsNotApp()	{return IsNotApp;}
	inline bool GetIsDeadAudio()	{return IsDeadOnceAudio;}
	inline Vector3 GetDeadParticlepos()	{return DeadParticlePos;}
	inline bool GetIsInvisible() {return IsInvisible;}

private:
	void Reset();

	void Movement();

	//出現予定エフェクト
	void PopParticleApp();
	//死亡エフェクト
	void DeadParticleApp();

private:

	//待機時間
	int waitCount = 0;

	//レイ
	Vector3 RayDir = {0,0,1};
	Vector3 OldPosition = {};
	//コライダー
	SphereCollider* sphereCollider = nullptr;
	bool IsCollision = true;

	//未出現
	bool IsNotApp = false;

	Vector3 NotAlivePos = {50,50,50};
	//死亡
	bool IsDead = false;
	bool IsDeadOnceAudio = false;
	bool IsDeadOnceParticle = false;
	Vector3 DeadParticlePos = {};

	//復活
	int appearancePopFrame = 0;

	//ポップ
	bool IsPop = false;
	int popCount = 0;
	Vector3 PopParticlePos = {};

	//距離に応じた非表示
	bool IsInvisible = false;
	float distance = 0.f;

	//追跡
	Vector3 targetPos;
	bool IsMoveEasing = false;
	Vector3 currentPos;
	Vector3 movePosition;
	const float MoveEasingMaxTime = 0.05f;
	float moveEasingFrame = 0;

	//スケール
	bool IsScaleEasing = false;

	//パーティクル
	ParticleObject* PopParticle = nullptr;
	ParticleObject* DeadParticle = nullptr;
};

