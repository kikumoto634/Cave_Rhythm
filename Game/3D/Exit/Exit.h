#pragma once
#include "../BaseObjObject.h"
#include "../../Collision/MeshCollider.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#include "../../2D/BaseSprites.h"

class Exit : public BaseObjObject
{
public:
	~Exit();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	void Initialize(std::string filePath, bool IsSmmothing = false) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw() override;

	void Draw2D();

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	void OnCollision(const CollisionInfo& info) override;

	//3D->2D変換
	Vector2 ChangeTransformation(Vector3 targetpos);

	//Getter
	inline bool GetCoinSpAlive()	{return IsPlayerContact;}

	//Setter
	inline void SetCoinSpPosition(Vector2 pos)	{coinSp->SetPosition(pos);}

private:
	//ベクトル*行列
	Vector3 Vector3Transform( Vector3 &v,  DirectX::XMMATRIX &m);

private:
	Window* window = nullptr;

	bool IsPlayerContact = false;

	std::unique_ptr<BaseSprites> coinSp;
	const int ExitNeedCoinsNum = 1;
	const int TexNumberBegin = 3;	//テクスチャ番号
	const int CoinSpNumSize = 3;	//サイズ
	std::unique_ptr<BaseSprites> coinSpNum[3];

};

