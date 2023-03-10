#pragma once
#include "BaseSprites.h"

class Notes : public BaseSprites
{
private:
	const Vector2 Start_Pos = {1288,600};

	//イージング目標地
	const Vector2 End_Pos = {670,600};
	//ノーツ削除位置
	const Vector2 Delete_Pos = {640,600};

	const Vector2 Notes_Size = {16,80};
	const Vector2 Notes_Anc = {0.5,0.5};

public:
	void Initialize(UINT textureNumber) override;
	
	void InputUpdate();

	void Update(float goalTime);
	void BeatUpdate();

	void Draw() override;
	
	void Finalize() override;

	//Getter
	inline bool GetIsNoteAlive()	{return IsNoteAlive;}

private:
	//生存
	bool IsNoteAlive = false;

	//移動用タイム
	float curBeatTime = 0;
	//移動時間
	float easingTime = 0;

	//イージング座標
	Vector2 easingPos = {};

	//移動速度
	float easingSp = 0;
};

