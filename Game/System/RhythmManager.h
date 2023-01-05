#pragma once

#include <time.h>

class RhythmManager
{
	//�����o�֐�
public:
	//���������̌v�Z���ԍ����p
	void InitializeMeasurement(clock_t _clock);

	//�v���J�n
	void StartMeasurement(clock_t _clock);



	//����
	void InputRhythm();

	//�r�[�g����Ԃ�
	void BeatMoveUp();

	bool HighJudgeRhythm();
	bool LowJudgeRhythm();

	//Getter
	//�v������
	inline double GetTimer()	{return timer;}
	//���ʎ��Ԃ̃x�[�X
	inline double GetJudgeTimeBase()	{return judgeTimeBase;}
	//���ʗp���͎���
	inline double GetInputJudgeTime()	{return inputJudgeTime;}
	//�J��オ��p
	inline double GetMoveUpNumber()	{return moveUpNumber;}
	//�r�[�g�^�C�~���O
	inline bool GetIsRhythmEnd()	{return IsRhythmEnd;}

	//�����o�ϐ�
private:
	//include "clock.h"�g�p
	clock_t clock;

	//���������̎��Ԍv�Z����
	double InitTimer = 0;
	//�X�V���̎���(Main)
	double timer = 0;

	//�J��オ��(���Y���^�C�~���O�I��)
	bool IsRhythmEnd = false;
	//�J��オ�蔻�ʗp����
	double moveUpNumber = 0;
	//���ʎ��Ԃ̃x�[�X
	double judgeTimeBase = 0;
	//�O��̔��莞��
	double oldJudgeTimeBase = 0;
	//���ʗp�A���͎���
	double inputJudgeTime = 0;

	//BPM�v�Z��
	double BPMTime = (1*secondFrame/bpm);
	//BPM
	const double bpm = 60;
	//��b(frame)
	const double secondFrame = 60;
};

