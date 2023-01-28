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

	/// <summary>
	/// 出口解放
	/// </summary>
	void ExitOpen()	{IsOpen = true;}

	/// <summary>
	/// 出口施錠
	/// </summary>
	void ExitClose()	{IsOpen = false;}

	//モデル変更
	void ModelChange()	{this->object->SetModel(stairsModel);}

	//Getter
	inline bool GetIsPlayerContact() {return IsPlayerContact;}
	inline const int GetExitNeedCoinNum() {return ExitNeedCoinsNum;}

	//Setter
	inline void SetCoinSpPosition(Vector2 pos)	{coinSp->SetPosition(pos);}
	inline void SetExitOpenNeedCoin(int num)	{ExitNeedCoinsNum = num;}

private:
	//ベクトル*行列
	Vector3 Vector3Transform( Vector3 &v,  DirectX::XMMATRIX &m);

private:
	Window* window = nullptr;

	//プレイヤー接触
	bool IsPlayerContact = false;

	//必要コイン数
	std::unique_ptr<BaseSprites> coinSp;
	int ExitNeedCoinsNum = 1;
	const int TexNumberBegin = 3;	//テクスチャ番号
	const int CoinSpNumSize = 3;	//サイズ
	std::unique_ptr<BaseSprites> coinSpNum[3];

	//開閉
	bool IsOpen = false;
	std::unique_ptr<BaseSprites> exitOpenSp;

	//階段モデル
	ObjModelManager* stairsModel = nullptr;
	bool IsModelChange = false;
};

