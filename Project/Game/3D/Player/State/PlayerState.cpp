#include "PlayerState.h"
#include "PlayerStateManager.h"
#include "Easing.h"
#include "Player.h"

#include <cassert>

void PlayerState::Initialize(Player *player)
{
	assert(player);
	player_ = player;
}


void IdelPlayerState::Update()
{
	if(player_->isDamage_) return;
}

void MovePlayerState::Update()
{
	//移動処理
	player_->world.translation = Easing_Linear_Point2(player_->easigStartPos_, player_->easingEndPos_, Time_OneWay(player_->easingMoveTime_, player_->EasingMoveTimeMax));

	//モデル変更
	player_->object->SetModel(player_->model);

	//移動完了時
	if(player_->easingMoveTime_ >= 1.0f){
		player_->world.translation = player_->easingEndPos_;
		player_->easigStartPos_ = {};
		player_->easingEndPos_ = {};
		player_->easingMoveTime_ = 0.f;
		
		stateManager_->SetNextState(new IdelPlayerState);
	}
}

void AttackPlayerState::Update()
{
	player_->weapon_->Attack();
	//モデル変更
	player_->object->SetModel(player_->attackModel_);

	stateManager_->SetNextState(new IdelPlayerState);
}

void DigPlayerState::Update()
{
	player_->weapon_->Attack();
	//モデル変更
	player_->object->SetModel(player_->attackModel_);

	stateManager_->SetNextState(new IdelPlayerState);
}

void DeadPlayerState::Update()
{
	//初回のみ
	if(player_->isDead_) player_->damageFrame_ ++;
	player_->isDead_ = true;

	player_->object->SetModel(player_->deadModel_);
}
