#pragma once
#include "../BaseObjObject.h"

class TrainingDummy : public BaseObjObject
{
private:
	//���S��̃��Z�b�g����
	const int AppearanceResetFrame = 60;

//�����o�ϐ�
public:
	~TrainingDummy();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize();

	/// <summary>
	/// �����蔻��
	/// </summary>
	void OnCollision(const CollisionInfo& info) override;

	//Getter
	bool GetIsDeadAudioOnce()	{return IsDeadAudioOnce;}

private:
	//�ڒn�t���O
	bool IsGround = true;
	//�X�P�[��
	bool IsScale = false;

	bool IsDead = false;
	bool IsDeadAudioOnce = false;
	//���S��̃��Z�b�g����
	int appearanceResetFrame = 0;
};

