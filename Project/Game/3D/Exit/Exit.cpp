#include "Exit.h"
#include "TextureUtility.h"

using namespace std;


void Exit::Initialize(std::string filePath, bool IsSmmothing)
{
	BaseObjObject::Initialize(filePath, IsSmmothing);

	SetModel(model);

	//コライダー追加
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	//属性セット
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	collider->ConstructTriangles(model);

	//スプライト
	coinSp = make_unique<BaseSprites>();
	coinSp->Initialize(coin_tex.number);
	coinSp->SetSize({40,40});
	coinSp->SetAnchorPoint({0.5f,0.5f});

	for(int i = 0; i < CoinSpNumSize; i++){
		coinSpNum[i] = make_unique<BaseSprites>();
		coinSpNum[i]->Initialize(cross_tex.number);
		coinSpNum[i]->SetSize({40,40});
		coinSpNum[i]->SetAnchorPoint({0.5f,0.5f});
	}
	NeedCoinSpriteUpdate();

	exitOpenSp = make_unique<BaseSprites>();
	exitOpenSp->Initialize(buttonZ_tex.number);
	exitOpenSp->SetSize({30,30});
	exitOpenSp->SetAnchorPoint({0.5f,0.5f});

	//階段モデル
	stairsModel = new ObjModelManager();
	stairsModel->CreateModel("Stairs");
}

void Exit::Update(Camera *camera)
{
	this->camera = camera;

	//拍終わり時
	if(IsBeatEnd){
		
		//プレイヤー接触時
		if(IsPlayerContact){
			//サイズ変更
			if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
				IsBeatEnd = false;
				IsPlayerContact = false;
			}
		}
	}

	coinSp->Update();
	for(int i = 0; i < CoinSpNumSize; i++){
		Vector2 pos = coinSp->GetPosition() + Vector2{static_cast<float>(40+(i*40)), 0};
		coinSpNum[i]->SetPosition(pos);
		coinSpNum[i]->Update();
	}
	{
		Vector2 pos = coinSp->GetPosition() + Vector2{-40,20};
		exitOpenSp->SetPosition(pos);
		exitOpenSp->Update();
	}

	BaseObjObject::Update(this->camera);
}


void Exit::Draw2D()
{
	if(!IsPlayerContact) return;
	coinSp->Draw();
	for(int i = 0; i < CoinSpNumSize; i++){
		coinSpNum[i]->Draw();
	}

	if(!IsOpen) return ;
	exitOpenSp->Draw();
}

void Exit::Finalize()
{
	delete stairsModel;
	stairsModel = nullptr;;

	coinSp->Finalize();
	for(int i = 0; i < CoinSpNumSize; i++){
		coinSpNum[i]->Finalize();
	}
	exitOpenSp->Finalize();

	BaseObjObject::Finalize();
}

void Exit::OnCollision(const CollisionInfo &info)
{
	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		IsPlayerContact = true;
	}
}


void Exit::NeedCoinSpriteUpdate()
{
	for(int i = 0; i < CoinSpNumSize; i++){		
		int value_Ten = ExitNeedCoinsNum/10;
		int value_One = ExitNeedCoinsNum - value_Ten*10;

		if(i == 0)coinSpNum[i]->SetTexNumber(16);
		else if(i == 1)coinSpNum[i]->SetTexNumber(TexNumberBegin + value_Ten);
		else if(i == 2)coinSpNum[i]->SetTexNumber(TexNumberBegin + value_One);
	}
}

