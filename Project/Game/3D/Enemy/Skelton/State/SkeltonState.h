#pragma once

//前方宣言
class SkeltonStateManager;
class Skelton;

class SkeltonState
{
public:
	virtual void SetStateManager(SkeltonStateManager* stateManager)	{stateManager_ = stateManager;}

	void Initialize(Skelton* skelton);
	virtual void Update() = 0;

protected:
	//借り物
	SkeltonStateManager* stateManager_ = nullptr;
	Skelton* skelton_ = nullptr;
};

//待機
class IdelSkeltonState : public SkeltonState{
private:
	void Update() override;
};

//追跡
class TrackSkeltonState : public SkeltonState{
private:
	void Update() override;
};

//攻撃
class AttackSkeltonState : public SkeltonState{
private:
	void Update() override;
};

//死亡
class DeadSkeltonState : public SkeltonState{
private:
	void Update() override;
};

//出現
class PopSkeltonState : public SkeltonState{
private:
	void Update() override;
};

