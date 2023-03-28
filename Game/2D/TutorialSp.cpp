#include "TutorialSp.h"

void TutorialSp::Initialize(UINT textureNumber)
{
	BaseSprites::Initialize(textureNumber);
}

void TutorialSp::Update()
{
	Vector3 lpos = PlayerPos - targetPos;
	distance = lpos.length();

	if(-13 <= distance && distance <= 13) IsHide = false;
	else if(-13 > distance || distance > 13) IsHide = true;

	if(IsHide) return;
	BaseSprites::Update();
}

void TutorialSp::Draw()
{
	if(IsHide) return;
	BaseSprites::Draw();
}

void TutorialSp::Finalize()
{
	BaseSprites::Finalize();
}
