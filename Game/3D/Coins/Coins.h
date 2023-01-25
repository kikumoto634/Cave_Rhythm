#pragma once
#include "../BaseObjObject.h"

class Coins : public BaseObjObject
{
private:
	const int LostMaxBeat = 8;

public:
	~Coins();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(std::string filePath, bool IsSmoothing = false) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Camera* camera) override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �㏈��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �����蔻��
	/// </summary>
	void OnCollision(const CollisionInfo& info) override;

	/// <summary>
	/// �o��
	/// </summary>
	void Pop(Vector3 pos);

	/// <summary>
	/// �o���\
	/// </summary>
	bool PopPossible();
	
	/// <summary>
	/// �R�C���l��
	/// </summary>
	bool GetCoin();

	//Getter
	bool GetIsAlive() {return IsAlive;}

private:
	//�o��
	bool IsAlive = false;

	//�l��
	bool IsGet = false;

	//�������̍��W
	Vector3 DeadPos = {50,50,50};

	//���X�g����(�r�[�g���ŏ���)
	int lostBeat = 0;
};

