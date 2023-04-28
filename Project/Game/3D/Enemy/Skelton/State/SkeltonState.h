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

