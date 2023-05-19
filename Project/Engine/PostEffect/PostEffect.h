#pragma once
#include "Sprite.h"
class PostEffect : public Sprite
{
public://エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
public:
	static PostEffect* Create(UINT texNumber, Vector2 pos, Vector2 size, XMFLOAT4 color = {1,1,1,1},
		Vector2 anchorpoint = {0.f,0.f}, bool isFlipX = false, bool isFlipY = false);

public:
	PostEffect(UINT texnumber, Vector3 pos, Vector2 size, XMFLOAT4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY);

	bool Initialize();

	void Draw();

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


	//

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
};

