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
	//リセット
	IsRhythm = false;

	Input* input = nullptr;
	input = Input::GetInstance();
	if(input->Trigger(DIK_Z)){
		bool IsFlag = true;

		if(messureTime == inputTime){
			bool IsAA = true;
		}

	}

	//BPM分立ったら
	if(timer >= timer_Interger){
		BPMTime = (1*secondFrame/bpm);

		//小節終了(リズムカウント時の時間)
		messureTime = timer;
		//入力振れ幅計算(60BPM 一秒に一回 0.25)
		HighTime = messureTime + (static_cast<double>((60/bpm)/4))*2;
		LowTime = messureTime - (static_cast<double>((60/bpm)/4));
		//フラグ
		IsRhythm = true;
		//秒数(整数)更新
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
