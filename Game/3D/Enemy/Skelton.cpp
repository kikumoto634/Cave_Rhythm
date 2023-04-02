﻿#include "Skelton.h"
#include "../../Collision/SphereCollider.h"
#include "../../../Engine/base/ParticleManager.h"

#include "../../Collision/CollisionSystem/CollisionManager.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#include "../../../Engine/math/Easing/Easing.h"

using namespace std;

Skelton::~Skelton()
{
}

void Skelton::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);
	ScaleMin = {0.7f,0.7f,0.7f};

	SetPosition(NotAlivePos);
	SetRotation({0,DirectX::XMConvertToRadians(180),0.f});
	world.UpdateMatrix();

	//コライダー
	float radius = 0.6f;
	SetCollider(new SphereCollider(XMVECTOR{0,0.0,0}, radius));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	//球コライダー取得
	sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);
	collider->Update();
	sphereCollider->Update();

	//パーティクル
	DeadParticle = new ParticleObject();
	DeadParticle->Initialize();
	PopParticle = new ParticleObject();
	PopParticle->Initialize();
}

void Skelton::Update(Camera *camera, Vector3 playerPos)
{
	this->camera = camera;
	PlayerPos = playerPos;
	if(!IsNotApp) return;

	//距離計測
	Vector3 pos = PlayerPos - world.translation;
	distance = pos.length();
	if(IsCaveLight){
		if(-DrawingRange_Half <= distance && distance <= DrawingRange_Half){
			object->OnLighting();
		}
		else if(-DrawingRange_Half > distance || distance > DrawingRange_Half){
			object->OffLighting();
		}
	}
	else if(!IsCaveLight){
		object->OnLighting();
	}

	if(-DrawingRange_Not <= distance && distance <= DrawingRange_Not)		{
		IsInvisible = false;
	}
	else if(-DrawingRange_Not > distance || distance > DrawingRange_Not)	{
		IsInvisible = true;
	}

	//出現
	if(IsPop){
		if(IsBeatEnd){
			popCount++;
			IsBeatEnd = false;

			if(popCount >= POP_COUNT){
				SetPosition(PopParticlePos);
				PopParticlePos = {0,50,0};
				popCount = 0;
				IsPop = false;
				IsDead = false;
				return;
			}
		}
	}
	PopParticleApp();
	PopParticle->Update(this->camera);

	//死亡
	if(IsDead && !IsPop){
		if(IsDeadOnceAudio){
			IsDeadOnceAudio = false;
			appearancePopFrame = 0;
		}
		DeadParticleApp();

		if(appearancePopFrame >= AppearanceResetFrame){
			Reset();
			return;
		}
		appearancePopFrame++;
	}
	DeadParticle->Update(this->camera);

	if(IsInvisible) return;
	//生存
	if(!IsDead && !IsPop){
		//拍終了
		if(IsBeatEnd){
			//スケール
			IsScaleEasing  = true;
			//拍終了
			IsBeatEnd = false;

			//待機カウント
			if(waitCount >= WaitCount){
				//移動
				Movement();
				waitCount = 0;
			}
			else{
				waitCount++;
			}
		}

		//スケール遷移
		if(IsScaleEasing){
			if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
				IsScaleEasing = false;
			}
		}
	}
	BaseObjObject::Update(this->camera);
}

void Skelton::Draw()
{
	if(IsDead) return;
	if(IsPop) return;
	if(IsInvisible) return;

	BaseObjObject::Draw();
}

void Skelton::ParticleDraw()
{
	if(IsDead){
		DeadParticle->Draw();
	}
	else if(IsPop){
		PopParticle->Draw();
	}
}

void Skelton::Finalize()
{
	DeadParticle->Finalize();
	delete DeadParticle;
	PopParticle->Finalize();
	delete PopParticle;

	BaseObjObject::Finalize();
}

void Skelton::OnCollision(const CollisionInfo &info)
{
	if(IsDead) return;
	if(IsPop) return;
	if(IsInvisible) return;

	if(info.collider->GetAttribute() == COLLISION_ATTR_WEAPONS){
		IsDead = true;
		IsDeadOnceAudio = true;
		IsDeadOnceParticle = true;

		SetPosition(NotAlivePos);
		world.UpdateMatrix();
		collider->Update();
		sphereCollider->Update();

		DeadParticlePos = info.objObject->GetPosition();
	}
	else if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		SetPosition(NotAlivePos);
		world.UpdateMatrix();
		collider->Update();
		sphereCollider->Update();
		IsDead = true;
	}
}

void Skelton::Pop(Vector3 pos)
{
	PopParticlePos = pos;
	IsNotApp = true;
	IsPop = true;
}

vector<MapNode *> Skelton::find_path(std::vector<std::vector<int>> &grid, int start_x, int start_y, int end_x, int end_y)
{
	//パス
    vector<MapNode*> path;
    //未探索ノード
    priority_queue<MapNode*> frontier;
    //調べたノードをマーク
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));

    //スタートノード設定
    MapNode* start_node = new MapNode(start_x, start_y, 0, Heuristic(start_x, start_y, end_x, end_y), nullptr);
    //未探索エリアの追加
    frontier.push(start_node);

    //未探索エリアが空で解除
    while (!frontier.empty()) {
        //現在地のノードを設定(priority_queueを使用しているので降順で大きい値が取得される)
        MapNode* current_node = frontier.top();
        //現在地で探索されたので、削除
        frontier.pop();

        //現在地がゴールなら終了
        if (current_node->x == end_x && current_node->y == end_y) {
            while (current_node != nullptr) {
                path.push_back(current_node);
                current_node = current_node->parent;
            }
            //取得した順番を逆にする
            reverse(path.begin(), path.end());
            break;
        }

        //現在地に調べたマークをつける
        visited[current_node->y][current_node->x] = true;

        //上下左右のルート先
        int dx[4] = {-1, 1, 0, 0};
        int dy[4] = {0, 0, -1, 1};

        //四方向をforで調べる
        for (int i = 0; i < 4; i++) {
            //dx,dy方向のルート移動
            int next_x = current_node->x + dx[i];
            int next_y = current_node->y + dy[i];

            //マップ範囲外
            if (next_x < 0 || next_y < 0 || next_x >= grid.size() || next_y >= grid[0].size()) {
                continue;
            }

            //常に調べ済み
            if (visited[next_y][next_x]) {
                continue;
            }

            //障害物
            if (grid[next_y][next_x] == 2 || grid[next_y][next_x] == 3) {
                continue;
            }

            //開始ノードからのコスト
            int new_g = current_node->g + 1;
            //ヒューリスティック推定値
            int new_h = Heuristic(next_x, next_y, end_x, end_y);

            //次のノードの取得、未探索エリアに追加
            MapNode* next_node = new MapNode(next_x, next_y, new_g, new_h, current_node);
            frontier.push(next_node);
        }
    }
    return path;
}

void Skelton::Reset()
{
	IsNotApp = false;
	IsDead = false;

	waitCount = 0;
}

void Skelton::Movement()
{
	if(IsInvisible) return;
	if(mapInfo.size() == 0) return;

	//マップ情報の更新
	mapPath = mapInfo;
	int eX = int(GetPosition().x/2)+15;
	int eY = int(GetPosition().z/2)+15;
	int pX = int(PlayerPos.x/2)+15;
	int pY = int(PlayerPos.z/2)+15;

	path = find_path(mapInfo,eY,eX, pY,pX);
	int i = 10;
	for(auto it = path.begin(); it != path.end(); it++){
        mapPath[(*it)->y][(*it)->x] = i;
		i++;
    }

	mapPath[eX][eY] = 5;
	mapPath[pX][pY] = 6;

	if(IsDebug) return ;

	for(int y = 0; y < 31; y++){
		for(int x = 0; x < 31; x++){
			if(mapPath[y][x] == 0) OutputDebugString(L"00,");
			else if(mapPath[y][x] == 1) OutputDebugString(L"01,");
			else if(mapPath[y][x] == 2) OutputDebugString(L"02,");
			else if(mapPath[y][x] == 3) OutputDebugString(L"03,");
			else if(mapPath[y][x] == 5) OutputDebugString(L"05,");
			else if(mapPath[y][x] == 6) OutputDebugString(L"06,");
			else if(mapPath[y][x] == 9) OutputDebugString(L"09,");

			else if(mapPath[y][x] == 10) OutputDebugString(L"10,");
			else if(mapPath[y][x] == 11) OutputDebugString(L"11,");
			else if(mapPath[y][x] == 12) OutputDebugString(L"12,");
			else if(mapPath[y][x] == 13) OutputDebugString(L"13,");
			else if(mapPath[y][x] == 14) OutputDebugString(L"14,");
			else if(mapPath[y][x] == 15) OutputDebugString(L"15,");
			else if(mapPath[y][x] == 16) OutputDebugString(L"16,");
			else if(mapPath[y][x] == 17) OutputDebugString(L"17,");
			else if(mapPath[y][x] == 18) OutputDebugString(L"18,");
			else if(mapPath[y][x] == 19) OutputDebugString(L"19,");
			else if(mapPath[y][x] == 20) OutputDebugString(L"20,");
		}
		OutputDebugString(L"\n");
	}
	IsDebug = true;
}

void Skelton::PopParticleApp()
{
	for (int i = 0; i < 10; i++) {

		//自身の座標を軸に[-1, 1]ランダム
		const Vector3 rnd_pos = PopParticlePos;
		const float range = 1.5f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * range - range/2.0f;
		pos.y = -0.5f;
		pos.z = (float)rand() / RAND_MAX * range - range/2.0f;
		pos += rnd_pos;

		const float rnd_vel = 0.025f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = 0.05f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		PopParticle->ParticleSet(40,pos,vel,acc,0.4f,0.0f,1,{0.6f,0.3f,0.2f,0.4f});
		PopParticle->ParticleAppearance();
	}
}

void Skelton::DeadParticleApp()
{
	if(!IsDeadOnceParticle) return;
	for (int i = 0; i < 10; i++) {
		const float rnd_vel = 0.08f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = 0.06f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		acc.y = -0.005f;

		DeadParticle->ParticleSet(AppearanceResetFrame,DeadParticlePos,vel,acc,0.4f,0.0f,1,{1.f,0.0f,0.0f,1.f});
		DeadParticle->ParticleAppearance();
	}

	IsDeadOnceParticle = false;
}
