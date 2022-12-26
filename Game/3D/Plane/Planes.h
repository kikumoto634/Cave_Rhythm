#pragma once
#include "../BaseObjObject.h"
#include "../../Collision/MeshCollider.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

class Planes : public BaseObjObject
{
public:
	~Planes();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	void Initialize(std::string filePath, bool IsSmmothing = false);

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


	//Getter
	inline bool GetIsPlayerContact()	{return IsPlayerContact;}

	//Setter
	inline void SetIsPlayerContact(bool IsFlag)	{IsPlayerContact = IsFlag;}

private:
	bool IsPlayerContact = false;
};

