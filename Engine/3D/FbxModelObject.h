#pragma once

#include <DirectXMath.h>
#include <string>

#include "../Engine/base/FbxModelManager.h"
#include "../Engine/loader/FbxLoader.h"
#include "../../camera/Camera.h"
#include "WorldTransform.h"
#include "../Engine/base/DirectXCommon.h"

class FbxModelObject{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static const int MAX_BONES = 32;

public:
	class CommonFbx{
		friend class FbxModelObject;

	public:
		void InitializeGraphicsPipeline();

	private:
		DirectXCommon* dxCommon = nullptr;

		ComPtr<ID3D12RootSignature> rootSignature;
		ComPtr<ID3D12PipelineState> pipelineState;
	};

	struct ConstBufferDataTransform{
		XMMATRIX viewproj;
		XMMATRIX world;
		Vector3 cameraPos;
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:
	static void StaticInitialize(DirectXCommon* dxCommon);
	static void StaticFinalize();

	static FbxModelObject* Create(FbxModelManager* model);

private:
	static CommonFbx* common;

public:
	FbxModelObject();
	FbxModelObject(FbxModelManager* model);

	bool Initialize();
	void Update(WorldTransform worldTransform, Camera* camera);
	void Draw();

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	void PlayAnimation();

protected:
	ComPtr<ID3D12Resource> constBufferTransform;
	//�萔�o�b�t�@(�X�L��)
	ComPtr<ID3D12Resource> constBufferSkin;

	ConstBufferDataTransform* constMap = nullptr;
	ConstBufferDataSkin* constSkinMap = nullptr;

	FbxModelManager* model = nullptr;

	//1frame�̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ���(�A�j���[�V����)
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;
};