#include "RhythmManager.h"

using namespace std::chrono;

void RhythmManager::Initialize()
{
	//1小節時間の計算
	beatTime_ = 60.0 / BPM;
	//差分時間
	beatSub_ = beatTime_/SUB;
}

void RhythmManager::PreUpdate()
{
	IsJustRhythm = false;

	//リズム値更新
	if(inputTimeTarget_ + beatSub_ <= calTime_.count()){
		inputTimeTarget_ += beatTime_;
		IsRhythmMoveUp = false;
	}

	if(!IsStart) return ;

	//リズムピッタリ時
	if(IsRhythmMoveUp) return;
	if(inputTimeTarget_ <= calTime_.count()){
		IsJustRhythm = true;
		IsRhythmMoveUp = true;
	}
}

void RhythmManager::PostUpdate()
{
	if(!IsStart) return;

	//1ループ処理時間
	endTime_ = system_clock::now();

	//最終時間計測
	calTime_ = endTime_ - startTime_;
	inputTime_ = inputEndTime_ - startTime_;
}

void RhythmManager::TimeStart()
{
	startTime_ = system_clock::now();
	IsStart = true;
}

void RhythmManager::InputTime()
{
	inputEndTime_ = std::chrono::system_clock::now();
}

bool RhythmManager::IsInputRhythmJudge()
{
	if(inputTime_.count() >= inputTimeTarget_ - beatSub_ && 
		inputTime_.count() <= inputTimeTarget_ + beatSub_){
		return true;
	}

	return false;
}
