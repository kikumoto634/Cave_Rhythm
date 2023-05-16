#include "BossState.h"
#include "BossStateManager.h"
#include "Boss1.h"

#include <cassert>

void BossState::Initialize(Boss1 *boss)
{
	assert(boss);
	boss_ = boss;
}



void IdelBossState::UpdateTrigger()
{
}

void IdelBossState::Update()
{
}

void IdelBossState::ParticleDraw()
{
}



void SummonBossState::UpdateTrigger()
{
}

void SummonBossState::Update()
{
}

void SummonBossState::ParticleDraw()
{
}



void TrackBossState::UpdateTrigger()
{
}

void TrackBossState::Update()
{
}

void TrackBossState::ParticleDraw()
{
}



void RunAwayBossState::UpdateTrigger()
{
}

void RunAwayBossState::Update()
{
}

void RunAwayBossState::ParticleDraw()
{
}



void DeadBossState::UpdateTrigger()
{
}

void DeadBossState::Update()
{
}

void DeadBossState::ParticleDraw()
{
}

