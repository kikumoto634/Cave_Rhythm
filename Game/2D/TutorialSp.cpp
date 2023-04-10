#include "TutorialSp.h"

void TutorialSp::Update()
{
	Vector3 lpos = PlayerPos - targetPos;
	distance = lpos.length();

	//距離に応じたハイド
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
