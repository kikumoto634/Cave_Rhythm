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

	RandamAreaWallsInitialize();
	RandamAreaPlaneInitialize();
	RandamAreaIndestructibleWallInitialize();
}

void AreaManager::CSVAreaInitialize(string name)
{
	CSVMapDataLoad(name);

	CommonInitialize();

	CSVAreaPlaneInitialize();
	CVSAreaWallsInitialize();
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

	DigParticle = new ParticleObject();
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
	delete DigParticle;
}

#pragma region 地面
void AreaManager::RandamAreaPlaneInitialize()
{
	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j] = new Planes();
			plane[i][j]->Initialize(PlaneModel);
			pos = {startPos + i, startPos + j};
			pos *= Block_Size;
			plane[i][j]->SetPosition({pos.x,-5,pos.y});
			plane[i][j]->CaveLightOn();
		}
	}
}
void AreaManager::CSVAreaPlaneInitialize()
{
	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(CSVMap[i][j] == 0){
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
	//地面
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
			if(plane[i][j] == nullptr){
				IsChange = !IsChange; 
				continue;
			}
			//コンボ数に応じて色変化
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

#pragma region 壁
void AreaManager::RandamAreaWallsInitialize()
{
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
			Wall[i][j]->CaveLightOn();
		}
	}
}
void AreaManager::CVSAreaWallsInitialize()
{
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
	//地面
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(Wall[i][j] == nullptr) continue;

			Wall[i][j]->SetPlayerPos(PlayerPos);
			if(Wall[i][j]->GetIsDIg()){
				IsDigSound = true;
				IsDig =true;
				DigParticlePos = Wall[i][j]->GetDigPosition();
				gameManager->AudioPlay(10);
			}
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

#pragma region 破壊不可壁
void AreaManager::RandamAreaIndestructibleWallInitialize()
{
	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};
	for(int i = 0; i <DIV_NUM; i++){
		//上
		IndestructibleWalls[0][i] = new IndestructibleWall();
		IndestructibleWalls[0][i]->Initialize(IndestructibleWallModel, IndestructibleWallColliderModel);
		pos = {startPos + i, startPos - 1};
		pos*=Block_Size;
		IndestructibleWalls[0][i]->SetPosition({pos.x, -3,pos.y});
		IndestructibleWalls[0][i]->CaveLightOn();
		//下
		IndestructibleWalls[1][i] = new IndestructibleWall();
		IndestructibleWalls[1][i]->Initialize(IndestructibleWallModel, IndestructibleWallColliderModel);
		pos = {startPos + i, DIV_NUM_HALF_FLOAT+1};
		pos*=Block_Size;
		IndestructibleWalls[1][i]->SetPosition({pos.x, -3,pos.y});
		IndestructibleWalls[1][i]->CaveLightOn();
		////左
		IndestructibleWalls[2][i] = new IndestructibleWall();
		IndestructibleWalls[2][i]->Initialize(IndestructibleWallModel, IndestructibleWallColliderModel);
		pos = {startPos - 1, startPos + i};
		pos*=Block_Size;
		IndestructibleWalls[2][i]->SetPosition({pos.x, -3,pos.y});
		IndestructibleWalls[2][i]->CaveLightOn();
		////右
		IndestructibleWalls[3][i] = new IndestructibleWall();
		IndestructibleWalls[3][i]->Initialize(IndestructibleWallModel, IndestructibleWallColliderModel);
		pos = {DIV_NUM_HALF_FLOAT+1, startPos + i};
		pos*=Block_Size;
		IndestructibleWalls[3][i]->SetPosition({pos.x, -3,pos.y});
		IndestructibleWalls[3][i]->CaveLightOn();
	}
}
void AreaManager::CSVAreaIndestructibleWallInitialize()
{
	float startPos = -DIV_NUM_HALF_FLOAT;
	Vector2 pos = {};

	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			if(CSVMap[i][j] != 2){
				continue;
			}

			IndestructibleWalls[i][j] = new IndestructibleWall();
			IndestructibleWalls[i][j]->Initialize(IndestructibleWallModel, IndestructibleWallColliderModel);
			pos = {startPos + i ,startPos + j};
			pos *= Block_Size;
			IndestructibleWalls[i][j]->SetPosition({pos.x,-3,pos.y});
		}
	}
}
void AreaManager::IndestructibleWallUpdate()
{
	//地面
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

	delete IndestructibleWallColliderModel;
	IndestructibleWallColliderModel = nullptr;

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

#pragma region ダンジョン自動生成
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
		//親部屋ないからランダムに一点を選択
		int X1 = parent.X + rand()%parent.Width;
		//子部屋
		int X2 = childRoom.X + rand()%childRoom.Width;
		//小さい
		int minX = min(X1,X2);
		//大きい
		int maxX = max(X1,X2);

		//マップに分割ライン上の通路作成
		for(int i = 0; minX + i <= maxX; i++){
			WallMap[divline][minX + i] = 'A';
		}
		//分割ラインから親部屋への通路
		for(int i = 1; WallMap[divline-i][X1] == '*'; i++){
			WallMap[divline-i][X1] = 'A';
		}
		//子部屋へ
		for(int i = 1; WallMap[divline+i][X2] == '*'; i++){
			WallMap[divline+i][X2] = 'A';
		}
	}
	else if(!hr){
		//親部屋ないからランダムに一点を選択
		int Y1 = parent.Y + rand()%parent.Height;
		//子部屋
		int Y2 = childRoom.Y + rand()%childRoom.Height;
		//小さい
		int minY = min(Y1,Y2);
		//大きい
		int maxY = max(Y1,Y2);

		//マップに分割ライン上の通路作成
		for(int i = 0; minY + i <= maxY; i++){
			WallMap[minY + i][divline] = 'A';
		}
		//分割ラインから親部屋への通路
		for(int i = 1; WallMap[Y1][divline-i] == '*'; i++){
			WallMap[Y1][divline-i] = 'A';
		}
		//子部屋へ
		for(int i = 1; WallMap[Y2][divline+i] == '*'; i++){
			WallMap[Y2][divline+i] = 'A';
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

#pragma region ランダム生成
void AreaManager::ObjectRandomPop()
{
	int roomSize = (int)rooms.size();
	Vector2 areaPos;
	Vector2 areaWH;

	float start = -DIV_NUM_HALF_FLOAT;
	//出口
	int exitRoomsNum = rand()%roomSize;
	//プレイヤー
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