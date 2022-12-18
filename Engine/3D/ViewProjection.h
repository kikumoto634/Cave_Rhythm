#pragma once
#include "../Engine/math/Vector/Vector3.h"

#include <DirectXMath.h>

class ViewProjection
{
public:
	using XMMATRIX = DirectX::XMMATRIX;

public:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(int width, int height);

public:
	//�r���{�[�h�s��
	static XMMATRIX matBillboard;
	//Y������r���{�[�h�s��
	static XMMATRIX matBillboardY;

	//�J�������
	//�������e
	XMMATRIX matProjection;	//�v���W�F�N�V�����s��
	//�r���[�ϊ��s��
	XMMATRIX matView;		//�r���[�s��
	Vector3 eye;			//���_���W
	Vector3 target;		//�����_���W
	Vector3 up;			//������x�N�g��

	XMMATRIX matViewProjection;
};

