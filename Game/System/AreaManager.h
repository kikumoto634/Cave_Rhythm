#pragma once
#include <sstream>
#include <string>

#include "../3D/Plane/Planes.h"
#include "../3D/Wall/Walls.h"
#include "../3D/Wall/IndestructibleWall.h"

#include "GameManager.h"

class AreaManager
{
private:
	static const int DIV_NUM = 31;
	static const float DIV_NUM_FLOAT;
	static const int DIV_NUM_HALF = 15;
	static const float DIV_NUM_HALF_FLOAT;
	static const float Block_Size;

	//壁破壊パーティクルのリセット時間
	const int DigAppearanceFrame = 50;

private:
	//領域
struct Area{
	int X;
	int Y;

	int Width;
	int Height;
};

//部屋
struct Room{
	int X;
	int Y;

	int Width;
	int Height;
};

public:
	void RandamAreaInitialize();
	void CSVAreaInitialize(std::string name);

	void CommonInitialize();

	void RandamAreaUpdate(Camera* camera, Vector3 PlayerPos);
	void CSVAreaUpdate(Camera* camera, Vector3 PlayerPos);

	void CommonUpdate();

	void BeatEndUpdate(GameManager* gameManager);

	void RandamAreaDraw();
	void CSVAreaDraw();

	void ParticleDraw();

	void RandamAreaFinalize();
	void CSVAreaFinalize();

	void CommonFinalize();

	//ランダム用
	Vector3 GetExitPosition()	{return exitPosition;}
	Vector3 GetPlayerPosition()	{return PlayerPopPosition;}
	Vector3 GetObjectPopPosition();
	
	//CSV用
	Vector3 GetCSVObjectPopPosition(int index);
	bool GetCSVObjectPopActive(int index, bool IsFlag = true);
	int GetCSVObjectSize()	{return (int)ObjectPos.size();}

	//Getter
	inline std::vector<std::vector<int>> GetMapInfo()	{return mapInfo;}

private:
	//地面
	void AreaPlaneInitialize(bool IsLighting = false);
	void PlaneUpdate();
	void PlaneBeatEndUpdate();
	void PlaneDraw();
	void PlaneFinalize();

	//壁
	void AreaWallsInitialize(bool IsLigthing = false);
	void WallUpdate();
	void WallBeatEndUpdate();
	void WallDraw();
	void WallFinalize();

	//破壊不可
	void RandamAreaIndestructibleWallInitialize();
	void CSVAreaIndestructibleWallInitialize();
	void IndestructibleWallUpdate();
	void IndestructibleWallDraw();
	void IndestructibleWallFinalize();

	//ダンジョン生成
	void CreateMap();
	Room DevideRoom(Area area, bool hr = false);
	Room CreateRoom(Area area);
	void ConnectRoom(Room parent, Room childRoom, int divline, bool hr);

	//CSV読み込み
	void CSVMapDataLoad(std::string fullPath);

	//座標
	void ObjectRandomPop();

	//壁破壊パーティクル発生
	void DigParticlePop();
	

private:
	//借りもの
	Camera* camera = nullptr;
	GameManager* gameManager = nullptr;
	Vector3 PlayerPos={};

	//Plane
	ObjModelManager* PlaneModel = nullptr;
	Planes* plane[DIV_NUM][DIV_NUM] = {nullptr};
	bool IsComboColorChange = false;

	//Wall
	ObjModelManager* WallModel = nullptr;
	ObjModelManager* WallColliderModel = nullptr;
	bool IsAlive = false;
	Walls* Wall[DIV_NUM][DIV_NUM] = {nullptr};

	//破壊不可能壁
	ObjModelManager* IndestructibleWallModel = nullptr;
	ObjModelManager* IndestructibleWallColliderModel = nullptr;
	IndestructibleWall* IndestructibleWalls[DIV_NUM][DIV_NUM] = {nullptr};

	//部屋情報
	/*
	* 0:無
	* 1:道
	* 2:不可壁
	* 3:可能壁
	*/
	std::vector<std::vector<int>> mapInfo;

	//ランダム生成部屋
	std::vector<Room> rooms;


	//CSVコマンド
	std::stringstream csvCommands;
	std::vector<Vector3> ObjectPos;
	std::vector<bool> ObjectPopActive;


	//壁破壊パーティクル
	bool IsDig = false;
	bool IsDigApp = false;
	int paricleApperanceFrame = 0;
	Vector3 DigParticlePos = {};
	ParticleObject* DigParticle = nullptr;
	bool IsDigSound = false;

	//座標
	//出口
	Vector3 exitPosition = {0,-5,0};

	//プレイヤー
	Vector3 PlayerPopPosition = {0,-3,0};

	//生成オブジェクト
	Vector3 ObjectPopPosition = {0,0,0};
};

