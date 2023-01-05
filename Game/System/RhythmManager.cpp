#include "RhythmManager.h"
#include "../../Engine/input/Input.h"

void RhythmManager::InitializeMeasurement(clock_t _clock)
{
	this->clock = _clock;
	InitTimer = static_cast<double>(clock)/CLOCKS_PER_SEC;
}

void RhythmManager::StartMeasurement(clock_t _clock)
{
	BPMTime = (1*secondFrame/bpm);
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
	//繰り上がり値より整数部分が大きくなったら繰り上がり
	if(timer >= moveUpNumber){

		//ベース時間
		judgeTimeBase = timer;

		IsRhythmEnd = true;
		moveUpNumber += BPMTime;
	}
}

bool RhythmManager::HighJudgeRhythm()
{
	double BeatTime = BPMTime/4;

	if(inputJudgeTime <= (judgeTimeBase + BeatTime) && judgeTimeBase <= inputJudgeTime){
		return true;
	}
	return false;
}

bool RhythmManager::LowJudgeRhythm()
{
	double BeatTime = BPMTime;

	if(inputJudgeTime <= (judgeTimeBase + BeatTime) && (judgeTimeBase + BeatTime - BeatTime/4) <= inputJudgeTime){
		return true;
	}
	return false;
}
