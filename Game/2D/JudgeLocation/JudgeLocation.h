#pragma once
#include "../BaseSprites.h"

class JudgeLocation : public BaseSprites
{
private:
	const Vector2 Position = {640,600};
	const Vector2 Size_Max = {128,128};
	const Vector2 Size_Min = {100,100};
	const Vector2 AnchorPoint = {0.5f,0.5f};

public:
	void Initialize(UINT textureNumber) override;

	void Update(bool &IsInput);

	void Draw() override;
	
	void Finalize() override;
};

