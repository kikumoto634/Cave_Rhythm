#pragma once
#include "../BaseObjObject.h"
#include "../ParticleObject.h"
class BlueSlime : public BaseObjObject
{
//定数
	//死亡後のリセット時間
	const int AppearanceResetFrame = 50;

public:
	~BlueSlime();
	void Initialize(std::string filePath, bool IsSmoothing = false) override;
	void Update(Camera* camera, Vector3 playerPos);
	void Draw() override;
	void ParticleDraw();
	void Finalize() override;
	void OnCollision(const CollisionInfo& info) override;

	void Pop(Vector3 pos);

	//Getter
	inline bool GetIsAlive()	{return IsAlive;}

private:
	//死亡エフェクト
	void DeadParticleApp();

private:
	//生存
	bool IsAlive = false;
	Vector3 IsNotAlivePos = {50,50,50};

	//死亡
	bool IsDead = false;
	bool IsDeadOnceAudio = false;
	bool IsDeadOnceParticle = false;
	Vector3 DeadParticlePos = {};

	//距離に応じた非表示
	bool IsInvisible = false;
	float distance = 0.f;

	//スケール
	bool IsScaleEasing = false;

	//パーティクル
	ParticleObject* DeadParticle = nullptr;
};

