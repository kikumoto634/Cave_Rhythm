#pragma once
#include "BaseSprites.h"

class Notes : public BaseSprites
{
private:
	const Vector2 Start_Pos = {1288,600};

	//�C�[�W���O�ڕW�n
	const Vector2 End_Pos = {670,600};
	//�m�[�c�폜�ʒu
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
	//����
	bool IsNoteAlive = false;

	//�ړ��p�^�C��
	float curBeatTime = 0;
	//�ړ�����
	float easingTime = 0;

	//�C�[�W���O���W
	Vector2 easingPos = {};

	//�ړ����x
	float easingSp = 0;
};

