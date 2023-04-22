#include "Player.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

#include "Easing.h"

void Player::Initialize(std::string filePath, bool IsSmoothing)
{
	BaseObjObject::Initialize(filePath, IsSmoothing);

	//ポインタ
	//入力
	input_ = Input::GetInstance();
	//武器
	weapon_ = new PlayerWeapon();
	weapon_->Initialize("Slash", false);
}

void Player::Update(Camera *camera)
{
	this->camera = camera;
	if(input_->Trigger(DIK_SPACE)) {
		weapon_->Attack();
	}

	//武器更新
	weapon_->SetPosition(world.translation + weaponOffset_);
	weapon_->SetRotation(GetRotation());
	weapon_->Update(this->camera);

	//ベース更新
	BaseObjObject::Update(this->camera);
}

void Player::Draw()
{
	weapon_->Draw();

	BaseObjObject::Draw();
}

void Player::Finalize()
{
	weapon_->Finalize();
	delete weapon_;
	weapon_ = nullptr;

	BaseObjObject::Finalize();
}

void Player::OnCollision(const CollisionInfo &info)
{
}

void Player::InputUpdate()
{
}
