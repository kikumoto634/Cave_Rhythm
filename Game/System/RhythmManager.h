#pragma once

#include <time.h>

class RhythmManager
{
	//�����o�֐�
public:
	void InitializeMeasurement(clock_t _clock);

	//�v���J�n
	void StartMeasurement(clock_t _clock);

	//�X�V
	void Update();

	//�^�C�~���O����
	bool InputBeat();

	//���Y������
	bool JudgeRhythm();


	//Getter
	//�v������
	inline double GetTimer()	{return timer;}
	//�������v������
	inline double GetInitializeTime()	{return InitTimer;}
	//�r�[�g�I���^�C�~���O
	inline bool GetIsRhythm()	{return IsRhythm;}
	//�I�����̎���
	inline double GetBeatTime() {return messureTime;}
	//���͌덷
	inline double GetHighTime()	{return HighTime;}
	inline double GetLowTime()	{return LowTime;}
	//BPM
	inline double GetBPM()	{return bpm;}
	//���͎���
	inline double GetInputTime()	{return inputTime;}


	//Setter
	inline void SetBPM(double _bpm)	{bpm = _bpm;}


	//�����o�ϐ�
private:
	clock_t clock;

	//�v���J�n
	bool IsMeasurement = false;

	//clock��double��
	double InitTimer = 0;
	double timer = 0;
	//�^�C�}�[�̐���
	double timer_Interger = 1;	
	//���Y���t���O
	bool IsRhythm = false;

	//���Y���J�E���g���̎���
	double messureTime = 0;

	//BPM�v�Z��
	double BPMTime = (1*secondFrame/bpm);
	//���̓��Y���̐U�ꕝ
	double HighTime = 0;
	double LowTime = 0;

	//BPM
	double bpm = 60;
	//��b(frame)
	double secondFrame = 60;


	//����
	double inputTime;
};

