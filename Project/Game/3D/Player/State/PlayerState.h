#pragma once

//前方宣言
class PlayerStateManager;
class Player;

//状態
class PlayerState{
public:
	virtual void SetStateManager(PlayerStateManager* stateManager)	{stateManager_ = stateManager;}

	void Initialize(Player* player);
	virtual void Update() = 0;

protected:
	//借り物
	PlayerStateManager* stateManager_ = nullptr;
	Player* player_ = nullptr;
};

//通常
class IdelPlayerState   : public PlayerState{
private:
	void Update() override;
};

//移動
class MovePlayerState   : public PlayerState{
private:
	void Update() override;
};

//攻撃
class AttackPlayerState : public PlayerState{
private:
	void Update() override;
};

//掘る
class DigPlayerState    : public PlayerState{
private:
	void Update() override;
};

//死亡
class DeadPlayerState   : public PlayerState{
private:
	void Update() override;
};