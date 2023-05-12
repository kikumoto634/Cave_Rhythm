#include "TitleScene.h"

#include "SceneManager.h"
#include "TutorialScene.h"
#include "GameScene.h"
#include "Easing.h"

#include "AudioUtility.h"
#include "TextureUtility.h"

using namespace std;
using namespace DirectX;

TitleScene::TitleScene(DirectXCommon *dxCommon, Window *window)
	: BaseScene(
		dxCommon,
		window)
{
}

void TitleScene::Application()
{
	BaseScene::Application();
}

void TitleScene::Initialize()
{
	BaseScene::Initialize();

	ObjectInitialize();

	SpriteInitialize();

	audio_ = Audio::GetInstance();

	//ライト
	lightGroup_ = LightGroup::Create();
	//色設定
	lightGroup_->SetAmbientColor(LightColor);
	//3Dオブジェクト(.obj)にセット
	ObjModelObject::SetLight(lightGroup_);

	lightGroup_->SetDirLightActive(0, true);
	lightGroup_->SetDirLightActive(1, false);
	lightGroup_->SetDirLightActive(2, false);

	//シーン遷移(FadeOut)
	fadeInSize_ = {static_cast<float>(window->GetWindowWidth()), static_cast<float>(window->GetWindowHeight())};
	fade_ = make_unique<BaseSprites>();
	fade_->Initialize(white1x1_tex.number);
	fade_->SetColor({fadeColor_.x,fadeColor_.y,fadeColor_.z,fadeColor_.w});
	fade_->SetSize({fadeInSize_});
}

void TitleScene::Update()
{
	BaseScene::Update();

	if(input->Trigger(DIK_Z) && !isNextSceneChange_){
		isNextSceneChange_ = true;
		audio_->PlayWave(coinGet_audio.number,coinGet_audio.volume);
	}

#pragma region _3DObj Update
	for(auto it = objs_.begin(); it != objs_.end(); ++it){
		(*it)->Update(camera);
	}
#pragma endregion

#pragma region _2DObj Update
	SceneChange();
	fade_->Update();

	for(auto it = sp_.begin(); it != sp_.end(); it++){
		(*it)->Update();
	}
#pragma endregion

#pragma region Common Update
	lightGroup_->Update();
#pragma endregion

#ifdef _DEBUG
	//Scene
	{
		//座標
		ImGui::SetNextWindowPos(ImVec2{1000,40});
		//サイズ
		ImGui::SetNextWindowSize(ImVec2{280,100});
		ImGui::Begin("SCENE");

		ImGui::Text("Now:Title   Next:Home");
		if(!IsPrevSceneChange && ImGui::Button("NextScene")){
			IsNextSceneChange = true;
		}
		ImGui::Text("Now:Title   Next:Debug");
		if(!IsPrevSceneChange && ImGui::Button("DebugScene")){
			IsDebugScene = true;
		}

		ImGui::End();
	}
#endif // _DEBUG


	BaseScene::EndUpdate();
}

void TitleScene::Draw()
{

	BaseScene::Draw();
#pragma region _3DObj Draw
	for(auto it = objs_.begin(); it != objs_.end(); ++it){
		(*it)->Draw();
	}
#pragma endregion

#pragma region _2D_UIDraw
	Sprite::SetPipelineState();
	for(auto it = sp_.begin(); it != sp_.end(); it++){
		(*it)->Draw();
	}
	fade_->Draw();
#pragma endregion

	BaseScene::EndDraw();
}

void TitleScene::Finalize()
{
	audio_->Finalize();

	for(auto it = sp_.begin(); it != sp_.end(); it++){
		(*it)->Finalize();
	}

	fade_->Finalize();

	delete lightGroup_;
	lightGroup_ = nullptr;

	for(auto it = objs_.begin(); it != objs_.end(); it++){
		(*it)->Finalize();
	}

	BaseScene::Finalize();
}


void TitleScene::NextSceneChange()
{
	sceneManager->SetNextScene(new TutorialScene(dxCommon,window));
}

void TitleScene::DebugSceneChange()
{
	sceneManager->SetNextScene(new GameScene(dxCommon,window));
}

void TitleScene::SceneChange()
{
	//PrevSceneからの移動後処理
	if(isPrevSceneChange_){
		if(fadeColor_.w <= 0){
			isPrevSceneChange_ = false;
			fadeCurrentFrame_ = 0;
			return;
		}

		fadeColor_.w = 
			Easing_Linear_Point2(1,0,Time_OneWay(fadeCurrentFrame_, FadeSecond));
		fade_->SetColor({fadeColor_.x,fadeColor_.y,fadeColor_.z,fadeColor_.w});
	}
	//NextSceneへの移動
	else if(isNextSceneChange_ || isDebugScene_){

		if(fadeColor_.w >= 1){
			if(isNextSceneChange_)NextSceneChange();
			else if(isDebugScene_)DebugSceneChange();
		}

		fadeColor_.w = 
			Easing_Linear_Point2(0,1,Time_OneWay(fadeCurrentFrame_, FadeSecond));
		fade_->SetColor({fadeColor_.x,fadeColor_.y,fadeColor_.z,fadeColor_.w});
	}
}


void TitleScene::ObjectInitialize()
{
	//プレイヤー
	unique_ptr<BaseObjObject> playerObj_ = make_unique<BaseObjObject>();
	playerObj_->Initialize("human2");
	playerObj_->SetPosition(PlayerPos);
	playerObj_->SetRotation(PlayerRot);
	playerObj_->SetScale(PlayerScale);
	objs_.push_back(move(playerObj_));

	//エネミー
	unique_ptr<BaseObjObject> enemyObj_ = make_unique<BaseObjObject>();
	enemyObj_->Initialize("slime");
	enemyObj_->SetPosition(SlimePos);
	enemyObj_->SetRotation(SlimeRot);
	enemyObj_->SetScale(SlimeScale);
	objs_.push_back(move(enemyObj_));

	unique_ptr<BaseObjObject> enemyObj2_ = make_unique<BaseObjObject>();
	enemyObj2_->Initialize("Skeleton");
	enemyObj2_->SetPosition(SkeltonPos);
	enemyObj2_->SetRotation(SkeltonRot);
	enemyObj2_->SetScale(SkeltonScale);
	objs_.push_back(move(enemyObj2_));

	unique_ptr<BaseObjObject> backObj_ = make_unique<BaseObjObject>();
	backObj_->Initialize("wall");
	backObj_->SetPosition(WallBackPos);
	objs_.push_back(move(backObj_));
}

void TitleScene::SpriteInitialize()
{
	unique_ptr<BaseSprites> title_ = make_unique<BaseSprites>();
	title_->Initialize(titleLogo_tex.number);
	title_->SetPosition(TitlePos);
	title_->SetSize(TitleSize);
	title_->SetAnchorPoint(TitleAnc);
	sp_.push_back(move(title_));

	unique_ptr<BaseSprites> pushText_ = make_unique<BaseSprites>();
	pushText_->Initialize(pushText_tex.number);
	pushText_->SetPosition(PushTextPos);
	pushText_->SetSize(PushTextSize);
	pushText_->SetAnchorPoint(PushtextAnc);
	sp_.push_back(move(pushText_));

	unique_ptr<BaseSprites> button_ = make_unique<BaseSprites>();
	button_->Initialize(buttonZ_tex.number);
	button_->SetPosition(ButtonPos);
	button_->SetSize(ButtonSize);
	button_->SetAnchorPoint(ButtonAnc);
	sp_.push_back(move(button_));
}
