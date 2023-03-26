#pragma once
#include "../BaseObjObject.h"
#include "../ParticleObject.h"
class BlueSlime : public BaseObjObject
{
//定数
	//ビートx回数終了時にPOP
	const int POP_COUNT = 6;
	//死亡後のリセット時間
	const int AppearanceResetFrame = 50;

	//描画範囲(非表示)
	const int DrawingRange_Not = 15;
	//描画範囲(暗めの表示)
	const int DrawingRange_Half = 8;

public:
	~BlueSlime();
	void Initialize(std::string filePath, bool IsSmoothing = false) override;
	void Update(Camera* camera, Vector3 playerPos);
	void Draw() override;
	void ParticleDraw();
	void Finalize() override;
	void OnCollision(const CollisionInfo& info) override;

	void Pop(Vector3 pos);

	inline void CaveLightOn()	{IsCaveLight = true;}
	inline void CaveLightOff()	{IsCaveLight = false;}

	//Getter
	inline bool GetIsNotApp()	{return IsNotApp;}
	inline bool GetIsDeadAudio()	{return IsDeadOnceAudio;}
	inline Vector3 GetDeadParticlepos()	{return DeadParticlePos;}
	inline bool GetIsInvisible() {return IsInvisible;}

private:
	void Reset();

	//出現予定エフェクト
	void PopParticleApp();
	//死亡エフェクト
	void DeadParticleApp();

private:

	//未出現
	bool IsNotApp = false;

	Vector3 NotAlivePos = {50,50,50};
	//死亡
	bool IsDead = false;
	bool IsDeadOnceAudio = false;
	bool IsDeadOnceParticle = false;
	Vector3 DeadParticlePos = {};

	//コライダー
	bool IsCollision = false;

	//復活
	int appearancePopFrame = 0;

	//ポップ
	bool IsPop = false;
	int popCount = 0;
	Vector3 PopParticlePos = {};

	//距離に応じた非表示
	bool IsInvisible = false;
	float distance = 0.f;

	//スケール
	bool IsScaleEasing = false;

	//パーティクル
	ParticleObject* PopParticle = nullptr;
	ParticleObject* DeadParticle = nullptr;

	//ライティング
	bool IsCaveLight = false;
};

