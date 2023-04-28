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
