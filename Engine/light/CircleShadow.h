#pragma once
#include <DirectXMath.h>

#include "../math/Vector/Vector2.h"
#include "../math/Vector/Vector3.h"
#include "../math/Vector/Vector4.h"

/// <summary>
/// �X�|�b�g���C�g
/// </summary>
class CircleShadow
{
//�T�u�N���X
public:
	//�萔�o�b�t�@�p�f�[�^�\��
	struct ConstBufferData
	{
		DirectX::XMVECTOR dir;
		Vector3 casterPos;
		float distanceCasterLight;
		Vector3 atten;
		float pad3;
		Vector2 factorAngleCos;
		unsigned int active;
		float pad4;
	};

//�����o�֐�
public:

	//Getter
	inline const DirectX::XMVECTOR& GetDir()	{return dir;}
	inline const float& GetDistanceCasterLight()	{return distanceCasterLight;}
	inline const Vector3& GetCasterPos()	{return casterPos;}
	inline const Vector3& GetAtten()	{return atten;}
	inline const Vector2& GetFactorAngleCos()	{return factorAngleCos;}
	inline bool GetActive()	{return active;}

	//Setter
	inline void SetDir(const DirectX::XMVECTOR& dir)	{this->dir = DirectX::XMVector3Normalize(dir);;}
	inline void SetDistanceCasterLight(const float& distanceCasterLight)	{this->distanceCasterLight = distanceCasterLight;}
	inline void SetCasterPos(const Vector3& casterPos)	{this->casterPos = casterPos;}
	inline void SetAtten(const Vector3& atten)	{this->atten = atten;}
	inline void SetFactorAngleCos(const Vector2& factorAngleCos)	
	{
		this->factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngleCos.x));
		this->factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngleCos.y));
	}
	inline void SetActive(bool active)	{this->active = active;}

//�����o�ϐ�
private:
	//����
	DirectX::XMVECTOR dir = {1,0,0,0};
	//�L���X�^�[�ƃ��C�g����
	float distanceCasterLight = 100.0f;
	//�L���X�^�[���W
	Vector3 casterPos = {0,0,0};
	//���������W��
	Vector3 atten = {0.5f, 0.6f, 0.0f};
	//�����p�x(�J�n�p�x�A�I���p�x)
	Vector2 factorAngleCos = {0.2f, 0.5f};
	//�L���t���O
	bool active = false;
};