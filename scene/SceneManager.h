#pragma once
#include "BaseScene.h"

class SceneManager
{
public:
	~SceneManager();

	static SceneManager* GetInstance();
	static void Delete();

	void SetNextScene(BaseScene* lnextScene)	{nextScene = lnextScene;}

	void Update();
	void Draw();

private:
	//�V���O���g��
	static SceneManager* instance;

	BaseScene* scene = nullptr;
	//���V�[��
	BaseScene* nextScene = nullptr;
};

