#include "BossState.h"
#include "BossStateManager.h"
#include "Boss1.h"
#include "Easing.h"
#include <cassert>

using namespace std;

void BossState::Initialize(Boss1 *boss)
{
	assert(boss);
	boss_ = boss;
}



void IdelBossState::UpdateTrigger()
{
}

void IdelBossState::Update()
{
	if(boss_->GetIsEvent()) return;
	if(!boss_->isBeatEnd_) return;

	waitCount_++;
	if(waitCount_ <= WaitCountMax) return;
	waitCount_ = 0;
	stateManager_->SetNextState(new TrackBossState);
}

void IdelBossState::ParticleDraw()
{
}



void SummonBossState::UpdateTrigger()
{
}

void SummonBossState::Update()
{
}

void SummonBossState::ParticleDraw()
{
}



void TrackBossState::UpdateTrigger()
{
	easigStartPos_ = boss_->world_.translation;
	//移動先計算
	mapPath_ = boss_->mapInfo_;
	eX_ = +int(boss_->world_.translation.x/AreaBlockSize)+AreaBlocksHalfNum;
	eY_ = -int(boss_->world_.translation.z/AreaBlockSize)+AreaBlocksHalfNum;
	pX_ = +int(boss_->playerPos_.x/AreaBlockSize)+AreaBlocksHalfNum;
	pY_ = -int(boss_->playerPos_.z/AreaBlockSize)+AreaBlocksHalfNum;
	path_ = boss_->PathSearch(boss_->mapInfo_, eX_,eY_, pX_,pY_);

	int root = RootPathStartNumber;
	for(auto it = path_.begin(); it != path_.end(); it++){
		mapPath_[(*it)->y][(*it)->x] = root;
		root++;
	}
	mapPath_[eY_][eX_] = RootPathSkeltonNumber;
	mapPath_[pY_][pX_] = RootPathPlayerNumber;

	//上下左右のルート先
    int dx[PathDirection] = {-1, 1, 0, 0};
    int dy[PathDirection] = {0, 0, -1, 1};

    //四方向をforで調べる
    for (int j = 0; j < PathDirection; j++) {
        //dx,dy方向のルート移動
        int next_x = eX_ + dx[j];
        int next_y = eY_ + dy[j];
		if(next_x < 0 || next_y < 0 || 31 <= next_x || 31 <= next_y) return;

        if(mapPath_[next_y][next_x] == pathRoot_ || mapPath_[next_y][next_x] == RootPathPlayerNumber){
			easingEndPos_ = boss_->world_.translation + Vector3{dx[j]*AreaBlockSize, 0.f, -dy[j]*AreaBlockSize};
			eX_ = next_x;
			eY_ = next_y;

			//回転
			if(j == 0){//Left
				boss_->world_.rotation.y = LeftAngle;
			}
			else if(j == 1){//Right
				boss_->world_.rotation.y = RightAngle;
			}
			else if(j == 2){//down
				boss_->world_.rotation.y = DownAngle;
			}
			else if(j == 3){//up
				boss_->world_.rotation.y = UpAngle;
			}

			if(mapPath_[next_y][next_x] == RootPathPlayerNumber) {
				pathRoot_ = RootPathGoalNumber;

			}
			else if(mapPath_[next_y][next_x] == pathRoot_){
				pathRoot_++;
			}
			break;
		}

		easingEndPos_ = boss_->world_.translation;
    }
}

void TrackBossState::Update()
{
	//移動処理
	boss_->world_.translation = Easing_Point2_Linear(easigStartPos_, easingEndPos_, Time_OneWay(easingMoveTime_, EasingMoveTimeMax));

	//移動完了時
	if(easingMoveTime_ >= EasingFrameMax){
		boss_->world_.translation = easingEndPos_;
		easigStartPos_ = {};
		easingEndPos_ = {};
		easingMoveTime_ = 0.f;
		
		stateManager_->SetNextState(new IdelBossState);
	}
}

void TrackBossState::ParticleDraw()
{
}



void RunAwayBossState::UpdateTrigger()
{
}

void RunAwayBossState::Update()
{
}

void RunAwayBossState::ParticleDraw()
{
}



void DeadBossState::UpdateTrigger()
{
	boss_->particlePos_ = boss_->GetPosition();
	boss_->SetPosition(boss_->NotAlivePos);
	boss_->world_.UpdateMatrix();
	boss_->baseCollider_->Update();

	boss_->isContactTrigger_ = true;

	App();
}

void DeadBossState::Update()
{
	boss_->deadParticle_->Update(boss_->camera_);
	boss_->particleAliveFrame_++;

	if(boss_->particleAliveFrame_ < ParticleAliveFrameMax) return;

}

void DeadBossState::ParticleDraw()
{
	boss_->deadParticle_->Draw();
}

void DeadBossState::App()
{
	for (int i = 0; i < CreateNum; i++) {
		//速度
		vel.x = (float)rand() / RAND_MAX * Rand_Vel - Rand_Vel_Half;
		vel.y = VelY;
		vel.z = (float)rand() / RAND_MAX * Rand_Vel - Rand_Vel_Half;

		//加速度
		acc.y = AccY;

		//生成
		boss_->deadParticle_->ParticleSet(
			ParticleAliveFrameMax,
			boss_->particlePos_,
			vel,
			acc,
			SizeStart,
			SizeEnd,
			TextureNumber,
			Color
		);
		boss_->deadParticle_->ParticleAppearance();
	}
}

