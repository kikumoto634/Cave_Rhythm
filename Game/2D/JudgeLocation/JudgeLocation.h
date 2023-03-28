#pragma once
#include "../BaseSprites.h"

class JudgeLocation : public BaseSprites
{
private:
	const Vector2 Position = {640,600};
	const Vector2 Size_Max = {156,156};
	const Vector2 Size_Min = {128,128};
	const Vector2 AnchorPoint = {0.5f,0.5f};

	//最大サイズテクスチャ番号
	const int SizeMaxTextureNumber = 31;
	//最小サイズテクスチャ番号
	const int SizeMinTextureNumber = 22;

public:
	void Initialize();

	void Update(bool &IsInput);

	void Draw() override;
	
	void Finalize() override;
};

