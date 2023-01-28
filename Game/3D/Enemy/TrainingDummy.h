#pragma once
#include "../BaseObjObject.h"
#include "../../3D/ParticleObject.h"
#include "../../2D/BaseSprites.h"

class TrainingDummy : public BaseObjObject
{
private:
	//死亡後のリセット時間
	const int AppearanceResetFrame = 180;

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
	void Update(Camera* camera) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	void Draw2D();

	/// <summary>
	/// パーティクル描画
	/// </summary>
	void ParticleDraw();

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	void OnCollision(const CollisionInfo& info) override;

	//3D->2D変換
	Vector2 ChangeTransformation(Vector3 targetpos);

	//Getter
	inline bool GetIsDeadAudioOnce()	{return IsDeadAudioOnce;}
	inline Vector3 GetDeadParticlePos()	{return DeadParticlePos;}

	//Setter
	inline void SetButtonSpPosition(Vector2 pos)	{buttonSp->SetPosition(pos);}

	//死亡エフェクト
	void DeadParticleApp();

private:
	//ベクトル*行列
	Vector3 Vector3Transform( Vector3 &v,  DirectX::XMMATRIX &m);

private:
	Window* window = nullptr;

	//接地フラグ
	bool IsGround = true;
	//スケール
	bool IsScale = false;

	bool IsDead = false;
	bool IsDeadAudioOnce = false;
	//死亡後のリセット時間
	int appearanceResetFrame = 0;
	//死亡時の待機位置
	Vector3 DeadPos = {50,50,50};
	//リスポーン地点
	Vector3 RespawnPos = {};


	//死亡地点(パーティクル用)
	Vector3 DeadParticlePos = {};
	bool IsDeadParticleOnce = false;

	//パーティクル
	ParticleObject* DeadParticle = nullptr;

	//ボタンスプライト
	std::unique_ptr<BaseSprites> buttonSp;
};

