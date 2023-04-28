#include "Skelton.h"
#include "SkeltonStateManager.h"
#include "SkeltonState.h"

void Skelton::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	state_ = SkeltonStateManager::GetInstance();
	state_->SetNextState(new IdelSkeltonState);
}

void Skelton::Update(Camera *camera)
{
	//アクション更新
	state_->Update(this);

	BaseObjObject::Update(camera);
}

void Skelton::Draw()
{

	BaseObjObject::Draw();
}

void Skelton::Finalize()
{
	BaseObjObject::Finalize();
}

void Skelton::OnCollision(const CollisionInfo &info)
{
}


void Skelton::ActionUpdate()
{
}

void Skelton::BeatUpdate()
{
}

void Skelton::PathSearch()
{
}

void Skelton::ColliderUpdate()
{
}
