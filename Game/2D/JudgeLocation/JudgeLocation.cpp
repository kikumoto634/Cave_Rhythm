#include "JudgeLocation.h"

void JudgeLocation::Initialize(UINT textureNumber)
{
	BaseSprites::Initialize(textureNumber);
	SetAnchorPoint(AnchorPoint);
	SetPosition(Position);
	SetSize(Size_Max);
}

void JudgeLocation::Update(bool &IsInput)
{
	if(IsInput){
		if(ScaleChange(Size_Max, Size_Min)){
			IsInput = false;
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
