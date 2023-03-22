#pragma once
#include "../BaseSprites.h"

class JudgeLocation : public BaseSprites
{
private:
	const Vector2 Position = {640,600};
	const Vector2 Size_Max = {156,156};
	const Vector2 Size_Min = {128,128};
	const Vector2 AnchorPoint = {0.5f,0.5f};

	//�ő�T�C�Y�e�N�X�`���ԍ�
	const int SizeMaxTextureNumber = 31;
	//�ŏ��T�C�Y�e�N�X�`���ԍ�
	const int SizeMinTextureNumber = 22;

public:
	void Initialize();

	void Update(bool &IsInput);

	void Draw() override;
	
	void Finalize() override;
};

