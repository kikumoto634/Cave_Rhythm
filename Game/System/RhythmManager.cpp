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

void RhythmManager::InputRhythm()
{
	inputJudgeTime = timer;
}

void RhythmManager::BeatMoveUp()
{
	IsRhythmEnd = false;
	//�J��オ��l��萮���������傫���Ȃ�����J��オ��
	if(timer >= moveUpNumber){

		//�x�[�X����
		judgeTimeBase = timer;

		IsRhythmEnd = true;
		moveUpNumber += 1;
	}
}

bool RhythmManager::JudgeRhythm()
{
	if(inputJudgeTime <= (judgeTimeBase + 0.25) && judgeTimeBase <= inputJudgeTime){
		return true;
	}

	return false;
}
