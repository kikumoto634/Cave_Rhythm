#pragma once
#include "BaseObjObject.h"
#include "Input.h"
#include "SphereCollider.h"
#include "PlayerWeapon.h"

class Player : public BaseObjObject
{
//定数
private:
	//移動距離
	const float MoveLength = 2.0f;
	//移動時の時間
	const float EasingMoveTimeMax = 0.075f;
	//レイキャストの長さ
	const float adsDistance = 1.0f;


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

	//初期化
	//コライダー
	void SphereColliderSet();

	//入力
	//更新
	void InputUpdate();
	void InputMovement();

	//拍終了
	void BeatUpdate();

	//アクション
	void ActionUpdate();

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
	//重複入力制限
	bool isDuplicateLimit_ = false;

	//状態
	enum State{
		None,
		Move,
		Dig,
		Attack
	};

	State state_ = None;

	//移動
	//イージングの移動開始座標
	Vector3 easigStartPos_ = {};
	//イージングの移動終了座標
	Vector3 easingEndPos = {};
	//イージングの移動タイム
	float easingMoveTime = 0;
	//移動ベクトル
	Vector3 addVector3;
	

	//扉解放
	bool isExitOpen = false;

	//コライダー
	SphereCollider* sphereCollider_ = nullptr;
	//レイキャスト
	Ray ray_;
	Vector3 rayCastDir_ = {0,0,-1};
};

