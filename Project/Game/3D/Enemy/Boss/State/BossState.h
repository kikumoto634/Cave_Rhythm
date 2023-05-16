#pragma once

class BossStateManager;
class Boss1;

class BossState
{
public:
	virtual void SetStateManager(BossStateManager* stateManager)	{stateManager_ = stateManager;}

	void Initialize(Boss1* boss);
	virtual void UpdateTrigger() = 0;
	virtual void Update() = 0;
	virtual void ParticleDraw() = 0;

protected:
	BossStateManager* stateManager_ = nullptr;
	Boss1* boss_ = nullptr;
};

//待機
class IdelBossState : public BossState{
private:
	void UpdateTrigger() override;
	void Update() override;
	void ParticleDraw() override;
};

//召喚
class SummonBossState : public BossState{
private:
	void UpdateTrigger() override;
	void Update() override;
	void ParticleDraw() override;
};

//追跡
class TrackBossState : public BossState{
private:
	void UpdateTrigger() override;
	void Update() override;
	void ParticleDraw() override;
};

//退避
class RunAwayBossState : public BossState{
private:
	void UpdateTrigger() override;
	void Update() override;
	void ParticleDraw() override;
};

//死亡
class DeadBossState : public BossState{
private:
	void UpdateTrigger() override;
	void Update() override;
	void ParticleDraw() override;
};
