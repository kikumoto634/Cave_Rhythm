#pragma once
#include "BaseObjObject.h"
#include "Input.h"
#include "SphereCollider.h"
#include "PlayerWeapon.h"

//前方宣言
class PlayerStateManager;

class Player : public BaseObjObject
{
public:
//フレンド関数
friend class IdelPlayerState;
friend class MovePlayerState;
friend class AttackPlayerState;
friend class DigPlayerState;
friend class DeadPlayerState;

//定数
private:
	//移動距離
	const float MoveLength = 2.0f;
	//レイキャストの長さ
	const float adsDistance = 1.0f;

	//ダメージ状態の総フレーム
	const int DamageFrameMax = 150;

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


	//入力可能状態
	inline void InputPossible()	{isDuplicateLimit_ = false;}

	//Getter
	//ダメージ
	bool GetIsDamage();
	//死亡
	bool GetIsDead();
	//体力
	inline int GetHp()	{return hp_;}
	//入力確認
	inline bool GetIsInput()	{return isInput_;}
	//次のシーン
	inline bool GetIsNextScene()	{return isNextScene_;}

	//Setter
	//入力判別
	void SetInputJudge(bool isJudge)	{isInputJudge_ = isJudge;}
	//武器のモデル、座標取得
	inline void SetWeaponModelPos(Vector3 pos)	{weaponOffset_ = pos;}
	//体力取得
	inline void SetHp(int value)	{hp_ = value;}
	//扉解放の取得
	inline void SetIsExitOpen(bool isFlag)	{isExitOpen_ = isFlag;}

private:

	//初期化
	//コライダー
	void SphereColliderSet();

	//入力
	//更新
	void InputUpdate();
	//移動
	void InputMovement();
	//決定
	void InputDecision();

	//拍終了
	void BeatUpdate();

	//アクション
	void ActionUpdate();

	//ダメージ
	void DamageUpdate();

private:
	//ポインタ
	//入力
	Input* input_ = nullptr;
	//武器
	PlayerWeapon* weapon_ = nullptr;

	//武器
	//オフセット
	Vector3 weaponOffset_ = {};

	//モデル
	ObjModelManager* attackModel_ = nullptr;
	ObjModelManager* deadModel_ = nullptr;

	//状態
	PlayerStateManager* state_ = nullptr;

	//体力
	int hp_ = 0;

	//ダメージ
	bool isDamage_ = false;
	int damageFrame_ = 0;

	//死亡
	bool isDead_ = false;
	
	//移動ベクトル
	Vector3 addVector3_;

	//入力
	//入力確認
	bool isInput_ = false;
	//判別
	bool isInputJudge_ = false;
	//重複入力制限
	bool isDuplicateLimit_ = false;

	//扉解放
	bool isExitOpen_ = false;

	//シーン移動
	bool isNextScene_ = false;

	//コライダー
	SphereCollider* sphereCollider_ = nullptr;
	//レイキャスト
	Ray ray_;
	Vector3 rayCastDir_ = {0,0,-1};
};

