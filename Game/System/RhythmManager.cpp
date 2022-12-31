#include "RhythmManager.h"


void RhythmManager::InitializeMeasurement(clock_t clock)
{
	InitTimer = static_cast<double>(clock)/CLOCKS_PER_SEC;
}

void RhythmManager::StartMeasurement(clock_t clock)
{
	timer = static_cast<double>(clock)/CLOCKS_PER_SEC - InitTimer;
}

void RhythmManager::Update()
{
	//���Z�b�g
	IsRhythm = false;

	//BPM����������
	if(timer >= timer_Interger){
		BPMTime = (1*secondFrame/bpm);
		SubRhythm = (BPMTime)/2.5;

		//���ߏI��(���Y���J�E���g���̎���)
		messureTime = timer;
		//���͐U�ꕝ�v�Z(60BPM ��b�Ɉ�� 0.25)
		HighTime = messureTime + 0.2;
		LowTime = messureTime - 0.35;
		//�t���O
		IsRhythm = true;
		//�b��(����)�X�V
		timer_Interger += BPMTime;
	}
}
