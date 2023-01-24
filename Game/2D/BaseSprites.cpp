#include "BaseSprites.h"

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
