#include "BaseSprites.h"
#include "../../../Engine/math/Easing/Easing.h"

void BaseSprites::Initialize(UINT textureNumber)
{
	sprite = Sprite::Create(textureNumber, position);
}

void BaseSprites::Update()
{
	sprite->SetPosition(position);
	sprite->SetSize(size);
	sprite->SetColor(color);
}

void BaseSprites::Draw()
{
	sprite->Draw();
}

void BaseSprites::Finalize()
{
	delete sprite;
	sprite = nullptr;
}

bool BaseSprites::ScaleChange(Vector2 sizeMax, Vector2 sizeMin, float EndTime)
{
	float ease = -(cosf(3.14159265f * scaleCurrentTime) - 1.f)/2.f;
	scale = Easing_Linear_Point2(sizeMin, sizeMax, ease);
	SetSize(scale);

	if(scaleCurrentTime >= 1.0f){
		scale = sizeMax;
		scaleCurrentTime = 0.f;
		return true;
	}

	scaleCurrentTime += 1.f/(60*EndTime);
	return false;
}
