#pragma once
#include "BaseObjObject.h"
#include "ParticleObject.h"
#include "SphereCollider.h"

#include "MapNode.h"

#include <vector>
#include <queue>

class Skelton : public BaseObjObject
{
//定数
	//ビートx回数終了時にPOP
	const int POP_COUNT = 6;
	//死亡後のリセット時間
	const int AppearanceResetFrame = 50;
	//移動待機
	const int WaitCount = 2;

	//描画範囲(非表示)
	const int DrawingRange_Not = 11;
	//描画範囲(暗めの表示)
	const int DrawingRange_Half = 7;

public:
	~Skelton();
	void Initialize(std::string filePath, bool IsSmoothing = false) override;
	void Update(Camera* camera, Vector3 playerPos);
	void Draw() override;
	void ParticleDraw();
	void Finalize() override;
	void OnCollision(const CollisionInfo& info) override;

	void Pop(Vector3 pos);

	//ヒューリスティック推定値計算
	int Heuristic(int x1,int y1, int x2,int y2){
		return abs(x1-x2) + abs(y1-y2);
	}

	//経路探索
	std::vector<MapNode*> find_path(
		std::vector<std::vector<int>>& grid, 
		int start_x, int start_y, 
		int end_x, int end_y
	);

	inline void CaveLightOn()	{IsCaveLight = true;}
	inline void CaveLightOff()	{IsCaveLight = false;}

	//Getter
	inline bool GetIsNotApp()	{return IsNotApp;}
	inline bool GetIsDeadAudio()	{return IsDeadOnceAudio;}
	inline Vector3 GetDeadParticlepos()	{return DeadParticlePos;}
	inline bool GetIsInvisible() {return IsInvisible;}

	//Setter
	inline void SetMapInfo(std::vector<std::vector<int>> info) {mapInfo = info;}

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

	//スケール
	bool IsScaleEasing = false;

	//パーティクル
	ParticleObject* PopParticle = nullptr;
	ParticleObject* DeadParticle = nullptr;
	//ライティング
	bool IsCaveLight = false;

	//移動経路
	Vector3 PlayerPos;
	std::vector<std::vector<int>> mapInfo;
	
	std::vector<MapNode*> path;
	std::vector<std::vector<int>> mapPath;

	//移動
	bool IsMoveEasing = false;
	Vector3 movePosition = {};
	Vector3 currentPos = {};
	const float MoveEasingMaxTime = 0.05f;
    float moveEasingFrame = 0;
	int pathRoot = 10;
	bool IsRootUpdate = false;

	int eX;
	int eY;
	int pX;
	int pY;

#ifdef _DEBUG
	bool IsDebug = false;
#endif // _DEBUG
};