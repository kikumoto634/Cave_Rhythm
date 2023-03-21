#pragma once
#include "BaseSprites.h"

class Combo
{
private:
	//2Dテキスト
	const int NumberSpSize = 3;
	const int TexNumberBegin = 3;

	//色変化時間
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
	//コンボ
	int comboNum = 0;

	//コンボリセット関連
	bool IsReset = false;
	Vector3 comboSpColor = {1,1,1};
	float curColorChangeFrame = 0;

	//コンボテキスト
	std::unique_ptr<BaseSprites> comboSp;
	//数字(TexNumber 3~12)
	std::unique_ptr<BaseSprites> numberSp_combo[3];
};

