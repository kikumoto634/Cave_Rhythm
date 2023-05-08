#include "Exit.h"
#include "TextureUtility.h"

using namespace std;


void Exit::Initialize(std::string filePath, bool IsSmmothing)
{
	BaseObjObject::Initialize(filePath, IsSmmothing);

	SetModel(model);

	//コライダー
	ColliderSet();

	//スプライト
	sp_ = make_unique<ExitSprite>();
	sp_->Initialize();

	//階段モデル
	stairsModel_ = new ObjModelManager();
	stairsModel_->CreateModel("Stairs");
}

void Exit::Update(Camera *camera)
{
	this->camera = camera;

	//拍終わり時
	if(IsBeatEnd){
		
		//プレイヤー接触時
		if(isPlayerContact_){
			//サイズ変更
			if(ScaleChange(ScaleMax, ScaleMin, scaleEndTime)){
				IsBeatEnd = false;
				isPlayerContact_ = false;
			}
		}
	}

	sp_->Update(isOpen_, isPlayerContact_);

	BaseObjObject::Update(this->camera);
}


void Exit::Draw2D()
{
	sp_->Draw();
}

void Exit::Finalize()
{
	delete stairsModel_;
	stairsModel_ = nullptr;;

	sp_->Finalize();

	BaseObjObject::Finalize();
}

void Exit::OnCollision(const CollisionInfo &info)
{
	if(info.collider->GetAttribute() == COLLISION_ATTR_ALLIES){
		isPlayerContact_ = true;
	}
}


void Exit::ColliderSet()
{
	meshCollider_ = new MeshCollider;
	SetCollider(meshCollider_);
	//属性セット
	meshCollider_->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	meshCollider_->ConstructTriangles(model);
}

