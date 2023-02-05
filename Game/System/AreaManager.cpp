#include "AreaManager.h"

using namespace std;

const float AreaManager::Block_Size = 2.f;

void AreaManager::Initialize()
{
	CreateMap();
	ObjectRandomPop();

	WallsInitialize();
	PlaneInitialize();
}

void AreaManager::Update(Camera* camera, Vector3 PlayerPos)
{
	assert(camera);

	this->camera = camera;
	this->PlayerPos = PlayerPos;

	WallUpdate();
	PlaneUpdate();
}

void AreaManager::BeatEndUpdate(GameManager* gameManager)
{
	assert(gameManager);

	this->gameManager = gameManager;

	PlaneBeatEndUpdate();
}

void AreaManager::Draw()
{
	WallDraw();
	PlaneDraw();
}

void AreaManager::Finalize()
{
	WallFinalize();
	PlaneFinalize();
}

#pragma region 地面
void AreaManager::PlaneInitialize()
{
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j] = make_unique<Planes>();
			plane[i][j]->Initialize("GroundBlock");
			plane[i][j]->SetPosition({ float(-((DIV_NUM/2)*Block_Size) + (i*Block_Size)) ,-5 ,float(-((DIV_NUM/2)*Block_Size) + (j*Block_Size))});
		}
	}
}

void AreaManager::PlaneUpdate()
{
	//地面
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j]->SetPlayerPos(PlayerPos);
			plane[i][j]->Update(camera);
		}
	}
}

void AreaManager::PlaneBeatEndUpdate()
{
	IsComboColorChange = !IsComboColorChange;
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			//コンボ数に応じて色変化
			if(gameManager->GetComboNum() >= gameManager->GetPlaneColorChangeCombo()){
						
				int changePos = i%2+j%2;
				if(changePos == 0 || changePos == 2)		{plane[i][j]->PlaneColorChange(true, IsComboColorChange);}
				if(changePos == 1)							{plane[i][j]->PlaneColorChange(false, IsComboColorChange);}
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
			plane[i][j]->Draw();
		}
	}
}

void AreaManager::PlaneFinalize()
{
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			plane[i][j]->Finalize();
		}
	}
}
#pragma endregion

#pragma region 壁
void AreaManager::WallsInitialize()
{
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			Wall[i][j] = new Planes();
			Wall[i][j]->Initialize("GroundBlock2");
			if(WallMap[i][j] == '*') {
				float startPos = float(-(DIV_NUM/2)*Block_Size);
				Vector3 pos = {startPos + (i*Block_Size) ,-3 ,startPos + (j*Block_Size)};
				Wall[i][j]->SetPosition(pos);
			}
			else{
				Wall[i][j]->IsNotAlive();
			}
		}
	}
}

void AreaManager::WallUpdate()
{
	//地面
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			Wall[i][j]->SetPlayerPos(PlayerPos);
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
			Wall[i][j]->Draw();
		}
	}
}

void AreaManager::WallFinalize()
{
	for(int i = 0; i < DIV_NUM; i++){
		for(int j = 0; j < DIV_NUM; j++){
			Wall[i][j]->Finalize();
			delete Wall[i][j];
			Wall[i][j] = nullptr;
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
	if(area.Width < 10 || area.Height < 10) {
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
void AreaManager::ObjectRandomPop()
{
	int roomSize = (int)rooms.size();

	//出口
	int exitRoomsNum = rand()%roomSize;
	exitPosition = {float(-((DIV_NUM/2)*Block_Size)+(rooms[exitRoomsNum].Y*Block_Size)),-5.f,float(-((DIV_NUM/2)*Block_Size)+(rooms[exitRoomsNum].X*Block_Size))};

	//プレイヤー
	int playerRoomsNum = rand()%roomSize;
	PlayerPopPosition = {float(-((DIV_NUM/2)*Block_Size)+(rooms[playerRoomsNum].Y*Block_Size)),-3.f,float(-((DIV_NUM/2)*Block_Size)+(rooms[playerRoomsNum].X*Block_Size))};
}
#pragma endregion