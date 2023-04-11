#pragma once
#include "BaseSprites.h"

class TutorialSp : public BaseSprites
{
public:
	void Update() override;
	void Draw() override;

	void SpriteHide()	{IsHide = true, Draw();}

	//Setter
	inline void SetPlayerPos(Vector3 pos)	{PlayerPos = pos;}

private:
	float distance;
	Vector3 PlayerPos ={};
	bool IsHide = false;
};

