#pragma once

#include <time.h>

class RhythmManager
{
	//メンバ関数
public:
	void InitializeMeasurement(clock_t _clock);

	//計測開始
	void StartMeasurement(clock_t _clock);

	//更新
	void Update();

	//タイミング入力
	bool InputBeat();

	//リズム判定
	bool JudgeRhythm();


	//Getter
	//計測時間
	inline double GetTimer()	{return timer;}
	//初期化計測時間
	inline double GetInitializeTime()	{return InitTimer;}
	//ビート終了タイミング
	inline bool GetIsRhythm()	{return IsRhythm;}
	//終了時の時間
	inline double GetBeatTime() {return messureTime;}
	//入力誤差
	inline double GetHighTime()	{return HighTime;}
	inline double GetLowTime()	{return LowTime;}
	//BPM
	inline double GetBPM()	{return bpm;}
	//入力時間
	inline double GetInputTime()	{return inputTime;}


	//Setter
	inline void SetBPM(double _bpm)	{bpm = _bpm;}


	//メンバ変数
private:
	clock_t clock;

	//計測開始
	bool IsMeasurement = false;

	//clockをdoubleに
	double InitTimer = 0;
	double timer = 0;
	//タイマーの整数
	double timer_Interger = 1;	
	//リズムフラグ
	bool IsRhythm = false;

	//リズムカウント時の時間
	double messureTime = 0;

	//BPM計算式
	double BPMTime = (1*secondFrame/bpm);
	//入力リズムの振れ幅
	double HighTime = 0;
	double LowTime = 0;

	//BPM
	double bpm = 60;
	//一秒(frame)
	double secondFrame = 60;


	//入力
	double inputTime;
};

