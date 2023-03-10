#include "Notes.h"
#include "../../Engine/math/Easing/Easing.h"

void Notes::Initialize(UINT textureNumber)
{
	BaseSprites::Initialize(textureNumber);
	SetAnchorPoint(Notes_Anc);
	SetPosition(Start_Pos);
	SetSize(Notes_Size);
}

void Notes::InputUpdate()
{
	IsNoteAlive = false;
	curBeatTime = 0;
	SetPosition(Start_Pos);
	BaseSprites::Update();
}

void Notes::Update(float goalTime)
{
	if(!IsNoteAlive) return;
	easingSp = goalTime*3;
	easingTime = (easingSp*60.f);

	//–ß‚µ
	if(GetPosition().x <= Delete_Pos.x){
		IsNoteAlive = false;
		curBeatTime = 0;
		return;
	}

	//ˆÚ“®
	curBeatTime += 1.f/easingTime;
	easingPos = Easing_Linear_Point2(Start_Pos,End_Pos,curBeatTime);

	SetPosition(easingPos);
	BaseSprites::Update();
}

void Notes::BeatUpdate()
{
	if(IsNoteAlive) return;

	SetPosition(Start_Pos);
	BaseSprites::Update();
	IsNoteAlive = true;
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
