#pragma once

//前方宣言
class PlayerStateManager;
class Player;

//状態
class PlayerState{
public:
	virtual void SetStateManager(PlayerStateManager* stateManager)	{stateManager_ = stateManager;}

	virtual void Update(Player* player) = 0;

protected:
	//借り物
	PlayerStateManager* stateManager_ = nullptr;
	Player* player_ = nullptr;
};

//通常
class IdelPlayerState   : public PlayerState{
public:
	void Update(Player* player) override;
};

//移動
class MovePlayerState   : public PlayerState{
public:
	void Update(Player* player) override;
};

//攻撃
class AttackPlayerState : public PlayerState{
public:
	void Update(Player* player) override;
};

//掘る
class DigPlayerState    : public PlayerState{
public:
	void Update(Player* player) override;
};