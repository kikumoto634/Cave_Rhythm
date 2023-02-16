#pragma once

#include <wrl.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "../math/Vector/Vector2.h"
#include "../math/Vector/Vector3.h"
#include "../math/Vector/Vector4.h"
#include "DirectXCommon.h"
#include "ObjModelMesh.h"

class ObjModelManager{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public:
	//�}�e���A��
	struct Material
	{
		std::string name;//�}�e���A����
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;
		float alpha;
		std::string textureFilename;	//�e�N�X�`���t�@�C��
		//�R���X�g���N�^
		Material(){
			ambient = {1.0f, 1.0f, 1.0f};
			diffuse = {0.0f, 0.0f, 0.0f};
			specular = {0.0f, 0.0f, 0.0f};
			alpha = 1.0f;
		}
	};

public:
	static void StaticInitialize(DirectXCommon* dxCommon_);

public:
	~ObjModelManager();

	void CreateModel(std::string filePath, bool smmothing = false);

	void Draw(ID3D12GraphicsCommandList* commandList);

	//Getter
	
	/// <summary>
	/// �}�e���A���擾
	/// </summary>
	/// <returns></returns>
	Material GetMaterial()	{return material;}

	/// <summary>
	/// ���b�V���R���e�i�擾
	/// </summary>
	/// <returns>�}�b�V���R���e�i</returns>
	inline const std::vector<ObjModelMesh*>& GetMeshes() {return meshs;}

private:

	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	void LoadModel(const std::string& filePath, bool smmothing);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

private:

private:
	// �f�o�C�X
	static DirectXCommon* dxCommon;

private:

	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;

	//���b�V��
	std::vector<ObjModelMesh*> meshs;

	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;

	//�}�e���A��
	Material material;
};