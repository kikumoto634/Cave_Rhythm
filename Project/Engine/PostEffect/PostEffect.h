﻿#pragma once
#include "Sprite.h"
class PostEffect : public Sprite
{
public:
	PostEffect(UINT texnumber, Vector3 pos, Vector2 size, XMFLOAT4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY);

	static PostEffect* Create(UINT texNumber, Vector2 pos, XMFLOAT4 color = {1,1,1,1},
		Vector2 anchorpoint = {0.f,0.f}, bool isFlipX = false, bool isFlipY = false);

	void Draw();
};

