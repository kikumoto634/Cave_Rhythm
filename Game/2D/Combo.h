#pragma once
#include "BaseSprites.h"

class Combo
{
private:
	//2D�e�L�X�g
	const int NumberSpSize = 3;
	const int TexNumberBegin = 3;

	//�F�ω�����
	const float ComboResetColorSecond = 0.5f;

public:
	void Initialize();
	
	void Update();

	void Draw();
	
	void Finalize();

	void Increment();
	void Reset();

	//Getter
	inline int GetComboNum()	{return comboNum;}

private:
	//�R���{
	int comboNum = 0;

	//�R���{���Z�b�g�֘A
	bool IsReset = false;
	Vector3 comboSpColor = {1,1,1};
	float curColorChangeFrame = 0;

	//�R���{�e�L�X�g
	std::unique_ptr<BaseSprites> comboSp;
	//����(TexNumber 3~12)
	std::unique_ptr<BaseSprites> numberSp_combo[3];
};

