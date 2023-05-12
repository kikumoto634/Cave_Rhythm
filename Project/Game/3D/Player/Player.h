#pragma once
#include "BaseObjObject.h"
#include "Input.h"
#include "SphereCollider.h"
#include "PlayerWeapon.h"

#include "AreaManager.h"

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

private:
	struct MoveInfo{
		Vector3 moveAdd;
		float Rot;
	};

//定数
private:
	//拡縮の最小サイズ
	const Vector3 SizeMin = {0.7f,0.7f,0.7f};

	//移動距離
	const float MoveLength = AreaManager::Block_Size;
	//レイキャストの長さ
	const float adsDistance = 1.0f;
	
	//移動
	MoveInfo LeftMove  = {{-MoveLength, 0.f ,0.f},		   DirectX::XMConvertToRadians(-90)};
	MoveInfo RightMove = {{+MoveLength, 0.f ,0.f},		   DirectX::XMConvertToRadians(90)};
	MoveInfo UpMove    = {{0.f        , 0.f ,+MoveLength}, DirectX::XMConvertToRadians(0)};
	MoveInfo DownMove  = {{0.f        , 0.f ,-MoveLength}, DirectX::XMConvertToRadians(180)};

	//ダメージ
	//カメラのシェイク強さ
	const int ShakeValue = 3;
	//ダメージ状態の総フレーム
	const int DamageFrameMax = 150;
	//色変化の間隔
	const int damageFrameInterval = 6;
	//通常色
	const Vector4 NormalColor = {1.0f,1.0f,1.0f,1.0f};
	//ダメージ色1
	const Vector4 Damage1Color = {0.0f,0.0f,0.0f,0.0f};
	//ダメージ色2
	const Vector4 Damage2Color = {1.0f,0.0f,0.0f,1.0f};
	

	//コライダー
	//オフセット
	XMVECTOR SphereColliderOffSet = {0.f,-0.2f,0.f,0.f};
	//半径
	const float SphereColliderRadius = 0.6f;

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


	void Pop(Vector3 pos)	{SetPosition(pos);}


	//入力可能状態
	inline void InputPossible()	{isDuplicateLimit_ = false;}

	//Getter
	//ダメージ
	bool GetIsDamage();
	//死亡
	bool GetIsDead();
	//破壊ブロック位置
	Vector2 GetBlockBreakPos()	{return blockBreakPos_;}
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

	//破壊ブロック位置
	Vector2 blockBreakPos_ = {-100,-100};
};

