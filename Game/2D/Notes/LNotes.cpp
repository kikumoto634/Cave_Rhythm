#include "LNotes.h"
#include "../../../Engine/math/Easing/Easing.h"

void LNotes::Initialize(UINT textureNumber)
{
	Notes::Initialize(textureNumber);
	SetPosition(L_Start_Pos);
}

void LNotes::Update(float goalTime)
{
	if(!IsNoteAlive) return;

	//�ړ�
	if(!IsMoveStop){

		easingSp = goalTime*3;
		easingTime = (easingSp*60.f);

		//�߂�
		if(GetPosition().x <= Delete_Pos.x){
			IsNoteAlive = false;
			curBeatTime = 0;
			return;
		}

		//�ړ�
		curBeatTime += 1.f/easingTime;
		easingPos = Easing_Linear_Point2(L_Start_Pos,L_End_Pos,curBeatTime);

		SetPosition(easingPos);
	}

	Notes::Update(goalTime);
}

void LNotes::BeatUpdate()
{
	if(IsNoteAlive) return;

	SetPosition(L_Start_Pos);
	Notes::BeatUpdate();
}
