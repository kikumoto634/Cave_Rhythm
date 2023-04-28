#include "SkeltonState.h"
#include "SkeltonStateManager.h"
#include "Easing.h"
#include "Skelton.h"

#include <cassert>

void SkeltonState::Initialize(Skelton *skelton)
{
	assert(skelton_);
	skelton_ = skelton;
}

void IdelSkeltonState::Update()
{
}

void TrackSkeltonState::Update()
{

}

void AttackSkeltonState::Update()
{
}

void DeadSkeltonState::Update()
{
}

void PopSkeltonState::Update()
{
}
