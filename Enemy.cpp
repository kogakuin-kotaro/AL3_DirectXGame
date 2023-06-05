#include "Enemy.h"
#include <cassert>
#include "MathUtility.h"

void Enemy::Initialize(Model* model) {
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 0};

}

void Enemy::Update() {
	Vector3 velocity(0, 0, -0.2f);

	velocity_ = TransformNormal(velocity, worldTransform_.matWorld_);

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);

	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
