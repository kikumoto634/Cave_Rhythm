#include "ObjModelManager.h"
#include <cassert>
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>

#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace DirectX;

DirectXCommon* ObjModelManager::dxCommon = nullptr;

void ObjModelManager::StaticInitialize(DirectXCommon* dxCommon_)
{
	dxCommon = dxCommon_;

	ObjModelMesh::StaticInitialize(dxCommon);
}

ObjModelManager::~ObjModelManager()
{
	for(auto& m : meshs){
		delete m;
	}
	meshs.clear();
}

void ObjModelManager::CreateModel(std::string filePath, bool smmothing)
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = dxCommon->GetDevice()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//����
	assert(SUCCEEDED(result));


	LoadModel(filePath, smmothing);

	for(auto& m : meshs){
		m->CreateBuffers();
	}
}

void ObjModelManager::Draw(ID3D12GraphicsCommandList* commandList)
{
	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	 commandList->SetGraphicsRootDescriptorTable(2, descHeap->GetGPUDescriptorHandleForHeapStart());
	
	 for(auto& m : meshs){
		m->Draw(commandList);
	 }
}

void ObjModelManager::LoadModel(const std::string &filePath, bool smmothing)
{
	//�t�@�C���X�g���[��
	ifstream file;
	int indexCountTex = 0;

	//.obj�t�@�C�����J��
	const string modelname = filePath;
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s
	assert(file.fill());

	// ���b�V������
	meshs.emplace_back(new ObjModelMesh);
	ObjModelMesh* mesh = meshs.back();

	vector<Vector3> positions;
	vector<Vector3> normals;
	vector<Vector2> texcoords;
	string line;
	while(getline(file,line)){
		istringstream line_stream(line);
		string key;
		getline(line_stream, key, ' ');

		if(key == "g")
		{
			if(modelname.size() > 0)
			{
				mesh->CalculateSmoothedVertexNormals();
				indexCountTex = 0;
			}
			// �O���[�v���ǂݍ���
			string groupName;
			line_stream >> groupName;
		}
		if(key == "v"){
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			positions.emplace_back(position);
		}
		if(key == "f"){
			int faceIndexCount = 0;
			string index_string;
			while(getline(line_stream, index_string, ' ')){
				istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1,ios_base::cur);


				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				ObjModelMesh::VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				mesh->AddVertex(vertex);

				//�G�b�W�������p�̃f�[�^�ǉ�
				if(smmothing)
				{
					//v�L�[(���W�f�[�^)�̔ԍ��ƁA���ׂč����������_�̃C���f�b�N�X���Z�b�g�œo�^����
					mesh->AddSmmpthData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
				}

				// �C���f�b�N�X�f�[�^�̒ǉ�
				if (faceIndexCount >= 3) {
					// �l�p�`�|���S����4�_�ڂȂ̂ŁA
					// �l�p�`��0,1,2,3�̓� 2,3,0�ŎO�p�`���\�z����
					mesh->AddIndex(indexCountTex - 1);
					mesh->AddIndex(indexCountTex);
					mesh->AddIndex(indexCountTex - 3);
				} else {
					mesh->AddIndex(indexCountTex);
				}
				indexCountTex++;
				faceIndexCount++;
			}
		}
		if(key == "vt"){
			Vector2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.emplace_back(texcoord);
		}
		if(key == "vn"){
			Vector3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			normals.emplace_back(normal);
		}
		if(key == "mtllib"){
			string filename;
			line_stream >> filename;
			LoadMaterial(directoryPath, filename);
		}
	}

	file.close();

	//���_�@���̕��ςɂ��G�b�W�̕�����
	if(smmothing){
		mesh->CalculateSmoothedVertexNormals();
	}
}

void ObjModelManager::LoadTexture(const std::string &directoryPath, const std::string &filename)
{
	HRESULT result = S_FALSE;

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// �t�@�C���p�X������
	 string filepath = directoryPath + filename;

	 // ���j�R�[�h������ɕϊ�����
	 wchar_t wfilepath[128];
	 int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile( wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
	assert(SUCCEEDED(result));

	ScratchImage mipChain{};
	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format, metadata.width, (UINT)metadata.height, (UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	// �q�[�v�v���p�e�B
	CD3DX12_HEAP_PROPERTIES heapProps =
		CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = dxCommon->GetDevice()->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr, IID_PPV_ARGS(&texbuff));
	assert(SUCCEEDED(result));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		const Image* img = scratchImg.GetImage(i, 0, 0); // ���f�[�^���o
		result = texbuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // �S�̈�փR�s�[
			img->pixels,          // ���f�[�^�A�h���X
			(UINT)img->rowPitch,  // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dxCommon->GetDevice()->CreateShaderResourceView(texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		descHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void ObjModelManager::LoadMaterial(const std::string &directoryPath, const std::string &filename)
{
	ifstream file;
	file.open(directoryPath + filename);
	if(file.fail()){
		assert(0);
	}

	string line;
	while(getline(file, line)){
		istringstream line_stream(line);

		string key;
		getline(line_stream,key,' ');

		if(key[0] == '\t'){
			key.erase(key.begin());
		}

		// �擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl") {
			// �}�e���A�����ǂݍ���
			line_stream >> material.name;
		}
		// �擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka") {
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		// �擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd") {
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		// �擪������Ks�Ȃ�X�y�L�����[�F
		if (key == "Ks") {
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}
		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd") {
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material.textureFilename;
			// �e�N�X�`���ǂݍ���
			LoadTexture(directoryPath, material.textureFilename);
		}
	}
	file.close();
}
