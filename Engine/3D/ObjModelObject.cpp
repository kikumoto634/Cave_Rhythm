#include "ObjModelObject.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

ObjModelObject::CommonObj* ObjModelObject::common = nullptr;


void ObjModelObject::StaticInitialize( DirectXCommon* dxCommon)
{
	// nullptr�`�F�b�N
	assert(dxCommon);
	common = new CommonObj();
	common->dxCommon = dxCommon;
	
	common->InitializeGraphicsPipeline();
}

void ObjModelObject::StaticFinalize()
{
	if(common != nullptr){
		delete common;
		common = nullptr;
	}
}

ObjModelObject * ObjModelObject::Create(ObjModelManager* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	ObjModelObject* objModelObject = new ObjModelObject(model);
	if (objModelObject == nullptr) {
		return nullptr;
	}

	// ������
	if (!objModelObject->Initialize()) {
		delete objModelObject;
		assert(0);
		return nullptr;
	}

	return objModelObject;
}

ObjModelObject::ObjModelObject(ObjModelManager *model)
{
	this->model = model;
}

bool ObjModelObject::Initialize()
{
	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDescB0 =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = common->dxCommon->GetDevice()->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDescB0, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffB0));
	assert(SUCCEEDED(result));


	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDescB1 =
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff);

	// �萔�o�b�t�@�̐���
	result = common->dxCommon->GetDevice()->CreateCommittedResource(
		&heapProps, // �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE, &resourceDescB1, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffB1));
	assert(SUCCEEDED(result));

	return true;
}

void ObjModelObject::Update(WorldTransform world, Camera* camera)
{
	HRESULT result {};

	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	const Vector3& cameraPos = camera->GetEye();

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0* constMapB0 = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMapB0);
	assert(SUCCEEDED(result));
	constMapB0->viewproj = matViewProjection;
	constMapB0->world = world.matWorld;
	constMapB0->cameraPos = cameraPos;
	constMapB0->color = color;
	constMapB0->lightLayer = lightLayer;
	constBuffB0->Unmap(0, nullptr);

	ConstBufferDataB1* constMapB1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMapB1);
	assert(SUCCEEDED(result));
	constMapB1->ambient = model->GetMaterial().ambient;
	constMapB1->diffuse = model->GetMaterial().diffuse;
	constMapB1->specular = model->GetMaterial().specular;
	constMapB1->alpha = model->GetMaterial().alpha;
	constBuffB1->Unmap(0, nullptr);
}

void ObjModelObject::Draw()
{
	// �p�C�v���C���X�e�[�g�̐ݒ�
	common->dxCommon->GetCommandList()->SetPipelineState(common->pipelinestate.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	common->dxCommon->GetCommandList()->SetGraphicsRootSignature(common->rootsignature.Get());
	// �v���~�e�B�u�`���ݒ�
	common->dxCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �萔�o�b�t�@�r���[���Z�b�g
	common->dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	common->dxCommon->GetCommandList()->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	//���C�g
	if(common->light != nullptr)common->light->Draw(common->dxCommon->GetCommandList(), 3);
	//���f��
	model->Draw(common->dxCommon->GetCommandList());
}

void ObjModelObject::CommonObj::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ID3DBlob* vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shader/ObjVS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shader/ObjPS.hlsl",	// �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0",	// �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W(1�s�ŏ������ق������₷��)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��(1�s�ŏ������ق������₷��)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W(1�s�ŏ������ق������₷��)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob);
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob);

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 ���W�X�^

	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[4] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* rootSigBlob;
	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ���[�g�V�O�l�`���̐���
	result = dxCommon->GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = dxCommon->GetDevice()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));

}
