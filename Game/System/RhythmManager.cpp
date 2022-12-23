#include "RhythmManager.h"


void RhythmManager::StartMeasurement(clock_t clock)
{
	timer = static_cast<double>(clock)/CLOCKS_PER_SEC;
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
		//���͐U�ꕝ�v�Z
		HighTime = messureTime + SubRhythm*1.5;
		LowTime = messureTime - SubRhythm*1.5;
		//�t���O
		IsRhythm = true;
		//�b��(����)�X�V
		timer_Interger += BPMTime;
	}
}