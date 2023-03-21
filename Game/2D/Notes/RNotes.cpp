#include "RNotes.h"
#include "../../../Engine/math/Easing/Easing.h"

void RNotes::Initialize(UINT textureNumber)
{
	Notes::Initialize(textureNumber);
	SetPosition(R_Start_Pos);
}

void RNotes::Update(float goalTime)
{
	if(!IsNoteAlive) return;

	//ˆÚ“®
	if(!IsMoveStop){

		easingSp = goalTime*3;
		easingTime = (easingSp*60.f);

		//–ß‚µ
		if(GetPosition().x >= Delete_Pos.x){
			IsNoteAlive = false;
			curBeatTime = 0;
			return;
		}

		//ˆÚ“®
		curBeatTime += 1.f/easingTime;
		easingPos = Easing_Linear_Point2(R_Start_Pos,R_End_Pos,curBeatTime);

		SetPosition(easingPos);
	}

	Notes::Update(goalTime);
}

void RNotes::BeatUpdate()
{
	if(IsNoteAlive) return;

	SetPosition(R_Start_Pos);
	Notes::BeatUpdate();
}
