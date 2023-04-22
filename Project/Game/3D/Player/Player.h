﻿#pragma once
#include "BaseObjObject.h"
#include "Input.h"
#include "SphereCollider.h"
#include "PlayerWeapon.h"

class Player : public BaseObjObject
{
//メンバ関数
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	//Getter
	//入力確認
	inline bool GetIsInput()	{return isInput_;}
	//体力
	inline int GetHp()	{return hp_;}
	//死亡
	inline bool GetIsDead()	{return isDead_;}


	//Setter
	//入力判別
	void SetInputJudge(bool isJudge)	{isInputJudge_ = isJudge;}
	//武器のモデル、座標取得
	inline void SetWeaponModelPos(Vector3 pos)	{weaponOffset_ = pos;}
	//体力取得
	inline void SetHp(int value)	{hp_ = value;}
	//扉解放の取得
	inline void SetIsExitOpen(bool isFlag)	{isExitOpen = isFlag;}

private:
	//入力
	//更新
	void InputUpdate();


private:
	//ポインタ
	//入力
	Input* input_ = nullptr;
	//武器
	PlayerWeapon* weapon_ = nullptr;

	//武器
	//オフセット
	Vector3 weaponOffset_ = {};

	//体力
	int hp_ = 0;

	//死亡
	bool isDead_ = false;

	//入力
	//入力確認
	bool isInput_ = false;
	//判別
	bool isInputJudge_ = false;

	//行動フラグ
	//移動
	bool isMove_ = false;
	//キャンセル
	bool isCancel_ = false;

	//移動
	//移動先
	Vector3 addPosition_ = {};

	//扉解放
	bool isExitOpen = false;
};

