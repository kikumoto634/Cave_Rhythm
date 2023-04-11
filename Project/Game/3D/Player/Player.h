#pragma once
#include "BaseObjObject.h"
#include "Input.h"
#include "SphereCollider.h"

#include "PlayerWeapon.h"

class Player : public BaseObjObject
{
//定数
//無敵時間
const int DamageFrame = 150;

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

	/// <summary>
	/// 入力成否時の処理
	/// </summary>
	/// <param name="IsFlag">成否</param>
	void JudgeUpdate(bool IsFlag);


	//Sound
	/// <summary>
	/// ダメージサウンド発生
	/// </summary>
	bool DamageSound();

	bool GetIsDead()	{return IsDead;};
	bool GetIsDeadAudioOnce();


	//Getter
	inline int GetHP()	{return HP;}
	inline bool GetIsInputOnce()	{return IsInputOnce;}
	inline bool GetIsNextScene()	{return IsNextScene;}

	//Setter
	inline void SetMoveEasingMaxTime(float time)	{MoveEasingMaxTime = time;}
	inline void SetIsExitOpen(bool IsFlag)	{IsExitOpen = IsFlag;}
	inline void SetIsWait(bool IsFlag)	{IsWait = IsFlag;}
	inline void SetWeaponPos(Vector3 pos)	{offSetWeaponPos = pos;}
	inline void SetHp(int hp)	{HP = hp;}

private:
	//移動
	bool MovementInput();
	void MoveModelSet();

	//攻撃
	bool AttackInput();
	void AttackModelSet();

	//ダメージ
	void Damage();
	//ダメージ更新
	void DamageUpdate();

private:	
	//移動
	Vector3 movePosition = {0,0,0};
	Vector3 moveRotation = {0,0,0};
	bool IsMove = false;

	//過去位置
	Vector3 OldPosition = {};

	//移動イージング
	bool IsMoveEasing = false;
	float moveEasingFrame = 0;
	float MoveEasingMaxTime = 0.075f;
	Vector3 moveEasingPos;

	//レイ
	Vector3 RayDir = {0,0,1};

	//HP
	bool IsDead = false;
	bool IsDeadAudioOnce = false;
	int HP = 5;

	//待機フラグ
	bool IsWait = false;

	//ダメージ
	bool IsDamage = false;
	bool IsDamageSoundOnce = false;
	////無敵時間(フレーム)
	int damageCurrentFrame = 0;

	//攻撃
	bool IsAttack = false;

	//使用モデル判別(false:Move, true:Attack)
	bool IsModelJudge = false;

	//入力
	Input* input = nullptr;
	bool IsInputOnce = false;
	bool IsInputJudge = false;

	//コライダー
	SphereCollider* sphereCollider = nullptr;

	//武器
	PlayerWeapon* weapon = nullptr;
	Vector3 offSetWeaponPos = {0,0,2.5};

	//攻撃モデル
	ObjModelManager* attackModel = nullptr;

	//死亡モデル
	ObjModelManager* deadModel = nullptr;

	//次のシーン
	bool IsNextScene = false;
	bool IsExitOpen = false;
};

