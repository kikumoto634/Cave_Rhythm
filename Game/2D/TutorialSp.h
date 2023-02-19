#pragma once
#include "BaseSprites.h"

class TutorialSp : public BaseSprites
{
public:
	void Initialize(UINT textureNumber) override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	void SpriteHide()	{IsHide = true, Draw();}

	//Setter
	inline void SetPlayerPos(Vector3 pos)	{PlayerPos = pos;}

private:
	float distance;
	Vector3 PlayerPos ={};
	bool IsHide = false;
};

