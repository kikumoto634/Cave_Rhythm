#pragma once
#include "../BaseFbxObjects.h"
#include "../../../Engine/input/Input.h"

class SampleFbxObject : public BaseFbxObjects
{
//�����o�֐�
public:
	~SampleFbxObject();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(std::string filePath) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

};

