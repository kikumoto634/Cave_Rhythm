#include "Exit.h"

using namespace std;

Exit::~Exit()
{
	//Finalize();
}

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

	window = Window::GetInstance();

	//スプライト
	coinSp = make_unique<BaseSprites>();
	coinSp->Initialize(13);
	coinSp->SetSize({40,40});
	coinSp->SetAnchorPoint({0.5f,0.5f});

	for(int i = 0; i < CoinSpNumSize; i++){
		coinSpNum[i] = make_unique<BaseSprites>();
		
		if(i == 0)coinSpNum[i]->Initialize(16);
		else if(i != 0)coinSpNum[i]->Initialize(TexNumberBegin+i-1);

		coinSpNum[i]->SetSize({40,40});
		coinSpNum[i]->SetAnchorPoint({0.5f,0.5f});
	}

	exitOpenSp = make_unique<BaseSprites>();
	exitOpenSp->Initialize(17);
	exitOpenSp->SetSize({30,30});
	exitOpenSp->SetAnchorPoint({0.5f,0.5f});
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

void Exit::Draw()
{
	BaseObjObject::Draw();
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

Vector2 Exit::ChangeTransformation(Vector3 targetpos)
{
	DirectX::XMMATRIX matViewport = 
	{
		(float)window->GetWindowWidth()/2, 0								  , 0, 0,
		0								 , -((float)window->GetWindowHeight())/2, 0, 0,
		0								 , 0								  , 1, 0, 
		(float)window->GetWindowWidth()/2, (float)window->GetWindowHeight()/2 , 0, 1,
	};
	DirectX::XMMATRIX matViewProjectionViewPort = camera->GetMatView() * camera->GetMatProjection() * matViewport;
	Vector3 positionreticle = Vector3Transform(targetpos, matViewProjectionViewPort);
	return Vector2{positionreticle.x, positionreticle.y};
}

Vector3 Exit::Vector3Transform(Vector3 &v, DirectX::XMMATRIX &m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	Vector3 result
	{
		(v.x*m.r[0].m128_f32[0] + v.y*m.r[1].m128_f32[0] + v.z*m.r[2].m128_f32[0] + m.r[3].m128_f32[0])/w,
		(v.x*m.r[0].m128_f32[1] + v.y*m.r[1].m128_f32[1] + v.z*m.r[2].m128_f32[1] + m.r[3].m128_f32[1])/w,
		(v.x*m.r[0].m128_f32[2] + v.y*m.r[1].m128_f32[2] + v.z*m.r[2].m128_f32[2] + m.r[3].m128_f32[2])/w
	};

	return result;
}
