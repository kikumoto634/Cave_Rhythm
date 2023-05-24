#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
public://エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	struct ConstBufferDate{
		bool isActive;
		Vector2 offset;
		float pad1;
		Vector3 color;
	};
	
public:
	static PostEffect* Create(UINT texNumber, Vector2 pos, Vector2 size, XMFLOAT4 color = {1,1,1,1},
		Vector2 anchorpoint = {0.f,0.f}, bool isFlipX = false, bool isFlipY = false);

public:
	PostEffect(UINT texnumber, Vector3 pos, Vector2 size, XMFLOAT4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY);

	bool Initialize();

	void Draw();

	//シーン描画前描画
	void PreDrawScene();
	//シーン描画後処理
	void PostDrawScene();

private:
	//スプライト共通初期化
	void SpriteInitialize();

	//テクスチャ初期化
	void TextureInitialize();
	//SRV初期化
	void SRVInitialize();
	//RTV初期化
	void RTVInitialize();
	//深度バッファ初期化
	void DepthInitialize();
	//DSV初期化
	void DSVInitialize();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipelineState();


private:
	//画面クリアカラー
	static const float clearColor[4];

	static const int VertNum = 4;

private:
	//テクスチャ
	ComPtr<ID3D12Resource> texbuff;
	//SRVデスクリ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	//グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	bool isActive = true;
	Vector2 offset = {0.0f, 0.0f};
	Vector3 color = {1,1,1};

	float frame = 0;
};

