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

	//�ǔj��p�[�e�B�N���̃��Z�b�g����
	const int DigAppearanceFrame = 50;

private:
	//�̈�
struct Area{
	int X;
	int Y;

	int Width;
	int Height;
};

//����
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

	//�����_���p
	Vector3 GetExitPosition()	{return exitPosition;}
	Vector3 GetPlayerPosition()	{return PlayerPopPosition;}
	Vector3 GetObjectPopPosition();
	
	//CSV�p
	Vector3 GetCSVObjectPopPosition(int index);
	bool GetCSVObjectPopActive(int index, bool IsFlag = true);
	int GetCSVObjectSize()	{return (int)ObjectPos.size();}

private:
	//�n��
	void RandamAreaPlaneInitialize();
	void CSVAreaPlaneInitialize();
	void PlaneUpdate();
	void PlaneBeatEndUpdate();
	void PlaneDraw();
	void PlaneFinalize();

	//��
	void RandamAreaWallsInitialize();
	void CVSAreaWallsInitialize();
	void WallUpdate();
	void WallBeatEndUpdate();
	void WallDraw();
	void WallFinalize();

	//�j��s��
	void RandamAreaIndestructibleWallInitialize();
	void CSVAreaIndestructibleWallInitialize();
	void IndestructibleWallUpdate();
	void IndestructibleWallDraw();
	void IndestructibleWallFinalize();

	//�_���W��������
	void CreateMap();
	Room DevideRoom(Area area, bool hr = false);
	Room CreateRoom(Area area);
	void ConnectRoom(Room parent, Room childRoom, int divline, bool hr);

	//CSV�ǂݍ���
	void CSVMapDataLoad(std::string fullPath);

	//���W
	void ObjectRandomPop();

	//�ǔj��p�[�e�B�N������
	void DigParticlePop();
	

private:
	//�؂����
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
	char WallMap[DIV_NUM][DIV_NUM] = {};
	Walls* Wall[DIV_NUM][DIV_NUM] = {nullptr};

	//�j��s�\��
	ObjModelManager* IndestructibleWallModel = nullptr;
	ObjModelManager* IndestructibleWallColliderModel = nullptr;
	IndestructibleWall* IndestructibleWalls[DIV_NUM][DIV_NUM] = {nullptr};

	//����
	std::vector<Room> rooms;

	//�o��
	Vector3 exitPosition = {0,-5,0};

	//�v���C���[
	Vector3 PlayerPopPosition = {0,-3,0};

	//�����I�u�W�F�N�g
	Vector3 ObjectPopPosition = {0,0,0};

	//CSV�R�}���h
	std::stringstream csvCommands;
	int CSVMap[DIV_NUM][DIV_NUM] = {0};
	std::vector<Vector3> ObjectPos;
	std::vector<bool> ObjectPopActive;

	//�ǔj��p�[�e�B�N��
	bool IsDig = false;
	bool IsDigApp = false;
	int paricleApperanceFrame = 0;
	Vector3 DigParticlePos = {};
	ParticleObject* DigParticle = nullptr;
	bool IsDigSound = false;

};

