#pragma once
#include "BaseEnemy.h"
#include "MapNode.h"
#include <vector>

class BossStateManager;

class Boss1 : public BaseEnemy
{
public:
	friend class IdelBossState;
	friend class SummonBossState;
	friend class TrackBossState;
	friend class RunAwayBossState;
	friend class DeadBossState;

	//template <class T> using queue = std::queue<T>;

public:
	void AddInitialize() override;
	void AddUpdate() override;
	void AddDraw() override;
	void AddParticleDraw() override;
	void AddFinalize() override;

	void EventBegin()	{isEvent_ = true;}
	void EventEnd()	{isEvent_ = false;}

	//Getter
	inline Vector3 GetPopPosition() override {return particlePos_;}
	inline bool GetIsEvent()	{return isEvent_;}

	void BossPopInit(Vector3 pos);

private:
	//経路探索(A*アルゴリズム)
	vector<MapNode*> PathSearch(
		vector<vector<int>>& grid, 
		int start_x, int start_y, 
		int end_x, int end_y);
	//ヒューリスティック推定値計算
	int Heuristic(int x1,int y1, int x2,int y2){
		return abs(x1-x2) + abs(y1-y2);
	}

private:
	//状態
	BossStateManager* state_= nullptr;
	//出現位置
	Vector3 popPosition_ = {};

	bool isEvent_ = false;

	//queue<Vector3> summonObjPopPos;
};

