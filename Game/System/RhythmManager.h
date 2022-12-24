#pragma once

#include <time.h>

class RhythmManager
{
	//�����o�֐�
public:
	//�v���J�n
	void StartMeasurement(clock_t clock);

	//�X�V
	void Update();


	//Getter
	inline double GetTimer()	{return timer;}
	inline double GetTimer_Interger()	{return timer_Interger;}
	inline bool GetIsRhythm()	{return IsRhythm;}
	inline double GetMessureTime()	{return messureTime;}
	inline double GetSubRhythm()	{return SubRhythm;}
	inline double GetHighTime()	{return HighTime;}
	inline double GetLowTime()	{return LowTime;}
	inline double GetBPM()	{return bpm;}

	//Setter
	inline void SetBPM(double _bpm)	{bpm = _bpm;}


	//�����o�ϐ�
private:
	//�v���J�n
	bool IsMeasurement = false;

	//clock��double��
	double timer = 0;
	//�^�C�}�[�̐���
	double timer_Interger = 10;	
	//���Y���t���O
	bool IsRhythm = false;

	//���Y���J�E���g���̎���
	double messureTime = 0;

	//BPM�v�Z��
	double BPMTime = (1*secondFrame/bpm);
	//���̓��Y���̐U�ꕝ��
	double SubRhythm = (BPMTime)/2.5;
	//���̓��Y���̐U�ꕝ
	double HighTime = SubRhythm*1.25;
	double LowTime = SubRhythm*0.3;

	//BPM
	double bpm = 60;
	//��b(frame)
	double secondFrame = 60;
};

