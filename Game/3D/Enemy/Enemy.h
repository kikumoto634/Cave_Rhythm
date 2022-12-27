#pragma once
#include "../BaseObjObject.h"

class Enemy : public BaseObjObject
{
//メンバ変数
public:
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision(const CollisionInfo& info) override;

	//Getter

	//Setter
	void SetDirection(Vector3 _dir)	{direction = _dir;}

private:
	//接地フラグ
	bool IsGround = true;
	//落下ベクトル
	Vector3 fallV;
	//死亡
	bool IsDead = false;
	//スケール
	bool IsScale = false;

	//移動方向
	Vector3 direction = {-1,0,0};

	//死亡時、敵保存地点
	Vector3 DeadPos = {50,50,50};
};