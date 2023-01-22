#include "Camera.h"

using namespace DirectX;

Camera *Camera::GetInstance()
{
	static Camera instance;
	return &instance;
}

void Camera::Initialize(Window* window)
{
	this->window = window;

	//�J����
	view.eye = {0, 0, -distance};
	view.target = {0, 0, 0};
	view.up = {0, 1, 0};


	view.UpdateViewMatrix();
	view.UpdateProjectionMatrix(window->GetWindowWidth(),window->GetWindowHeight());
}

void Camera::Update()
{
	view.matViewProjection = view.matView * view.matProjection;

	view.UpdateViewMatrix();
	view.UpdateProjectionMatrix(window->GetWindowWidth(),window->GetWindowHeight());
}

void Camera::MoveEyeVeector(Vector3 move)
{
	Vector3 eye_= GetEye();
	eye_ += move;
	SetEye(eye_);
}

void Camera::MoveVector(Vector3 move)
{
	Vector3 eye_ = GetEye();
	Vector3 target_ = GetTarget();
	eye_ += move;
	target_ += move;
	SetEye(eye_);
	SetTarget(target_);
}

void Camera::RotVector(Vector3 rot)
{
	// �ǉ���]���̉�]�s��𐶐�
	XMMATRIX matRotNew = XMMatrixIdentity();
	matRotNew *= XMMatrixRotationZ(-rot.z);
	matRotNew *= XMMatrixRotationX(-rot.x);
	matRotNew *= XMMatrixRotationY(-rot.y);
	// �ݐς̉�]�s�������
	// ����]�s���ݐς��Ă����ƁA�덷�ŃX�P�[�����O��������댯�������
	// �N�H�[�^�j�I�����g�p��������]�܂���
	matRot = matRotNew * matRot;

	// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
	XMVECTOR vTargetEye = {0.0f, 0.0f, -distance, 1.0f};
	XMVECTOR vUp = {0.0f, 1.0f, 0.0f, 0.0f};

	// �x�N�g������]
	vTargetEye = XMVector3Transform(vTargetEye, matRot);
	vUp = XMVector3Transform(vUp, matRot);

	// �����_���炸�炵���ʒu�Ɏ��_���W������
	const Vector3& target = GetTarget();
	SetEye(
		{target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1],
		target.z + vTargetEye.m128_f32[2]});
	SetUp({vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2]});
}


