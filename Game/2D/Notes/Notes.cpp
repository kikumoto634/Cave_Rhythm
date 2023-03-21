#include "Notes.h"
#include "../../../Engine/math/Easing/Easing.h"

void Notes::Initialize(UINT textureNumber)
{
	BaseSprites::Initialize(textureNumber);
	SetAnchorPoint(Notes_Anc);
	SetSize(Notes_Size);
}

void Notes::InputUpdate()
{
	IsMoveStop = true;
}

void Notes::Update(float goalTime)
{
	//’âŽ~
	if(IsMoveStop){
		this->color.w = Easing_Linear_Point2(1,0,Time_OneWay(curAlphaFrame, AlphaMaxTime));

		if(this->color.w <= 0){
			IsNoteAlive = false;
			curBeatTime = 0;
			curAlphaFrame = 0;
			this->color.w = 1;
			SetPosition(L_Start_Pos);
		}
	}

	BaseSprites::Update();
}

void Notes::BeatUpdate()
{
	BaseSprites::Update();
	IsNoteAlive = true;
	IsMoveStop = false;
}

void Notes::Draw()
{
	if(!IsNoteAlive)return;
	BaseSprites::Draw();
}

void Notes::Finalize()
{
	BaseSprites::Finalize();
}
