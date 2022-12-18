#pragma once
#include "../BaseObjObject.h"

class Player : public BaseObjObject
{
//�����o�֐�
public:
	~Player();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// 3D�`��
	/// </summary>
	void Draw3D();

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

private:
	//�ڒn�t���O
	bool IsGround = true;
	//�����x�N�g��
	Vector3 fallV;
};

