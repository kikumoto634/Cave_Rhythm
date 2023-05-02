#include "Skelton.h"
#include "SkeltonStateManager.h"
#include "SkeltonState.h"

#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace DirectX;
using namespace std;

void Skelton::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//拡縮最小値
	ScaleMin = {0.7f, 0.7f, 0.7f};

	state_ = SkeltonStateManager::GetInstance();
	//state_->SetNextState(new IdelSkeltonState);
	state_->SetNextState(new PopSkeltonState);

    //コライダー
    ColliderInitialize();

    //パーティクル
    ParticleInitialize();
}

void Skelton::Update(Camera *camera, const Vector3& playerPos)
{
	this->camera = camera;
	playerPos_ = playerPos;

    state_->Update(this);

    if(isDead_) return;
	//距離に応じた更新
	DistanceUpdate();
	//アクション更新
	ActionUpdate();
	//ビート更新
	BeatUpdate();

	BaseObjObject::Update(camera);
}

void Skelton::Draw()
{
	if(isPosImposibble_ || isInvisible_ || isDead_) return;

	BaseObjObject::Draw();
}

void Skelton::ParticleDraw()
{
    state_->ParticleDraw();
}

void Skelton::Finalize()
{
    deadParticle_->Finalize();
    popParticle_->Finalize();

	BaseObjObject::Finalize();
}

void Skelton::OnCollision(const CollisionInfo &info)
{
    if(isDead_) return;
    if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
        isDead_ = true;
    }
}

void Skelton::Pop(Vector3 pos)
{
    particlePos_ = pos;
	isPosImposibble_ = false;
}

bool Skelton::GetIsDeadTrigger()
{
    //Trigger
    if(isDeadTrigger_){
        isDeadTrigger_ = false;
        return true;
    }
    return false;
}



void Skelton::ActionUpdate()
{
    //拡縮
	if(isScaleChange_ && ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
		isScaleChange_ = false;
	}

    //コライダー
    if(collider){
        sphereCollider_->Update();
    }
}

void Skelton::BeatUpdate()
{
	if(!IsBeatEnd) return;
    IsBeatEnd = false;

    //拡縮
    isScaleChange_ = true;
}

void Skelton::DistanceUpdate()
{
	//距離
	Vector3 sub = playerPos_ - world.translation;
	distance_ = sub.length();

	//光計算
	if(isLightCal){
		//距離範囲内
		if(-DrawingRange_Half <= distance_ && distance_ <= DrawingRange_Half){
			object->OnLighting();
		}
		else if(-DrawingRange_Half > distance_ || distance_ > DrawingRange_Half){
			object->OffLighting();
		}
	}
	else if(!isLightCal){
		object->OnLighting();
	}

	//範囲が非表示
	if(-DrawingRange_Not <= distance_ && distance_ <= DrawingRange_Not)		{
		isInvisible_ = false;
	}
	else if(-DrawingRange_Not > distance_ || distance_ > DrawingRange_Not)	{
		isInvisible_ = true;
	}
}


vector<MapNode*> Skelton::PathSearch(vector<vector<int>> &grid, int start_x, int start_y, int end_x, int end_y)
{
	//パス
    vector<MapNode*> path;
    //未探索ノード
    priority_queue<MapNode*> frontier;
    //調べたノードをマーク
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));

    //スタートノード設定
    MapNode* start_node = new MapNode(start_x,start_y, 0,Heuristic(start_x, start_y, end_x, end_y), nullptr);
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
            if (next_x < 0 || next_y < 0 || next_x >= grid[0].size() || next_y >= grid[0].size()) {
                continue;
            }

            //常に調べ済み
            if (visited[next_y][next_x]) {
                continue;
            }

            //障害物
            if (grid[next_y][next_x] != 1) {
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


void Skelton::ColliderInitialize()
{
    SetCollider(new SphereCollider(XMVECTOR{0,0,0,0}, colliderRadius_));
    collider->SetAttribute(COLLISION_ATTR_ENEMYS);
    //球コライダー取得
	sphereCollider_ = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider_);
    collider->Update();
}

void Skelton::ColliderSet()
{
    collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void Skelton::ColliderRemove()
{
    //コリジョンマネージャーから登録を解除する
	CollisionManager::GetInstance()->RemoveCollider(collider);
}


void Skelton::ParticleInitialize()
{
    deadParticle_ = make_unique<ParticleObject>();
    deadParticle_->Initialize();

    popParticle_ = make_unique<ParticleObject>();
    popParticle_->Initialize();
}

