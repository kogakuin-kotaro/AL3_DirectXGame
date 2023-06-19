#include "Skydome.h"
#include <cassert>
#include "GameScene.h"

void Skydome::Initialize(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_.y += 400;

	uint32_t textureHandle = TextureManager::Load("./Resources/Images/skydome2.png");
	sprite_ = Sprite::Create(textureHandle, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f});
}

void Skydome::Update() {
	worldTransform_.UpdateMatrix(); 
	worldTransform_.rotation_.x += 0.02f;
}

void Skydome::Draw(ViewProjection& viewProjection) {
	if (gameScene_->GetBossIsAlive() == false) {
		model_->Draw(worldTransform_, viewProjection);

	} else {
		model_->Draw(worldTransform_, viewProjection, sprite_->GetTextureHandle());

	}
}
