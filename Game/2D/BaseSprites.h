#pragma once
#include "../Engine/2D/Sprite.h"
#include "../Engine/math/Vector/Vector2.h"

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

	//Setter
	void SetPosition(const Vector2& position)	{this->position = position;}
	void SetSize(const Vector2& size)	{this->size = size;}
	void SetColor(DirectX::XMFLOAT4 color)	{this->color = color;}

protected:
	Sprite* sprite;
	Vector2 position = {0,0};
	Vector2 size = {100,100};
	DirectX::XMFLOAT4 color = {1,1,1,1};
};