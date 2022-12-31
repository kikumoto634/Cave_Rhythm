#include "RhythmManager.h"
#include "../../Engine/input/Input.h"

void RhythmManager::InitializeMeasurement(clock_t _clock)
{
	this->clock = _clock;
	InitTimer = static_cast<double>(clock)/CLOCKS_PER_SEC;
}

void RhythmManager::StartMeasurement(clock_t _clock)
{
	this->clock = _clock;
	timer = (static_cast<double>(this->clock)/CLOCKS_PER_SEC) - InitTimer;
}

void RhythmManager::Update()
{
	//���Z�b�g
	IsRhythm = false;

	Input* input = nullptr;
	input = Input::GetInstance();
	if(input->Trigger(DIK_Z)){
		bool IsFlag = true;

		if(messureTime == inputTime){
			bool IsAA = true;
		}

	}

	//BPM����������
	if(timer >= timer_Interger){
		BPMTime = (1*secondFrame/bpm);

		//���ߏI��(���Y���J�E���g���̎���)
		messureTime = timer;
		//���͐U�ꕝ�v�Z(60BPM ��b�Ɉ�� 0.25)
		HighTime = messureTime + (static_cast<double>((60/bpm)/4))*2;
		LowTime = messureTime - (static_cast<double>((60/bpm)/4));
		//�t���O
		IsRhythm = true;
		//�b��(����)�X�V
		timer_Interger += BPMTime;
	}
}

bool RhythmManager::InputBeat()
{
	inputTime = (static_cast<double>(clock)/CLOCKS_PER_SEC) - InitTimer;
	return true;
}

bool RhythmManager::JudgeRhythm()
{
	if(inputTime <= HighTime && LowTime <= inputTime){
		return true;
	}
	if(messureTime == inputTime){
		return true;
	}
	return false;
}
