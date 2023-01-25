#pragma once
#include "../BaseObjObject.h"

class Coins : public BaseObjObject
{
private:
	const int LostMaxBeat = 8;

public:
	~Coins();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision(const CollisionInfo& info) override;

	/// <summary>
	/// 出現
	/// </summary>
	void Pop(Vector3 pos);

	/// <summary>
	/// 出現可能
	/// </summary>
	bool PopPossible();
	
	/// <summary>
	/// コイン獲得
	/// </summary>
	bool GetCoin();

	//Getter
	bool GetIsAlive() {return IsAlive;}

private:
	//出現
	bool IsAlive = false;

	//獲得
	bool IsGet = false;

	//消失時の座標
	Vector3 DeadPos = {50,50,50};

	//ロスト時間(ビート数で処理)
	int lostBeat = 0;
};

