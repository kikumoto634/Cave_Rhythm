#include "SkeltonStateManager.h"
#include "SkeltonState.h"
#include "Skelton.h"


//静的
SkeltonStateManager* SkeltonStateManager::instance = nullptr;

SkeltonStateManager::~SkeltonStateManager()
{
	delete state_;
}

SkeltonStateManager* SkeltonStateManager::GetInstance()
{
	if(!instance)
	{
		instance = new SkeltonStateManager();
	}
	return instance;
}

void SkeltonStateManager::Delete()
{
	if(instance){
		delete instance;
		instance = nullptr;
	}
}

void SkeltonStateManager::Update(Skelton* skelton)
{
	if(nextState_){
		if(state_){
			delete state_;
		}
		state_ = nextState_;

		nextState_ = nullptr;

		state_->SetStateManager(this);
		state_->Initialize(skelton);
		state_->UpdateTrigger();
	}

	state_->Update();
}

void SkeltonStateManager::ParticleDraw()
{
	state_->ParticleDraw();
}
