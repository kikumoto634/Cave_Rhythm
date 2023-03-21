#pragma once
#include "../BaseSprites.h"

class Notes : public BaseSprites
{
protected:
	const Vector2 L_Start_Pos = {1288,600};
	const Vector2 R_Start_Pos = {-8,600};

	//�C�[�W���O�ڕW�n
	const Vector2 L_End_Pos = {650,600};
	const Vector2 R_End_Pos = {630,600};
	//�m�[�c�폜�ʒu
	const Vector2 Delete_Pos = {640,600};

	const Vector2 Notes_Size = {32,80};
	const Vector2 Notes_Anc = {0.5,0.5};

	//���ߗ��ω�����
	const float AlphaMaxTime = 0.5f;

public:
	virtual void Initialize(UINT textureNumber);
	
	void InputUpdate();

	virtual void Update(float goalTime);
	virtual void BeatUpdate();

	void Draw() override;
	
	void Finalize() override;

	//Getter
	inline bool GetIsNoteAlive()	{return IsNoteAlive;}

protected:
	//����
	bool IsNoteAlive = false;
	//��~
	bool IsMoveStop = false;

	//���ߗ��ω�����
	float curAlphaFrame = 0.f;


	//�ړ��p�^�C��
	float curBeatTime = 0;
	//�ړ�����
	float easingTime = 0;

	//�C�[�W���O���W
	Vector2 easingPos = {};

	//�ړ����x
	float easingSp = 0;
};

