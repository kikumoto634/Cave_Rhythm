#pragma once
#include "Audio.h"
#include "LightGroup.h"

#include "BaseSprites.h"
#include "combo.h"

#include <string>
#include <vector>

class GameManager
{
private:
	const int PlaneColorChangeConboNum = 10;

	//XビートでPOPターン
	const int EnemyPopBeatTurn = 6;

	//1ビートでの敵POP数
	const int EnemyPopCreateNum = 1;

	//2Dテキスト
	const int NumberSpSize = 3;
	const int TexNumberBegin = 3;

public:
	//初期化
	void Initialize();
	//後処理
	void Finalize();

	//コンボ加算
	void ComboIncrement();
	//コンボリセット
	void ComboReset();
	//コンボ数取得
	inline int GetComboNum()	{return combo->GetComboNum();}
	//地面色変化コンボ数
	inline int GetPlaneColorChangeCombo()	{return PlaneColorChangeConboNum;}

	//コイン加算
	void CoinIncrement();
	//コイン数取得
	inline int GetCoinNum()	{return coinNum;}

	//体力初期設定
	void InitializeSetHp(int _hp);
	//体力スプライトリズムフラグ
	void IsBeatEndOn()	{IsHpScaleChange = true;}
	//体力減少
	void HpDecrement();

	//オーディオ再生
	void AudioPlay(int number, float volume = 1.f, bool loop = false);

	//ライト更新
	void LightUpdate(bool IsPlayerShadowDead = false);
	//プレイヤー丸影の設置
	void PlayerCircleShadowSet(Vector3 pos);

	//敵生成
	int EnemyPopTurnCount();
	Vector2 EnemyRandomPos(const int groundWidth, const float Plane_Size);
	Vector2 EnemyRandomDir(Vector2 pos);

	//2Dスプライト更新
	void SpriteUpdate();

	//2Dスプライト描画
	void SpriteDraw();

private:
	//オーディオ初期化
	void AudioInitialize();

	//ライト初期化
	void LightInitialize();

	//スプライト初期化
	void SpriteInitialize();

private:
	
	//コイン
	int coinNum = 0;

	//体力
	int hp = 0;

	//オーディオ
	Audio* audio = nullptr;

	//ライト
	LightGroup* lightGroup = nullptr;
	//丸影
	//プレイヤー
	bool IsPlayerShadowAlive = false;
	Vector3 circleShadowDir = { 0,-1,0 };
	Vector3 circleShadowAtten = { 0.5f,0.6f,0.0f };
	Vector2 circleShadowFactorAngle = { 0.0f, 0.5f };

	//敵のランダム生成
	//XビートでPOPターン
	int currentEnemyPopBeatTurn = 0;

	//コンボ
	std::unique_ptr<Combo> combo;

	//獲得コイン
	std::unique_ptr<BaseSprites> coinSp;
	std::unique_ptr<BaseSprites> numberSp_coin[3];

	//体力
	const int HpSpSize = 5;
	int DamageHpSpriteIndex = 0;
	std::vector<std::unique_ptr<BaseSprites>> hpSp;
	bool IsHpScaleChange = false;
};

