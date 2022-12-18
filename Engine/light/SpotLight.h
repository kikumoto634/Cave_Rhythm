#pragma once
#include <DirectXMath.h>

#include "../math/Vector/Vector2.h"
#include "../math/Vector/Vector3.h"
#include "../math/Vector/Vector4.h"

/// <summary>
/// �X�|�b�g���C�g
/// </summary>
class SpotLight
{
//�T�u�N���X
public:
	//�萔�o�b�t�@�p�f�[�^�\��
	struct ConstBufferData
	{
		DirectX::XMVECTOR lightv;
		Vector3 lightpos;
		float pad1;
		Vector3 lightcolor;
		float pad2;
		Vector3 lightatten;
		float pad3;
		Vector2 lightfactoranglecos;
		unsigned int active;
		float pad4;
	};

//�����o�֐�
public:

	//Getter
	inline const DirectX::XMVECTOR& GetLightDir()	{return lightdir;}
	inline const Vector3& GetLightPos()	{return lightpos;}
	inline const Vector3& GetLightColor()	{return lightcolor;}
	inline const Vector3& GetLightAtten()	{return lightAtten;}
	inline const Vector2& GetLightFactorAngleCos()	{return lightFactorAngleCos;}
	inline bool GetActive()	{return active;}

	//Setter
	inline void SetLightDir(const DirectX::XMVECTOR& lightdir)	{this->lightdir = DirectX::XMVector3Normalize(lightdir);}
	inline void SetLightPos(const Vector3& lightpos)	{this->lightpos = lightpos;}
	inline void SetLightColor(const Vector3& lightcolor)	{this->lightcolor = lightcolor;}
	inline void SetLightAtten(const Vector3& lightAtten)	{this->lightAtten = lightAtten;}
	inline void SetLightFactorAngleCos(const Vector2& lightfactoranglecos)	
	{
		this->lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightfactoranglecos.x));
		this->lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightfactoranglecos.y));
	}
	inline void SetActive(bool active)	{this->active = active;}

//�����o�ϐ�
private:
	//���C�g����
	DirectX::XMVECTOR lightdir = {1,0,0,0};
	//���C�g���W
	Vector3 lightpos = {0,0,0};
	//���C�g�F
	Vector3 lightcolor = {1,1,1};
	//���C�g���������W��
	Vector3 lightAtten = {1.0f, 1.0f, 1.0f};
	//���C�g�����p�x(�J�n�p�x�A�I���p�x)
	Vector2 lightFactorAngleCos = {0.5f, 0.2f};
	//�L���t���O
	bool active = false;
};