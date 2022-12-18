#include "ViewProjection.h"

using namespace DirectX;

XMMATRIX ViewProjection::matBillboard  = DirectX::XMMatrixIdentity();
XMMATRIX ViewProjection::matBillboardY  = DirectX::XMMatrixIdentity();

void ViewProjection::UpdateViewMatrix()
{
	XMFLOAT3 leye = {eye.x,eye.y,eye.z};
	XMFLOAT3 ltarget = {target.x,target.y,target.z};
	XMFLOAT3 lup = {up.x,up.y,up.z};

	//���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&leye);
	XMVECTOR targetPosition = XMLoadFloat3(&ltarget);
	XMVECTOR upVector = XMLoadFloat3(&lup);

	//�J����Z��
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);
	//0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));
	//�x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);
	//�J������X��
	XMVECTOR cameraAxisX;
	//X���͏����->Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//�x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);
	//�J������Y��
	XMVECTOR cameraAxisY;
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	//�J�����s��
	XMMATRIX matCameraRot;
	//�J�������W�n->���[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0,0,0,1);

	//�r���[�ϊ��s��
	matView = XMMatrixTranspose(matCameraRot);

	//���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��(�J�������W�n)
	XMVECTOR tX = XMVector3Dot(cameraAxisX, reverseEyePosition);//X����
	XMVECTOR tY = XMVector3Dot(cameraAxisY, reverseEyePosition);//Y����
	XMVECTOR tZ = XMVector3Dot(cameraAxisZ, reverseEyePosition);//Z����
	//��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//�r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;

#pragma region �r���{�[�h�v�Z
	//�J����X���AY���AZ��
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;
	//X���͋���
	ybillCameraAxisX = cameraAxisX;
	//Y���̓��[���h���W�n��Y��
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z����X��->Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = XMVector3Cross(cameraAxisX, cameraAxisY);

	//�r���{�[�h�s��v�Z
	matBillboard.r[0] = ybillCameraAxisX;
	matBillboard.r[1] = ybillCameraAxisY;
	matBillboard.r[2] = ybillCameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0,0,0,1);
#pragma endregion �r���{�[�h�v�Z
}

void ViewProjection::UpdateProjectionMatrix(int width, int height)
{
	//�������e
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),	//�㉺��p45��
		(float)width / height,			//aspect��(��ʉ���/��ʏc��)
		0.1f, 1000.0f				//�O�[�A���[
	);
}
