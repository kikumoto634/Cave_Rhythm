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

	CommonInitialize();

	AreaWallsInitialize(true);
	AreaPlaneInitialize(true);
	RandamAreaIndestructibleWallInitialize();
}

void AreaManager::CSVAreaInitialize(string name)
{
	CSVMapDataLoad(name);

	CommonInitialize();

	AreaPlaneInitialize();
	AreaWallsInitialize();
	CSVAreaIndestructibleWallInitialize();
}

void AreaManager::CommonInitialize()
{
	if(!PlaneModel){
		PlaneModel = new ObjModelManager();
		PlaneModel->CreateModel("GroundBlock");
	}

	if(!WallModel){
		WallModel = new ObjModelManager();
		WallModel->CreateModel("GroundBlock2");
	}
	if(!WallColliderModel){
		WallColliderModel = new ObjModelManager();
		WallColliderModel->CreateModel("GroundBlock2_Collider");
	}

	if(!IndestructibleWallModel){
		IndestructibleWallModel = new ObjModelManager();
		IndestructibleWallModel->CreateModel("GroundBlock3");
	}
	if(!IndestructibleWallColliderModel){
		IndestructibleWallColliderModel = new ObjModelManager();
		IndestructibleWallColliderModel->CreateModel("GroundBlock2_Collider");
	}

	DigParticle = make_unique<ParticleObject>();
	DigParticle->Initialize();
}

void AreaManager::RandamAreaUpdate(Camera *camera, Vector3 PlayerPos)
{
	assert(camera);

	this->camera = camera;
	this->PlayerPos = PlayerPos;

	IsDigSound = false;

	WallUpdate();
	IndestructibleWallUpdate();
	PlaneUpdate();

	CommonUpdate();
}

void AreaManager::CSVAreaUpdate(Camera* camera, Vector3 PlayerPos)
{
	assert(camera);

	IsDigSound = false;

	this->camera = camera;
	this->PlayerPos = PlayerPos;

	WallUpdate();
	IndestructibleWallUpdate();
	PlaneUpdate();

	CommonUpdate();
}

void AreaManager::CommonUpdate()
{
	if(IsDig && !IsAlive){
		DigParticlePop();
		IsDig = false;
	}
	else if(IsDigApp){
		if(paricleApperanceFrame >= DigAppearanceFrame){
			IsDigApp = false;
			paricleApperanceFrame = 0;
		}
		paricleApperanceFrame++;
		DigParticle->Update(this->camera);
	}
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
	if(IsDigApp){
		DigParticle->Draw();
	}
}

void AreaManager::RandamAreaFinalize()
{
	CommonFinalize();

	WallFinalize();
	IndestructibleWallFinalize();
	PlaneFinalize();
}

void AreaManager::CSVAreaFinalize()
{
	PlayerPos = {};

	CommonFinalize();

	WallFinalize();
	IndestructibleWallFinalize();
	PlaneFinalize();
}

void AreaManager::CommonFinalize()
{
	DigParticle->Finalize();
	//delete DigParticle;
}

#pragma region 地面
void AreaManager::AreaPlaneInitialize(bool IsLighting)
{
	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		plane.resize(i+1);
		for(int j = 0; j < DIV_NUM; j++){
			if(mapInfo[i][j] == 0){
				continue;
			}
			Planes* obj = new Planes();
			obj->Initialize(PlaneModel);
			pos = {startPos + j, startPos + i};
			pos *= Block_Size;
			obj->SetPosition({pos.x,-5,-pos.y});

			if(!IsLighting) {
				plane[i].push_back(move(obj));
				continue;
			}
			obj->CaveLightOn();
			plane[i].push_back(move(obj));
		}
	}
}
void AreaManager::PlaneUpdate()
{
	//地面
	for(size_t i = 0; i < plane.size(); i++){
		for(auto it = plane[i].begin(); it != plane[i].end(); ++it){
			if((*it) == nullptr) continue;
			(*it)->SetPlayerPos(PlayerPos);
			(*it)->Update(camera);
		}
	}
}
void AreaManager::PlaneBeatEndUpdate()
{
	IsComboColorChange = !IsComboColorChange;
	bool IsChange = false;

	for(size_t i = 0; i < plane.size(); i++){
		for(auto it = plane[i].begin(); it != plane[i].end(); ++it){
			if((*it) == nullptr){
				IsChange = !IsChange; 
				continue;
			}
			//コンボ数に応じて色変化
			if(gameManager->GetComboNum() >= gameManager->GetPlaneColorChangeCombo()){
				(*it)->PlaneColorChange(IsChange, IsComboColorChange);
				IsChange = !IsChange;
			}
			else if(gameManager->GetComboNum() < 10){
				(*it)->PlaneColorReset();
			}

			(*it)->IsBeatEndOn();
		}
	}
}
void AreaManager::PlaneDraw()
{
	for(size_t i = 0; i < plane.size(); i++){
		for(auto it = plane[i].begin(); it != plane[i].end(); ++it){
			if((*it) == nullptr) continue;
			(*it)->Draw();
		}
	}
}
void AreaManager::PlaneFinalize()
{
	delete PlaneModel;
	for(size_t i = 0; i < plane.size(); i++){
		for(auto it = plane[i].begin(); it != plane[i].end(); ++it){
			if((*it) == nullptr) continue;
			(*it)->Finalize();
			delete (*it);
			(*it) = nullptr;
		}
	}
}
#pragma endregion

#pragma region 壁
void AreaManager::AreaWallsInitialize(bool IsLigthing)
{
	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		wall.resize(i+1);
		for(int j = 0; j < DIV_NUM; j++){
			
			if(mapInfo[i][j] != 3){
				continue;
			}

			Walls* obj = new Walls();
			obj->Initialize(WallModel,WallColliderModel);

			pos = {startPos + j, startPos + i};
			pos*= Block_Size;
			obj->SetPosition({pos.x,-3,-pos.y});

			if(!IsLigthing) {
				wall[i].push_back(move(obj));
				continue;
			}
			obj->CaveLightOn();
			wall[i].push_back(move(obj));
		}
	}
}
void AreaManager::WallUpdate()
{
	//地面
	for(size_t i = 0; i < wall.size(); i++){
		for(auto it = wall[i].begin(); it != wall[i].end(); ++it){
			if ((*it) == nullptr) continue;

			(*it)->SetPlayerPos(PlayerPos);
			if((*it)->GetIsDIg()){
				IsDigSound = true;
				IsDig =true;
				DigParticlePos = (*it)->GetDigPosition();
				gameManager->AudioPlay(10);
			}
			(*it)->Update(camera);
		}
	}
}
void AreaManager::WallBeatEndUpdate()
{
}
void AreaManager::WallDraw()
{
	for(size_t i = 0; i < wall.size(); i++){
		for(auto it = wall[i].begin(); it != wall[i].end(); ++it){
			if((*it) == nullptr) continue;
			(*it)->Draw();
		}
	}
}
void AreaManager::WallFinalize()
{

	delete WallModel;
	WallModel = nullptr;
	delete WallColliderModel;
	WallColliderModel = nullptr;

	for(size_t i = 0; i < wall.size(); i++){
		for(auto it = wall[i].begin(); it != wall[i].end(); ++it){
			if((*it) == nullptr) continue;
			(*it)->Finalize();
			delete (*it);
			(*it) = nullptr;
		}
	}
}
#pragma endregion

#pragma region 破壊不可壁
void AreaManager::RandamAreaIndestructibleWallInitialize()
{
	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < 4; i++){
		indestructibleWalls.resize(i + 1);
		for(int j = 0; j < DIV_NUM; j++){
			IndestructibleWall* obj = new IndestructibleWall();
			obj->Initialize(IndestructibleWallModel, IndestructibleWallColliderModel);

			if(i == 0){
				pos = {startPos + j, startPos - 1};
				pos*=Block_Size;
			}
			else if(i == 1){
				pos = {startPos + j, DIV_NUM_HALF_FLOAT+1};
				pos*=Block_Size;
			}
			else if(i == 2){
				pos = {startPos - 1, startPos + j};
				pos*=Block_Size;
			}
			else if(i == 3){
				pos = {DIV_NUM_HALF_FLOAT+1, startPos + j};
				pos*=Block_Size;
			}
			obj->SetPosition({pos.x, -3,pos.y});
			obj->CaveLightOn();

			indestructibleWalls[i].push_back(move(obj));
		}
	}
}
void AreaManager::CSVAreaIndestructibleWallInitialize()
{
	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		indestructibleWalls.resize(i+1);
		for(int j = 0; j < DIV_NUM; j++){
			if(mapInfo[i][j] != 2){
				continue;
			}

			IndestructibleWall* obj = new IndestructibleWall();
			obj->Initialize(IndestructibleWallModel, IndestructibleWallColliderModel);
			pos = {startPos + j ,startPos + i};
			pos *= Block_Size;
			obj->SetPosition({pos.x,-3,-pos.y});

			indestructibleWalls[i].push_back(move(obj));
		}
	}
}
void AreaManager::IndestructibleWallUpdate()
{
	//地面
	for(size_t i = 0; i < indestructibleWalls.size(); i++){
		for(auto it = indestructibleWalls[i].begin(); it != indestructibleWalls[i].end(); ++it){
			if ((*it) == nullptr) continue;
			(*it)->SetPlayerPos(PlayerPos);
			(*it)->Update(camera);
		}
	}
}
void AreaManager::IndestructibleWallDraw()
{
	for(size_t i = 0; i < indestructibleWalls.size(); i++){
		for(auto it = indestructibleWalls[i].begin(); it != indestructibleWalls[i].end(); ++it){
			if((*it) == nullptr) continue;
			(*it)->Draw();
		}
	}
}
void AreaManager::IndestructibleWallFinalize()
{
	delete IndestructibleWallModel;
	IndestructibleWallModel = nullptr;

	delete IndestructibleWallColliderModel;
	IndestructibleWallColliderModel = nullptr;

	for(size_t i = 0; i < indestructibleWalls.size(); i++){
		for(auto it = indestructibleWalls[i].begin(); it != indestructibleWalls[i].end(); ++it){
			if((*it) == nullptr) continue;
			(*it)->Finalize();
			delete (*it);
			(*it) = nullptr;
		}
	}
}
#pragma endregion

#pragma region ダンジョン自動生成
void AreaManager::CreateMap()
{
	for(int y = 0; y < DIV_NUM; y++){
		mapInfo.resize(y+1);
		for(int x = 0; x < DIV_NUM; x++){
			mapInfo[y].push_back(3);
		}
	}

	DevideRoom({0,0,DIV_NUM,DIV_NUM}, false);
}
AreaManager::Room AreaManager::DevideRoom(Area area, bool hr)
{
	if(area.Width < 8 || area.Height < 8) {
		return CreateRoom(area);
	}

	if(!hr){
		//領域の高さ 1/3~2/3の範囲でランダム
		//24 : 1/3 = 8, 2/3 = 16
		//分割ライン水平
		int divline = static_cast<int>(floor(area.Height/3 + rand()%(area.Height/3)));

		//親(上)
		Area parent = {area.X,area.Y, area.Width,divline};
		//子(下)
		Area child = {area.X,area.Y+divline, area.Width,area.Height-divline};

		//親領域に部屋作成
		Room parentRoom = CreateRoom(parent);
		//子領域に部屋作成
		Room childRoom = DevideRoom(child, !hr);

		//通路開拓
		ConnectRoom(parentRoom, childRoom, divline, hr);

		return parentRoom;
	}
	else if(hr){
		//領域の高さ 1/3~2/3の範囲でランダム
		//48 : 1/3 = 16, 2/3 = 32
		//分割ライン水平
		//int divline = rand()%(area.Width/3)+ area.Width/3;
		int divline = static_cast<int>(floor(area.Width/3 + rand()%(area.Width/3)));

		//親(上)
		Area parent = {area.X,area.Y, divline,area.Height};
		//子(下)
		Area child = {area.X+divline,area.Y, area.Width-divline,area.Height};

		//親領域に部屋作成
		Room parentRoom = CreateRoom(parent);
		//子領域に部屋作成
		Room childRoom = DevideRoom(child, !hr);

		//通路開拓
		ConnectRoom(parentRoom, childRoom, divline, hr);

		return parentRoom;
	}
	return CreateRoom(area);
}
AreaManager::Room AreaManager::CreateRoom(Area area)
{
//範囲内でランダムに4点を取得
	int X1 = rand()%(area.Width) + area.X;
	int X2 = rand()%(area.Width) + area.X;
	int Y1 = rand()%(area.Height) + area.Y;
	int Y2 = rand()%(area.Height) + area.Y;

	//Xの始点、小さいほう
	int X = min(X1,X2);
	//差分が幅
	int W = abs(X1 - X2) + 2;
	//Yの始点、小さいほう
	int Y = min(Y1,Y2);
	//差分が高さ
	int H = abs(Y1 - Y2) + 2;

	Room room = {X,Y,W,H};
	rooms.push_back(room);

	//マップに反映
	for(int y = 0; y < H-1; y++){
		for(int x = 0; x < W-1; x++){
			mapInfo[Y+y][X+x] = 1;
		}
	}

	return room;
}
void AreaManager::ConnectRoom(Room parent, Room childRoom, int divline, bool hr)
{
	if(hr){
		//親部屋ないからランダムに一点を選択
		int X1 = parent.X + rand()%(parent.Width-1);
		//子部屋
		int X2 = childRoom.X + rand()%(childRoom.Width-1);
		//小さい
		int minX = min(X1,X2);
		//大きい
		int maxX = max(X1,X2);

		//マップに分割ライン上の通路作成
		for(int i = 0; minX + i < maxX; i++){
			mapInfo[divline][minX + i] = 1;
		}
		//分割ラインから親部屋への通路
		for(int i = 1; mapInfo[divline-i][X1] == 3 && divline - i != 0; i++){
			mapInfo[divline-i][X1] = 1;
		}
		//子部屋へ
		for(int i = 1; mapInfo[divline+i][X2] == 3 && divline + i != 30; i++){
			mapInfo[divline+i][X2] = 1;
		}
	}
	else if(!hr){
		//親部屋ないからランダムに一点を選択
		int Y1 = parent.Y + rand()%(parent.Height-1);
		//子部屋
		int Y2 = childRoom.Y + rand()%(childRoom.Height-1);
		//小さい
		int minY = min(Y1,Y2);
		//大きい
		int maxY = max(Y1,Y2);

		//マップに分割ライン上の通路作成
		for(int i = 0; minY + i < maxY; i++){
			mapInfo[minY + i][divline] = 1;
		}
		//分割ラインから親部屋への通路
		for(int i = 1; mapInfo[Y1][divline-i] == 3 && divline - i != 0; i++){
			mapInfo[Y1][divline-i] = 1;
		}
		//子部屋へ
		for(int i = 1; mapInfo[Y2][divline+i] == 3 && divline + i != 30; i++){
			mapInfo[Y2][divline+i] = 1;
		}
	}
}
#pragma endregion

#pragma region CSVデータ ダンジョン生成
void AreaManager::CSVMapDataLoad(string fullPath)
{
	string directoryPath = "Resources/CSVData/";
	string csv = ".csv";
	//file開く
	ifstream file;
	string Path = directoryPath + fullPath + csv;
	file.open(Path);
	assert(file.is_open());

	//file培養を文字列ストリームにコピー
	csvCommands << file.rdbuf();
	file.close();

	//一行分の文字列を入れる変数
	string line;
	int y = 0;
	int x = 0;

	float start = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	//コマンドループ
	while(getline(csvCommands, line)){
		//一行分のっ文字列をストリームに変換して解析しやすく
		istringstream line_stream(line);

		string word;
		string area;

		//区切りで行の銭湯文字列取得
		getline(line_stream, word, ',');

		x = 0;
		mapInfo.resize(y+1);
		for(int i = 0; i < DIV_NUM*DIV_NUM; i++){

			if(word.find("0") == 0){
				mapInfo[y].push_back(0);
				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("1") == 0){
				mapInfo[y].push_back(1);
				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("2") == 0){
				mapInfo[y].push_back(2);
				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("3") == 0){
				mapInfo[y].push_back(3);
				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("4") == 0){
				mapInfo[y].push_back(1);

				pos = {start + x, start + y};
				pos *= Block_Size;
				PlayerPopPosition = {pos.x, -3, -pos.y};

				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("5") == 0){
				mapInfo[y].push_back(1);

				pos = {start + x, start + y};
				pos *= Block_Size;
				ObjectPos.push_back({pos.x,0,-pos.y});
				ObjectPopActive.push_back(true);

				getline(line_stream, word, ',');
				x++;
			}
			else if(word.find("6") == 0){
				mapInfo[y].push_back(1);

				pos = {start + x, start + y};
				pos *= Block_Size;
				exitPosition = {pos.x,-5,-pos.y};

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

#pragma region ランダム生成
void AreaManager::ObjectRandomPop()
{
	int roomSize = (int)rooms.size();
	Vector2 areaPos;
	Vector2 areaWH;

	float start = -DIV_NUM_HALF_FLOAT;

	//exit
	int exitRoomsNum = rand()%roomSize;
	areaPos = {(start+rooms[exitRoomsNum].X), (start+rooms[exitRoomsNum].Y)};
	areaWH = {float(rand()%(rooms[exitRoomsNum].Width-1)),float(rand()%(rooms[exitRoomsNum].Height-1))};
	areaPos*= Block_Size;
	areaWH *= Block_Size;
	exitPosition = {areaPos.x+areaWH.x,-5.f,-(areaPos.y+areaWH.y)};

	//player
	int playerRoomsNum = rand()%roomSize;
	areaPos = {(start+rooms[playerRoomsNum].X),(start+rooms[playerRoomsNum].Y)};
	areaWH = {float(rand()%(rooms[playerRoomsNum].Width-1)),float(rand()%(rooms[playerRoomsNum].Height-1))};
	areaPos*= Block_Size;
	areaWH *= Block_Size;
	PlayerPopPosition = {areaPos.x+areaWH.x,-3.f,-(areaPos.y+areaWH.y)};
}

Vector3 AreaManager::GetObjectPopPosition()
{
	int roomSize = (int)rooms.size();
	Vector2 areaPos;
	Vector2 areaWH;

	float start = -DIV_NUM_HALF_FLOAT;
	int RoomsNum = rand()%roomSize;

	areaPos = {(start+rooms[RoomsNum].X),(start+rooms[RoomsNum].Y)};
	areaWH = {float(rand()%(rooms[RoomsNum].Width-1)),float(rand()%(rooms[RoomsNum].Height-1))};
	areaPos*= Block_Size;
	areaWH *= Block_Size;
	ObjectPopPosition = {areaPos.x+areaWH.x,0.f,-(areaPos.y+areaWH.y)};

	return ObjectPopPosition;
}
#pragma endregion

void AreaManager::DigParticlePop()
{
	IsDigApp = true;
	for (int i = 0; i < 3; i++) {
		const float rnd_vel = 0.08f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = 0.06f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		acc.y = -0.005f;

		DigParticle->ParticleSet(DigAppearanceFrame,DigParticlePos,vel,acc,0.4f,0.0f,1,{0.5f,0.3f,0.2f,1.f});
		DigParticle->ParticleAppearance();
	}
}