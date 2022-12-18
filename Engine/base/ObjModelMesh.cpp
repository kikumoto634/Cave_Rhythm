#include "ObjModelMesh.h"

#include <d3dcompiler.h>
#include <cassert>
#include <DirectXMath.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
DirectXCommon* ObjModelMesh::dxCommon = nullptr;

void ObjModelMesh::StaticInitialize(DirectXCommon *dxCommon)
{
	assert(!ObjModelMesh::dxCommon);

	ObjModelMesh::dxCommon = dxCommon;
}

void ObjModelMesh::CreateBuffers()
{
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv)*vertices.size());

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// ���_�o�b�t�@����
	HRESULT result = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)*indices.size());
	// ���\�[�X�ݒ�
	resourceDesc.Width = sizeIB;

	// �C���f�b�N�X�o�b�t�@����
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&indexBuff));

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void ObjModelMesh::Draw(ID3D12GraphicsCommandList *commandList)
{
	// ���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&ibView);

	// �`��R�}���h
	commandList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

void ObjModelMesh::AddVertex(const VertexPosNormalUv &vertex)
{
	vertices.emplace_back(vertex);
}

void ObjModelMesh::AddIndex(unsigned short index)
{
	indices.emplace_back(index);
}

void ObjModelMesh::AddSmmpthData(unsigned short indexPosition, unsigned short indexVertex)
{
	smoothData[indexPosition].emplace_back(indexVertex);
}

void ObjModelMesh::CalculateSmoothedVertexNormals()
{
	auto it = smoothData.begin();
	for(; it != smoothData.end(); ++it){
		//�e�ʗp�̋��ʒ��_�R���N�V����
		std::vector<unsigned short>& v = it->second;
		//�S���_�̖@���𕽋ω�����
		XMVECTOR normal = {};
		for(unsigned short index : v){
			normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z,0);
		}
		normal = XMVector3Normalize(normal/(float)v.size());
		//���ʖ@�����g�p���邷�ׂĂ̒��_�f�[�^�ɏ�������
		for(unsigned short index : v){
			vertices[index].normal = {normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2]};
		}
	}
}
