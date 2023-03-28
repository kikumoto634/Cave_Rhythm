#pragma once
#include "../BaseObjObject.h"

class SampleObjObject : public BaseObjObject
{
//メンバ関数
public:
	~SampleObjObject();

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
	void Draw() override;

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;
};

