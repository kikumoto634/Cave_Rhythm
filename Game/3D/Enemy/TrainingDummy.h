#pragma once
#include "../BaseObjObject.h"
#include "../../3D/ParticleObject.h"
#include "../../2D/BaseSprites.h"

class TrainingDummy : public BaseObjObject
{
private:
	//���S��̃��Z�b�g����
	const int AppearanceResetFrame = 180;

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
	void Update(Camera* camera) override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	void Draw2D();

	/// <summary>
	/// �p�[�e�B�N���`��
	/// </summary>
	void ParticleDraw();

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �����蔻��
	/// </summary>
	void OnCollision(const CollisionInfo& info) override;

	//3D->2D�ϊ�
	Vector2 ChangeTransformation(Vector3 targetpos);

	//Getter
	inline bool GetIsDeadAudioOnce()	{return IsDeadAudioOnce;}
	inline Vector3 GetDeadParticlePos()	{return DeadParticlePos;}

	//Setter
	inline void SetButtonSpPosition(Vector2 pos)	{buttonSp->SetPosition(pos);}

	//���S�G�t�F�N�g
	void DeadParticleApp();

private:
	//�x�N�g��*�s��
	Vector3 Vector3Transform( Vector3 &v,  DirectX::XMMATRIX &m);

private:
	Window* window = nullptr;

	//�ڒn�t���O
	bool IsGround = true;
	//�X�P�[��
	bool IsScale = false;

	bool IsDead = false;
	bool IsDeadAudioOnce = false;
	//���S��̃��Z�b�g����
	int appearanceResetFrame = 0;
	//���S���̑ҋ@�ʒu
	Vector3 DeadPos = {50,50,50};
	//���X�|�[���n�_
	Vector3 RespawnPos = {};


	//���S�n�_(�p�[�e�B�N���p)
	Vector3 DeadParticlePos = {};
	bool IsDeadParticleOnce = false;

	//�p�[�e�B�N��
	ParticleObject* DeadParticle = nullptr;

	//�{�^���X�v���C�g
	std::unique_ptr<BaseSprites> buttonSp;
};

