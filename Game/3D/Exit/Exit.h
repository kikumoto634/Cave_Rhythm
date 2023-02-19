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

	/// <summary>
	/// �o�����
	/// </summary>
	void ExitOpen()	{IsOpen = true;}

	/// <summary>
	/// �o���{��
	/// </summary>
	void ExitClose()	{IsOpen = false;}

	//�K�v�R�C���X�V
	void NeedCoinSpriteUpdate();

	//���f���ύX
	void ModelChange()	{this->object->SetModel(stairsModel), IsOpenAudioOnce = true;}

	//Getter
	inline bool GetIsPlayerContact() {return IsPlayerContact;}
	inline const int GetExitNeedCoinNum() {return ExitNeedCoinsNum;}
	inline bool GetIsOpenAudioOnce()	{return IsOpenAudioOnce;}
	inline BaseSprites* GetCoinSp()	{return coinSp.get();}

	//Setter
	inline void SetCoinSpPosition(Vector2 pos)	{coinSp->SetPosition(pos);}
	inline void SetExitOpenNeedCoin(int num)	{ExitNeedCoinsNum = num;}


private:

	//�v���C���[�ڐG
	bool IsPlayerContact = false;

	//�K�v�R�C����
	std::unique_ptr<BaseSprites> coinSp;
	int ExitNeedCoinsNum = 1;
	const int TexNumberBegin = 3;	//�e�N�X�`���ԍ�
	const int CoinSpNumSize = 3;	//�T�C�Y
	std::unique_ptr<BaseSprites> coinSpNum[3];

	//�J��
	bool IsOpen = false;
	std::unique_ptr<BaseSprites> exitOpenSp;

	//�K�i���f��
	ObjModelManager* stairsModel = nullptr;
	bool IsModelChange = false;
	bool IsOpenAudioOnce = false;
};

