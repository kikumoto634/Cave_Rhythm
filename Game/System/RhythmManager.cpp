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
	//リセット
	IsRhythm = false;

	//BPM分立ったら
	if(timer >= timer_Interger){
		BPMTime = (1*secondFrame/bpm);
		SubRhythm = (BPMTime)/2.5;

		//小節終了(リズムカウント時の時間)
		messureTime = timer;
		//入力振れ幅計算
		HighTime = messureTime + SubRhythm*1.75;
		LowTime = messureTime - SubRhythm*1.5;
		//フラグ
		IsRhythm = true;
		//秒数(整数)更新
		timer_Interger += BPMTime;
	}
}
