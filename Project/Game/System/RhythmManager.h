﻿#pragma once

#include <time.h>

class RhythmManager
{
//定数
private:
	//BPM
	const double bpm = 120;
	//一秒(frame)
	const double secondFrame = 60;
	//判定時間の差分絶対値 BPMタイムの何分割分
	const double RhythmSubTime_Split = 4;

//メンバ関数
public:
	//初期化時の計算時間差分用
	void InitializeMeasurement(clock_t _clock);

	//計測開始
	void StartMeasurement(clock_t _clock);

	//入力
	void InputRhythm();

	//ビートくり返し
	void BeatMoveUp();

	bool HighJudgeRhythm();
	bool LowJudgeRhythm();

	//Getter
	//計測時間
	inline double GetTimer()	{return timer;}
	//判別時間のベース
	inline double GetJudgeTimeBase()	{return judgeTimeBase;}
	//判別用入力時間
	inline double GetInputJudgeTime()	{return inputJudgeTime;}
	//繰り上がり用
	inline double GetMoveUpNumber()	{return moveUpNumber;}
	//ビートタイミング
	inline bool GetIsRhythmEnd()	{return IsRhythmEnd;}
	//リズム判別時の絶対値差分
	inline double GetBPMTimeSub()	{return BPMTimeSub;}
	//1BPMの秒数
	inline double GetBPMTime()	{return BPMTime;}

	//メンバ変数
private:
	//include "clock.h"使用
	clock_t clock;

	//初期化時の時間計算差分
	double InitTimer = 0;
	//更新時の時間(Main)
	double timer = 0;

	//繰り上がり(リズムタイミング終了)
	bool IsRhythmEnd = false;
	//繰り上がり判別用整数
	double moveUpNumber = 0;
	//判別時間のベース
	double judgeTimeBase = 0;
	//前回の判定時間
	double oldJudgeTimeBase = 0;
	//判別用、入力時間
	double inputJudgeTime = 0;

	//BPM計算式
	double BPMTime = (1*secondFrame/bpm);

	//リズム判別時の絶対値差分
	double BPMTimeSub;
};

