#pragma once
#include "../BaseObjObject.h"
#include "../../Collision/MeshCollider.h"
#include "../../Collision/CollisionSystem/CollisionAttribute.h"

#include "../../2D/BaseSprites.h"

class Exit : public BaseObjObject
{
public:
	~Exit();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	void Initialize(std::string filePath, bool IsSmmothing = false) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// 3D�`��
	/// </summary>
	void Draw() override;

	void Draw2D();

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	/// <param name="info">�Փˏ��</param>
	void OnCollision(const CollisionInfo& info) override;

	//3D->2D�ϊ�
	Vector2 ChangeTransformation(Vector3 targetpos);

	//Getter
	inline bool GetCoinSpAlive()	{return IsPlayerContact;}

	//Setter
	inline void SetCoinSpPosition(Vector2 pos)	{coinSp->SetPosition(pos);}

private:
	//�x�N�g��*�s��
	Vector3 Vector3Transform( Vector3 &v,  DirectX::XMMATRIX &m);

private:
	Window* window = nullptr;

	bool IsPlayerContact = false;

	std::unique_ptr<BaseSprites> coinSp;
	const int ExitNeedCoinsNum = 1;
	const int TexNumberBegin = 3;	//�e�N�X�`���ԍ�
	const int CoinSpNumSize = 3;	//�T�C�Y
	std::unique_ptr<BaseSprites> coinSpNum[3];

};

