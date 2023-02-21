#include "AreaManager.h"

#include <fstream>
#include <cassert>

using namespace std;

const float AreaManager::DIV_NUM_FLOAT = 31.f;
const float AreaManager::DIV_NUM_HALF_FLOAT = 15.f;
const float AreaManager::Block_Size = 2.f;



void AreaManager::RandamAreaInitialize()
{
	CreateMap();
	ObjectRandomPop();

	RandamAreaWallsInitialize();
	RandamAreaPlaneInitialize();
	RandamAreaIndestructibleWallInitialize();
}

void AreaManager::CSVAreaInitialize(string name)
{
	CSVMapDataLoad(name);

	CSVAreaPlaneInitialize();
	CVSAreaWallsInitialize();
	CSVAreaIndestructibleWallInitialize();
}

void AreaManager::RandamAreaUpdate(Camera *camera, Vector3 PlayerPos)
{
	assert(camera);

	this->camera = camera;
	this->PlayerPos = PlayerPos;

	WallUpdate();
	IndestructibleWallUpdate();
	PlaneUpdate();
}

void AreaManager::CSVAreaUpdate(Camera* camera, Vector3 PlayerPos)
{
	assert(camera);

	this->camera = camera;
	this->PlayerPos = PlayerPos;

	WallUpdate();
	IndestructibleWallUpdate();
	PlaneUpdate();
}

void AreaManager::BeatEndUpdate(GameManager* gameManager)
{
	assert(gameManager);

	this->gameManager = gameManager;

	PlaneBeatEndUpdate();
}

void AreaManager::RandamAreaDraw()
{
	WallDraw();
	IndestructibleWallDraw();
	PlaneDraw();
}

void AreaManager::CSVAreaDraw()
{
	WallDraw();
	IndestructibleWallDraw();
	PlaneDraw();
}

void AreaManager::ParticleDraw()
{
	WallParticleDraw();
}

void AreaManager::RandamAreaFinalize()
{
	WallFinalize();
	IndestructibleWallFinalize();
	PlaneFinalize();
}

void AreaManager::CSVAreaFinalize()
{
	WallFinalize();
	IndestructibleWallFinalize();
	PlaneFinalize();
}

#pragma region �n��
void AreaManager::RandamAreaPlaneInitialize()
{
	PlaneModel = new ObjModelManager();
	PlaneModel->CreateModel("GroundBlock");

	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j] = new Planes();
			plane[i][j]->Initialize(PlaneModel);
			pos = {startPos + i, startPos + j};
			pos *= Block_Size;
			plane[i][j]->SetPosition({pos.x,-5,pos.y});
		}
	}
}

void AreaManager::CSVAreaPlaneInitialize()
{
	PlaneModel = new ObjModelManager();
	PlaneModel->CreateModel("GroundBlock");

	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(CSVMap[i][j] == 0){
				//plane[i][j]->IsNotAlive();
				continue;
			}

			plane[i][j] = new Planes();
			plane[i][j]->Initialize(PlaneModel);
			pos = {startPos + i, startPos + j};
			pos*= Block_Size;
			plane[i][j]->SetPosition({pos.x,-5,pos.y});
		}
	}
}

void AreaManager::PlaneUpdate()
{
	//�n��
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(plane[i][j] == nullptr) continue;
			plane[i][j]->SetPlayerPos(PlayerPos);
			plane[i][j]->Update(camera);
		}
	}
}

void AreaManager::PlaneBeatEndUpdate()
{
	IsComboColorChange = !IsComboColorChange;
	bool IsChange = false;

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(plane[i][j] == nullptr) continue;
			//�R���{���ɉ����ĐF�ω�
			if(gameManager->GetComboNum() >= gameManager->GetPlaneColorChangeCombo()){
				plane[i][j]->PlaneColorChange(IsChange, IsComboColorChange);
				IsChange = !IsChange;
			}
			else if(gameManager->GetComboNum() < 10){
				plane[i][j]->PlaneColorReset();
			}

			plane[i][j]->IsBeatEndOn();
		}
	}
}

void AreaManager::PlaneDraw()
{
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(plane[i][j] == nullptr) continue;
			plane[i][j]->Draw();
		}
	}
}

void AreaManager::PlaneFinalize()
{
	delete PlaneModel;
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(plane[i][j] == nullptr) continue;
			plane[i][j]->Finalize();
			delete plane[i][j];
			plane[i][j] = nullptr;
		}
	}
}
#pragma endregion

#pragma region ��
void AreaManager::RandamAreaWallsInitialize()
{
	WallModel = new ObjModelManager();
	WallModel->CreateModel("GroundBlock2");

	WallColliderModel = new ObjModelManager();
	WallColliderModel->CreateModel("GroundBlock2_Collider");

	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(WallMap[i][j] != '*') {
				continue;
			}

			Wall[i][j] = new Walls();
			Wall[i][j]->Initialize(WallModel,WallColliderModel);

			pos = {startPos + i, startPos + j};
			pos*= Block_Size;
			Wall[i][j]->SetPosition({pos.x,-3,pos.y});
			
		}
	}
}

void AreaManager::CVSAreaWallsInitialize()
{
	WallModel = new ObjModelManager();
	WallModel->CreateModel("GroundBlock2");

	WallColliderModel = new ObjModelManager();
	WallColliderModel->CreateModel("GroundBlock2_Collider");

	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			
			if(CSVMap[i][j] != 3){
				continue;
			}

			Wall[i][j] = new Walls();
			Wall[i][j]->Initialize(WallModel,WallColliderModel);

			pos = {startPos + i, startPos + j};
			pos*= Block_Size;
			Wall[i][j]->SetPosition({pos.x,-3,pos.y});
		}
	}
}

void AreaManager::WallUpdate()
{
	//�n��
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(Wall[i][j] == nullptr) continue;
			Wall[i][j]->SetPlayerPos(PlayerPos);
			if(Wall[i][j]->GetIsDigSound())	gameManager->AudioPlay(10);
			Wall[i][j]->Update(camera);
		}
	}
}

void AreaManager::WallBeatEndUpdate()
{
}

void AreaManager::WallDraw()
{
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(Wall[i][j] == nullptr) continue;
			Wall[i][j]->Draw();
		}
	}
}

void AreaManager::WallParticleDraw()
{
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(Wall[i][j] == nullptr) continue;
			Wall[i][j]->ParticleDraw();
		}
	}
}

void AreaManager::WallFinalize()
{

	delete WallModel;
	WallModel = nullptr;
	delete WallColliderModel;
	WallColliderModel = nullptr;

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(Wall[i][j] == nullptr) continue;
			Wall[i][j]->Finalize();
			delete Wall[i][j];
			Wall[i][j] = nullptr;
		}
	}
}
#pragma endregion

#pragma region �j��s��
void AreaManager::RandamAreaIndestructibleWallInitialize()
{
	IndestructibleWallModel = new ObjModelManager();
	IndestructibleWallModel->CreateModel("GroundBlock3");

	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};
	for(int i = 0; i <DIV_NUM; i++){
		//��
		IndestructibleWalls[0][i] = new IndestructibleWall();
		IndestructibleWalls[0][i]->Initialize(IndestructibleWallModel);
		pos = {startPos + i, startPos - 1};
		pos*=Block_Size;
		IndestructibleWalls[0][i]->SetPosition({pos.x, -3,pos.y});
		//��
		IndestructibleWalls[1][i] = new IndestructibleWall();
		IndestructibleWalls[1][i]->Initialize(IndestructibleWallModel);
		pos = {startPos + i, DIV_NUM_HALF_FLOAT+1};
		pos*=Block_Size;
		IndestructibleWalls[1][i]->SetPosition({pos.x, -3,pos.y});
		////��
		IndestructibleWalls[2][i] = new IndestructibleWall();
		IndestructibleWalls[2][i]->Initialize(IndestructibleWallModel);
		pos = {startPos - 1, startPos + i};
		pos*=Block_Size;
		IndestructibleWalls[2][i]->SetPosition({pos.x, -3,pos.y});
		////�E
		IndestructibleWalls[3][i] = new IndestructibleWall();
		IndestructibleWalls[3][i]->Initialize(IndestructibleWallModel);
		pos = {DIV_NUM_HALF_FLOAT+1, startPos + i};
		pos*=Block_Size;
		IndestructibleWalls[3][i]->SetPosition({pos.x, -3,pos.y});
	}
}
void AreaManager::CSVAreaIndestructibleWallInitialize()
{
	IndestructibleWallModel = new ObjModelManager();
	IndestructibleWallModel->CreateModel("GroundBlock3");

	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(CSVMap[i][j] != 2){
				continue;
			}

			IndestructibleWalls[i][j] = new IndestructibleWall();
			IndestructibleWalls[i][j]->Initialize(IndestructibleWallModel);
			pos = {startPos + i ,startPos + j};
			pos *= Block_Size;
			IndestructibleWalls[i][j]->SetPosition({pos.x,-3,pos.y});
		}
	}
}
void AreaManager::IndestructibleWallUpdate()
{
	//�n��
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(IndestructibleWalls[i][j] == nullptr) continue;
			IndestructibleWalls[i][j]->SetPlayerPos(PlayerPos);
			IndestructibleWalls[i][j]->Update(camera);
		}
	}
}
void AreaManager::IndestructibleWallDraw()
{
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(IndestructibleWalls[i][j] == nullptr) continue;
			IndestructibleWalls[i][j]->Draw();
		}
	}
}
void AreaManager::IndestructibleWallFinalize()
{
	delete IndestructibleWallModel;
	IndestructibleWallModel = nullptr;

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(IndestructibleWalls[i][j] == nullptr) continue;
			IndestructibleWalls[i][j]->Finalize();
			delete IndestructibleWalls[i][j];
			IndestructibleWalls[i][j] = nullptr;
		}
	}
}
#pragma endregion

#pragma region �_���W������������
void AreaManager::CreateMap()
{
	for(int y = 0; y < DIV_NUM; y++){
		for(int x = 0; x < DIV_NUM; x++){
			WallMap[y][x] = '*';
		}
	}

	DevideRoom({0,0,DIV_NUM,DIV_NUM}, false);
}
AreaManager::Room AreaManager::DevideRoom(Area area, bool hr)
{
	if(area.Width < 6 || area.Height < 6) {
		return CreateRoom(area);
	}

	if(!hr){
		//�̈�̍��� 1/3~2/3�͈̔͂Ń����_��
		//24 : 1/3 = 8, 2/3 = 16
		//�������C������
		int divline = static_cast<int>(floor(area.Height/3 + rand()%(area.Height/3)));

		//�e(��)
		Area parent = {area.X,area.Y, area.Width,divline};
		//�q(��)
		Area child = {area.X,area.Y+divline, area.Width,area.Height-divline};

		//�e�̈�ɕ����쐬
		Room parentRoom = CreateRoom(parent);
		//�q�̈�ɕ����쐬
		Room childRoom = DevideRoom(child, !hr);

		//�ʘH�J��
		ConnectRoom(parentRoom, childRoom, divline, hr);

		return parentRoom;
	}
	else if(hr){
		//�̈�̍��� 1/3~2/3�͈̔͂Ń����_��
		//48 : 1/3 = 16, 2/3 = 32
		//�������C������
		//int divline = rand()%(area.Width/3)+ area.Width/3;
		int divline = static_cast<int>(floor(area.Width/3 + rand()%(area.Width/3)));

		//�e(��)
		Area parent = {area.X,area.Y, divline,area.Height};
		//�q(��)
		Area child = {area.X+divline,area.Y, area.Width-divline,area.Height};

		//�e�̈�ɕ����쐬
		Room parentRoom = CreateRoom(parent);
		//�q�̈�ɕ����쐬
		Room childRoom = DevideRoom(child, !hr);

		//�ʘH�J��
		ConnectRoom(parentRoom, childRoom, divline, hr);

		return parentRoom;
	}
	return CreateRoom(area);
}
AreaManager::Room AreaManager::CreateRoom(Area area)
{
//�͈͓��Ń����_����4�_���擾
	int X1 = rand()%(area.Width) + area.X;
	int X2 = rand()%(area.Width) + area.X;
	int Y1 = rand()%(area.Height) + area.Y;
	int Y2 = rand()%(area.Height) + area.Y;

	//X�̎n�_�A�������ق�
	int X = min(X1,X2);
	//��������
	int W = abs(X1 - X2) + 2;
	//Y�̎n�_�A�������ق�
	int Y = min(Y1,Y2);
	//����������
	int H = abs(Y1 - Y2) + 2;

	Room room = {X,Y,W,H};
	rooms.push_back(room);

	//�}�b�v�ɔ��f
	for(int y = 0; y < H; y++){
		for(int x = 0; x < W; x++){
			WallMap[Y + y][X + x] = 'A';
		}
	}

	return room;
}
void AreaManager::ConnectRoom(Room parent, Room childRoom, int divline, bool hr)
{
	if(hr){
		//�e�����Ȃ����烉���_���Ɉ�_��I��
		int X1 = parent.X + rand()%parent.Width;
		//�q����
		int X2 = childRoom.X + rand()%childRoom.Width;
		//������
		int minX = min(X1,X2);
		//�傫��
		int maxX = max(X1,X2);

		//�}�b�v�ɕ������C����̒ʘH�쐬
		for(int i = 0; minX + i <= maxX; i++){
			WallMap[divline][minX + i] = 'A';
		}
		//�������C������e�����ւ̒ʘH
		for(int i = 1; WallMap[divline-i][X1] == '*'; i++){
			WallMap[divline-i][X1] = 'A';
		}
		//�q������
		for(int i = 1; WallMap[divline+i][X2] == '*'; i++){
			WallMap[divline+i][X2] = 'A';
		}
	}
	else if(!hr){
		//�e�����Ȃ����烉���_���Ɉ�_��I��
		int Y1 = parent.Y + rand()%parent.Height;
		//�q����
		int Y2 = childRoom.Y + rand()%childRoom.Height;
		//������
		int minY = min(Y1,Y2);
		//�傫��
		int maxY = max(Y1,Y2);

		//�}�b�v�ɕ������C����̒ʘH�쐬
		for(int i = 0; minY + i <= maxY; i++){
			WallMap[minY + i][divline] = 'A';
		}
		//�������C������e�����ւ̒ʘH
		for(int i = 1; WallMap[Y1][divline-i] == '*'; i++){
			WallMap[Y1][divline-i] = 'A';
		}
		//�q������
		for(int i = 1; WallMap[Y2][divline+i] == '*'; i++){
			WallMap[Y2][divline+i] = 'A';
		}
	}
}
#pragma endregion

#pragma region CSV�f�[�^ �_���W��������
void AreaManager::CSVMapDataLoad(string fullPath)
{
	string directoryPath = "Resources/CSVData/";
	string csv = ".csv";
	//file�J��
	ifstream file;
	string Path = directoryPath + fullPath + csv;
	file.open(Path);
	assert(file.is_open());

	//file�|�{�𕶎���X�g���[���ɃR�s�[
	csvCommands << file.rdbuf();
	file.close();

	//��s���̕����������ϐ�
	string line;
	int y = 0;
	int x = 0;

	float start = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	//�R�}���h���[�v
	while(getline(csvCommands, line)){
		//��s���̂���������X�g���[���ɕϊ����ĉ�͂��₷��
		istringstream line_stream(line);

		string word;
		string area;

		//��؂�ōs�̑K��������擾
		getline(line_stream, word, ',');

		x = 0;
		for(int i = 0; i < DIV_NUM*DIV_NUM; i++){

			if(word.find("0") == 0){
				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("1") == 0){
				CSVMap[y][x] = 1;
				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("2") == 0){
				CSVMap[y][x] = 2;
				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("3") == 0){
				CSVMap[y][x] = 3;
				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("4") == 0){
				CSVMap[y][x] = 1;

				pos = {start + y, start + x};
				pos *= Block_Size;
				PlayerPopPosition = {pos.x, -3, pos.y};

				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("5") == 0){
				CSVMap[y][x] = 1;

				pos = {start + y, start + x};
				pos *= Block_Size;
				ObjectPos.push_back({pos.x,0,pos.y});
				ObjectPopActive.push_back(true);

				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("6") == 0){
				CSVMap[y][x] = 1;

				pos = {start + y, start + x};
				pos *= Block_Size;
				exitPosition = {pos.x,-5,pos.y};

				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("//") == 0){
				y++;
				break;
			}
		}
	}
}

Vector3 AreaManager::GetCSVObjectPopPosition(int index)
{
	return ObjectPos[index];
}
bool AreaManager::GetCSVObjectPopActive(int index, bool IsFlag)
{
	if(ObjectPopActive[index]){
		ObjectPopActive[index] = IsFlag;
		return true;
	}

	return false;
}
#pragma endregion

#pragma region �����_������
void AreaManager::ObjectRandomPop()
{
	int roomSize = (int)rooms.size();
	Vector2 areaPos;
	Vector2 areaWH;

	float start = -DIV_NUM_HALF_FLOAT;
	//�o��
	int exitRoomsNum = rand()%roomSize;
	//�v���C���[
	int playerRoomsNum = rand()%roomSize;

	//exit
	areaPos = {(start+rooms[exitRoomsNum].Y), (start+rooms[exitRoomsNum].X)};
	areaPos *= Block_Size;
	areaWH = {float(rand()%(rooms[exitRoomsNum].Height-1)),float(rand()%(rooms[exitRoomsNum].Width-1))};
	areaWH *= Block_Size;
	exitPosition = {areaPos.x+areaWH.x,-5.f,areaPos.y+areaWH.y};

	//player
	areaPos = {(start+rooms[playerRoomsNum].Y),(start+rooms[playerRoomsNum].X)};
	areaPos*= Block_Size;
	areaWH = {float(rand()%(rooms[playerRoomsNum].Height-1)),float(rand()%(rooms[playerRoomsNum].Width-1))};
	areaWH *= Block_Size;
	PlayerPopPosition = {areaPos.x+areaWH.x,-3.f,areaPos.y+areaWH.y};
}

Vector3 AreaManager::GetObjectPopPosition()
{
	int roomSize = (int)rooms.size();
	Vector2 areaPos;
	Vector2 areaWH;

	float start = -DIV_NUM_HALF_FLOAT;
	int RoomsNum = rand()%roomSize;

	areaPos = {(start+rooms[RoomsNum].Y),(start+rooms[RoomsNum].X)};
	areaPos *= Block_Size;
	areaWH = {float(rand()%(rooms[RoomsNum].Height-1)),float(rand()%(rooms[RoomsNum].Width-1))};
	areaWH *= Block_Size;
	ObjectPopPosition = {areaPos.x+areaWH.x,0.f,areaPos.y+areaWH.y};

	return ObjectPopPosition;
}
#pragma endregion;