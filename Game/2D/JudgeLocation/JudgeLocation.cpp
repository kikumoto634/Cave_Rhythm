#include "JudgeLocation.h"

void JudgeLocation::Initialize()
{
	BaseSprites::Initialize(SizeMinTextureNumber);
	SetAnchorPoint(AnchorPoint);
	SetPosition(Position);
	SetSize(Size_Min);
}

void JudgeLocation::Update(bool &IsInput)
{
	if(IsInput){
		SetTexNumber(SizeMaxTextureNumber);
		if(ScaleChange(Size_Min, Size_Max)){
			IsInput = false;
			SetTexNumber(SizeMinTextureNumber);
		}
	}
	BaseSprites::Update();
}

void JudgeLocation::Draw()
{
	BaseSprites::Draw();
}

void JudgeLocation::Finalize()
{
	BaseSprites::Finalize();
}
