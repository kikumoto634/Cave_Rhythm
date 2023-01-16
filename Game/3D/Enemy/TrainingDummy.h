#pragma once
#include "../BaseObjObject.h"

class TrainingDummy : public BaseObjObject
{
private:
	//死亡後のリセット時間
	const int AppearanceResetFrame = 60;

//メンバ変数
public:
	~TrainingDummy();

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
	bool GetIsDeadAudioOnce()	{return IsDeadAudioOnce;}

private:
	//接地フラグ
	bool IsGround = true;
	//スケール
	bool IsScale = false;

	bool IsDead = false;
	bool IsDeadAudioOnce = false;
	//死亡後のリセット時間
	int appearanceResetFrame = 0;
};

