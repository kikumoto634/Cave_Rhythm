#pragma once

//前方宣言
class SkeltonState;
class Skelton;

class SkeltonStateManager
{
public:
	~SkeltonStateManager();

	static SkeltonStateManager* GetInstance();
	static void Delete();

	void SetNextState(SkeltonState* nextState)	{nextState_ = nextState;}

	void Update(Skelton* skelton);

private:
	static SkeltonStateManager* instance;

	SkeltonState* state_ = nullptr;
	SkeltonState* nextState_ = nullptr;
};

