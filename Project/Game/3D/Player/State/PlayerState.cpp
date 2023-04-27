#include "PlayerState.h"
#include "PlayerStateManager.h"
#include "Easing.h"
#include "Player.h"

void IdelPlayerState::Update(Player* player)
{
	player_ = player;
}

void MovePlayerState::Update(Player* player)
{
	player_ = player;

	//移動処理
	player_->world.translation = Easing_Linear_Point2(player_->easigStartPos_, player_->easingEndPos_, Time_OneWay(player_->easingMoveTime_, player_->EasingMoveTimeMax));

	//移動完了時
	if(player_->easingMoveTime_ >= 1.0f){
		player_->world.translation = player_->easingEndPos_;
		player_->easigStartPos_ = {};
		player_->easingEndPos_ = {};
		player_->easingMoveTime_ = 0.f;
		
		stateManager_->SetNextState(new IdelPlayerState);
	}
}

void AttackPlayerState::Update(Player *player)
{
	player_ = player;

	player_->weapon_->Attack();
	stateManager_->SetNextState(new IdelPlayerState);
}

void DigPlayerState::Update(Player *player)
{
	player_ = player;

	player_->weapon_->Attack();
	stateManager_->SetNextState(new IdelPlayerState);
}
