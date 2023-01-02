#pragma once
#include "../Engine/base/ParticleManager.h"

class ParticleObject
{
//�����o�֐�
public:
	ParticleObject(){};
	ParticleObject(
		int life, 
		Vector3 position, 
		Vector3 velocity, 
		Vector3 accel, 
		float start_scale, 
		float end_scale, 
		UINT texNumber = 1,
		Vector4 color = {1,1,1,1}
	);

	~ParticleObject();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

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
	/// �p�[�e�B�N������
	/// </summary>
	void ParticleAppearance();

	/// <summary>
	/// �p�[�e�B�N���ύX
	/// </summary>
	void ParticleSet(int life, 
		Vector3 position, 
		Vector3 velocity, 
		Vector3 accel, 
		float start_scale, 
		float end_scale, 
		UINT texNumber,
		Vector4 color
	);

//�����o�ϐ�
public:
	//�p�[�e�B�N��
	ParticleManager* particle = nullptr;
	WorldTransform world = {};

	//�e����
	int life;
	Vector3 position;
	Vector3 velocity;
	Vector3 accel;
	float start_scale;
	float end_scale;
	UINT texNumber;
};

