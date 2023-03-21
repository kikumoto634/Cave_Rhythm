#pragma once
#include "../Engine/2D/Sprite.h"
#include "../Engine/math/Vector/Vector2.h"
#include "../../camera/Camera.h"

class BaseSprites
{
public:
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize(UINT textureNumber);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// �㏈��
	/// </summary>
	virtual void Finalize();

	//Getter
	const Vector2& GetPosition()	{return position;}
	const Vector2& GetSize()	{return size;}
	const Vector3& GetColor()	{return {color.x,color.y,color.z};}

	//Setter
	void SetPosition(const Vector2& position)	{this->position = position;}
	void SetSize(const Vector2& size)	{this->size = size;}
	void SetColor(DirectX::XMFLOAT4 color)	{this->color = color;}
	void SetTexNumber(UINT texNumber)	{sprite->SetTexNumber(texNumber);}
	void SetAnchorPoint(Vector2 anchorPoint)	{sprite->SetAnchorpoint(anchorPoint);}

public:
	/// <summary>
	/// ���Y���ɂ��킹�ăT�C�Y�ύX 
	/// </summary>
	/// <param name="sizeMax">�ő�T�C�Y</param>
	/// <param name="sizeMin">�ŏ��T�C�Y</param>
	/// <param name="EndTime">�I������ (x:�b)</param>
	/// <returns></returns>
	bool ScaleChange(Vector2 sizeMax = {1.f,1.f}, Vector2 sizeMin = {0.7f,0.7f}, float EndTime = 0.25f);

	//3D->2D�ϊ�
	Vector2 ChangeTransformation(Vector3 target, Camera* camera);

private:
	//�x�N�g��*�s��
	Vector3 Vector3Transform( Vector3 &v,  DirectX::XMMATRIX &m);

private:
	Window* window = nullptr;

protected:
	Sprite* sprite;
	Vector2 position = {0,0};
	Vector2 size = {100,100};
	DirectX::XMFLOAT4 color = {1,1,1,1};

	Vector3 targetPos = {};

	//���Y���X�V�p
	//���I���t���O
	bool IsBeatEnd = false;

	//�T�C�Y�ύX�p
	//�T�C�Y
	Vector2 baseSize = {1,1};
	Vector2 scale = {1,1};
	//���ݎ���
	float scaleCurrentTime = 0.f;
};