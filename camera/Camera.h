#pragma once
#include "../Engine/base/Window.h"
#include "../Engine/math/Vector/Vector3.h"

#include "../Engine/3D/ViewProjection.h"

#include <DirectXMath.h>

/// <summary>
/// �J����
/// </summary>
class Camera
{
protected:
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static Camera* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Window* window);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �����_�ړ�
	/// </summary>
	/// <param name="move">�ړ���(radian��ݒ�)</param>
	void MoveEyeVeector(Vector3 move);

	/// <summary>
	/// �J�����ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveVector(Vector3 move);

	/// <summary>
	/// ��]
	/// </summary>
	/// <param name="rot">��]��</param>
	void RotVector(Vector3 rot);

	void ShakeStart(int MaxFrame = 5);

	//getter
	const XMMATRIX& GetMatProjection()	{return view.matProjection;}
	const XMMATRIX& GetMatView()	{return view.matView;}
	const XMMATRIX& GetViewProjectionMatrix()	{return view.matViewProjection;}


	const Vector3& GetEye() {return view.eye; }
	const Vector3& GetTarget() {return view.target; }
	const Vector3& GetUp() {return view.up; }
	const float& Distance() {return distance; }
	const DirectX::XMMATRIX& GetBillboard() {return view.matBillboard;}

	//setter
	void SetEye(const Vector3& eye)	{this->view.eye = eye; }
	void SetTarget(const Vector3& target)	{this->view.target = target; }
	void SetUp(const Vector3& up)	{this->view.up = up; }
	void SetDistance(const float& distance)	{this->distance = distance; }

private:
	void Shake();

private:
	//�A�X�y�N�g�p
	Window* window;

protected:
	ViewProjection view;
	float distance = 20.f;	//�J�����̋���

	//��]�s��
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	//�V�F�C�N
	bool IsShake = false;
	int shakePower = 1;
	int ShakeFrame = 5;
	int frame = 0;
	Vector3 saveTarget{};
	Vector3 saveEye{};
};

