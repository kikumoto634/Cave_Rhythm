#include "SkeltonState.h"
#include "SkeltonStateManager.h"
#include "Easing.h"
#include "Skelton.h"

#include <cassert>

using namespace DirectX;
using namespace std;

void SkeltonState::Initialize(Skelton *skelton)
{
	assert(skelton);
	skelton_ = skelton;
}



void IdelSkeltonState::UpdateTrigger()
{
}

void IdelSkeltonState::Update()
{
	//ビート時
	if(!skelton_->IsBeatEnd) return;

	//待機カウント終了時
	if(waitCount_ > WaltCountMax){
		waitCount_ = 0;
		stateManager_->SetNextState(new TrackSkeltonState);
	}

	waitCount_++;
}



void TrackSkeltonState::UpdateTrigger()
{
	easigStartPos_ = skelton_->world.translation;
	//移動先計算
	mapPath_ = skelton_->mapInfo_;
	eX_ = int(skelton_->world.translation.x/2)+15;
	eY_ = -int(skelton_->world.translation.z/2)+15;
	pX_ = int(skelton_->playerPos_.x/2)+15;
	pY_ = -int(skelton_->playerPos_.z/2)+15;
	path_ = skelton_->PathSearch(skelton_->mapInfo_, eX_,eY_, pX_,pY_);

	int root = 9;
	for(auto it = path_.begin(); it != path_.end(); it++){
		mapPath_[(*it)->y][(*it)->x] = root;
		root++;
	}
	mapPath_[eY_][eX_] = 5;
	mapPath_[pY_][pX_] = 6;

	//上下左右のルート先
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    //四方向をforで調べる
    for (int j = 0; j < 4; j++) {
        //dx,dy方向のルート移動
        int next_x = eX_ + dx[j];
        int next_y = eY_ + dy[j];


        if(mapPath_[next_y][next_x] == pathRoot_ || mapPath_[next_y][next_x] == 6){
			easingEndPos_ = skelton_->world.translation + Vector3{dx[j]*2.0f, 0.f, -dy[j]*2.0f};
			eX_ = next_x;
			eY_ = next_y;

			//回転
			if(j == 0){//Left
				skelton_->world.rotation.y = XMConvertToRadians(-90);
			}
			else if(j == 1){//Right
				skelton_->world.rotation.y = XMConvertToRadians(90);
			}
			else if(j == 2){//down
				skelton_->world.rotation.y = 0;
			}
			else if(j == 3){//up
				skelton_->world.rotation.y = XMConvertToRadians(180);
			}

			if(mapPath_[next_y][next_x] == 6) {
				pathRoot_ = 10;

			}
			else if(mapPath_[next_y][next_x] == pathRoot_){
				pathRoot_++;
			}
			break;
		}

		easingEndPos_ = skelton_->world.translation;
    }
}

void TrackSkeltonState::Update()
{
	//死亡時
	if(skelton_->isDead_){
		stateManager_->SetNextState(new DeadSkeltonState);
	}

	//移動処理
	skelton_->world.translation = Easing_Linear_Point2(easigStartPos_, easingEndPos_, Time_OneWay(easingMoveTime_, EasingMoveTimeMax));

	//移動完了時
	if(easingMoveTime_ >= 1.0f){
		skelton_->world.translation = easingEndPos_;
		easigStartPos_ = {};
		easingEndPos_ = {};
		easingMoveTime_ = 0.f;
		
		stateManager_->SetNextState(new IdelSkeltonState);
	}
}



void AttackSkeltonState::UpdateTrigger()
{
}

void AttackSkeltonState::Update()
{
}



void DeadSkeltonState::UpdateTrigger()
{
	particlePos_ = skelton_->GetPosition();
	skelton_->SetPosition(deadPos_);

	App();
}

void DeadSkeltonState::Update()
{
	//更新処理
	skelton_->particle_->Update(skelton_->camera);
	particleAliveFrame++;

	//距離計算
	Vector3 sub = (skelton_->playerPos_ - particlePos_);
	skelton_->distance_ = sub.length();

	//出現
	if(particleAliveFrame < ParticleAliveFrameMax) return;
	particleAliveFrame = ParticleAliveFrameMax;

	if(skelton_->distance_ < PopDistance) return;
	//再出現
	stateManager_->SetNextState(new IdelSkeltonState);
	skelton_->isDead_ = false;	
	skelton_->isPosImposibble_ = true;	
}

void DeadSkeltonState::App()
{
	for (int i = 0; i < 10; i++) {
		const float rnd_vel = 0.08f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = 0.06f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;

		Vector3 acc{};
		acc.y = -0.005f;

		skelton_->particle_->ParticleSet(ParticleAliveFrameMax,particlePos_,vel,acc,0.4f,0.0f,1,{1.f,0.0f,0.0f,1.f});
		skelton_->particle_->ParticleAppearance();
	}
}



void PopSkeltonState::UpdateTrigger()
{
	skelton_->ColliderRemove();
}

void PopSkeltonState::Update()
{
}

