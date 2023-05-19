#include "PostEffect.h"
#include "Window.h"

PostEffect::PostEffect(UINT texnumber, Vector3 pos, Vector2 size, XMFLOAT4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY) 
	: Sprite(
		texnumber,
		pos,
		size,
		color,
		anchorpoint,
		isFlipX,
		isFlipY
	)
{
}

PostEffect *PostEffect::Create(UINT texNumber, Vector2 pos, Vector2 size, XMFLOAT4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	// Spriteのインスタンスを生成
	PostEffect* postEffect =
		new PostEffect(texNumber, {pos.x,pos.y,0.f}, size, color, anchorpoint, isFlipX, isFlipY);
	if (postEffect == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!postEffect->Initialize()) {
		delete postEffect;
		assert(0);
		return nullptr;
	}

	return postEffect;
}

bool PostEffect::Initialize()
{
	SpriteInitialize();
	TextureInitialize();
	SRVInitialize();
	RTVInitialize();
	DepthInitialize();
	DSVInitialize();

	return true;
}

void PostEffect::Draw()
{
	//ワールド行列の更新
	this->matWorld = XMMatrixIdentity();
	//Z軸回転
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(this->rotation));
	//平行移動
	this->matWorld *= XMMatrixTranslation(this->position.x, this->position.y, this->position.z);

	//定数バッファの転送
	HRESULT result;
	result = this->constBuffData->Map(0,nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->mat = this->matWorld * common->matProjection;
	constMap->color = this->color;
	this->constBuffData->Unmap(0, nullptr);


	//パイプラインステートの設定
	common->dxCommon->GetCommandList()->SetPipelineState(common->pipelinestate.Get());
	//ルートシグネチャの設定
	common->dxCommon->GetCommandList()->SetGraphicsRootSignature(common->rootsignature.Get());
	//プリミティブ形状を設定
	common->dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if(this->IsInvisible){
		return ;
	}

	//頂点バッファのセット
	common->dxCommon->GetCommandList()->IASetVertexBuffers(0,1,&this->vbView);
	//定数バッファをセット
	common->dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, this->constBuffData->GetGPUVirtualAddress());
	//テスクチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = {descHeapSRV.Get()};
	common->dxCommon->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps),ppHeaps);
	//シェーダーリソースビュー(SRV)をセット
	common->dxCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());
	//ポリゴンの描画
	common->dxCommon->GetCommandList()->DrawInstanced(4, 1, 0, 0);
}



void PostEffect::SpriteInitialize()
{
	HRESULT result;
	{
		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDesc =
		  CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * common->vertNum);

		//頂点バッファ生成
		result = common->dxCommon->GetDevice()->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&this->vertBuff)
		);
		assert(SUCCEEDED(result));
	}

	//頂点バッファへのデータ転送
	SpriteTransferVertexBuffer();

	//頂点バッファビューの作成
	this->vbView.BufferLocation = this->vertBuff->GetGPUVirtualAddress();
	this->vbView.SizeInBytes = sizeof(VertexPosUv) * common->vertNum;
	this->vbView.StrideInBytes = sizeof(VertexPosUv);

	{
		// ヒーププロパティ
		CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		// リソース設定
		CD3DX12_RESOURCE_DESC resourceDesc =
		  CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

		//定数バッファの生成
		result = common->dxCommon->GetDevice()->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&this->constBuffData)
		);
		assert(SUCCEEDED(result));
	}

	//定数バッファのデータ転送
	result = this->constBuffData->Map(0, nullptr, (void**)&constMap);
	if(SUCCEEDED(result)){
		constMap->color = this->color;
		constMap->mat = common->matProjection;
		this->constBuffData->Unmap(0, nullptr);
	}
}

void PostEffect::TextureInitialize()
{
	HRESULT result;
	//テクスチャリソース
	{
		CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			Window::GetWindowWidth(),
			(UINT)Window::GetWindowHeight(),
			1,0,1,0,
			D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
		);

		//テクスチャバッファ
		result = common->dxCommon->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
				D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			nullptr,
			IID_PPV_ARGS(&texbuff)
		);
		assert(SUCCEEDED(result));
	}

	//イメージデータ転送
	{
		///画素数
		const UINT pixelCount = Window::GetWindowWidth()*Window::GetWindowHeight();
		//画像1行のデータ
		const UINT rowPitch = sizeof(UINT) * Window::GetWindowWidth();
		//画像全体サイズ
		const UINT depthPitch = rowPitch * Window::GetWindowHeight();
		//イメージ
		UINT* img = new UINT[pixelCount];
		for(UINT i = 0; i < pixelCount; i++)	{img[i] = 0xff0000ff;}

		//転送
		result = texbuff->WriteToSubresource(
			0,
			nullptr,
			img,
			rowPitch,
			depthPitch
		);
		assert(SUCCEEDED(result));
		delete[] img;
	}
}

void PostEffect::SRVInitialize()
{
	HRESULT result;
	//SRVデスクリ
	{
		D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
		srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		srvDescHeapDesc.NumDescriptors = 1;
		//生成
		result = common->dxCommon->GetDevice()->CreateDescriptorHeap(
			&srvDescHeapDesc,
			IID_PPV_ARGS(&descHeapSRV)
		);
		assert(SUCCEEDED(result));

		//SRV作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		//デスクリプタヒープにSRV作成
		common->dxCommon->GetDevice()->CreateShaderResourceView(
			texbuff.Get(),
			&srvDesc,
			descHeapSRV->GetCPUDescriptorHandleForHeapStart()
		);
	}
}

void PostEffect::RTVInitialize()
{
	HRESULT result;
	//RTV
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
		rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescHeapDesc.NumDescriptors = 1;
		//生成
		result = common->dxCommon->GetDevice()->CreateDescriptorHeap(
			&rtvDescHeapDesc,
			IID_PPV_ARGS(&descHeapRTV)
		);
		assert(SUCCEEDED(result));

		//設定
		D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		//シェーダーの計算結果をSRGBに変換と書き込み
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		//作成
		common->dxCommon->GetDevice()->CreateRenderTargetView(
			texbuff.Get(),
			&renderTargetViewDesc,
			descHeapRTV->GetCPUDescriptorHandleForHeapStart()
		);
	}
}

void PostEffect::DepthInitialize()
{
	HRESULT result;
	//深度バッファ
	{
		//設定
		CD3DX12_RESOURCE_DESC depthResDesc = 
			CD3DX12_RESOURCE_DESC::Tex2D(
				DXGI_FORMAT_D32_FLOAT,
				Window::GetWindowWidth(),
				Window::GetWindowHeight(),
				1,0,
				1,0,
				D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
			);

		//生成
		result = common->dxCommon->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&depthResDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT,1.0f, 0),
			IID_PPV_ARGS(&depthBuff)
		);
		assert(SUCCEEDED(result));
	}
}

void PostEffect::DSVInitialize()
{
	HRESULT result;
	//DSV
	{
		//設定
		D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
		DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		DescHeapDesc.NumDescriptors = 1;

		//作成
		result = common->dxCommon->GetDevice()->CreateDescriptorHeap(
			&DescHeapDesc,
			IID_PPV_ARGS(&descHeapDSV)
		);
		assert(SUCCEEDED(result));

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		common->dxCommon->GetDevice()->CreateDepthStencilView(
			depthBuff.Get(),
			&dsvDesc,
			descHeapDSV->GetCPUDescriptorHandleForHeapStart()
		);
	}
}
