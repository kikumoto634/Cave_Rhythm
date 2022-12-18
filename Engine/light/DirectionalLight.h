#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <d3d12.h>
#include <d3dx12.h>

#include "../math//Vector/Vector2.h"
#include "../math//Vector/Vector3.h"
#include "../math//Vector/Vector4.h"
#include "../base/DirectXCommon.h"

class DirectionalLight
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//�T�u�N���X
public:
	struct ConstBufferData
	{
		DirectX::XMVECTOR lightv;	//���C�g����
		Vector3 lightcolor;			//���C�g�F
		unsigned int active;
	};


//�����o�֐�
public:

	//Getter
	inline const DirectX::XMVECTOR GetLightDir()	{return DirectX::XMVector3Normalize(lightdir);}
	inline const Vector3& GetLightColor()	{return lightcolor;}
	inline const bool& GetIsActive()	{return IsActive;}

	//Setter
	inline void SetLightDir(const DirectX::XMVECTOR& lightdir)	{this->lightdir = lightdir;}
	inline void SetLightColor(const Vector3 lightcolor)	{this->lightcolor = lightcolor;}
	inline void SetIsActive(const bool IsActive)	{this->IsActive = IsActive;}

//�ÓI�����o�ϐ�
private:
	static DirectXCommon* dxCommon;

//�����o�ϐ�
private:
	//���C�g��������
	DirectX::XMVECTOR lightdir = {1,0,0,0};
	//���C�g�F
	Vector3 lightcolor = {1,1,1};
	//�_�[�e�B�t���O
	bool IsActive = false;
};

