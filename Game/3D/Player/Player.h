#pragma once
#include "../BaseObjObject.h"
#include "../../../Engine/input/Input.h"
#include "../../Collision/SphereCollider.h"

#include "PlayerWeapon.h"

//ToDO プレイヤーのリファクタリング
/*
処理一覧
Initialize();
・スケールイージングの最小値設定
・Inputのinstance取得
・コライダーセット(同じ属性を持ったコライダー同士の当たり判定)
・属性セット
・球コライダーの取得(レイを飛ばしての当たり判定用)
・武器の取得(new)
・攻撃モデルの読み込み(new)

Update();
・カメラの受け取り

入力
・ボタンが入力されたかの確認
・移動(モデル変更) 座標、回転、過去座標、武器のオフセット座標の取得
・攻撃(モデル変更 + weapon->Attack()呼び出し)
・移動制限

更新
・ビート時処理 (サイズイージング)
・ダメージ(接触時の無敵時間計測)
・重力(必要性を感じない)
・武器位置の更新
・Baseの更新
・コライダー更新
・地面があるか無いかの取得(Ray)
・武器更新

描画
・武器描画
・プレイヤー描画

接触時
・敵(ダメージ)

後処理
・武器後処理、解放
・攻撃モデル解放
・Baseの解放
*/

class Player : public BaseObjObject
{
//定数
//無敵時間
const int DamageFrame = 120;

//メンバ関数
public:
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw();

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
	//void JudgeUpdate(bool IsFlag);

	/// <summary>
	/// ダメージサウンド発生
	/// </summary>
	bool DamageSound();

	//Getter
	inline int GetHP()	{return HP;}
	inline bool GetIsInputOnce()	{return IsInputOnce;}

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

	//重力
	//void GravityFall();

	//コライダー
	//地面接触判定
	//void GroundCollider();

private:
	//接地フラグ
	//bool IsGround = true;
	//落下ベクトル
	//Vector3 fallV;
	
	//移動
	Vector3 movePosition = {0,0,0};
	Vector3 moveRotation = {0,0,0};
	Vector3 oldPosition = {};

	//HP
	int HP = 5;

	//ダメージ
	bool IsDamage = false;
	bool IsDamageSoundOnce = false;
	////無敵時間(フレーム)
	int damageCurrentFrame = 0;

	//使用モデル判別(false:Move, true:Attack)
	bool IsModelJudge = false;

	//入力
	Input* input = nullptr;
	bool IsInputOnce = false;

	//コライダー
	SphereCollider* sphereCollider = nullptr;

	//武器
	PlayerWeapon* weapon = nullptr;
	Vector3 offSetWeaponPos = {0,0,2.5};

	//攻撃モデル
	ObjModelManager* attackModel = nullptr;
};

