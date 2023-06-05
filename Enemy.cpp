#include "Enemy.h"
#include <cassert>
#include "MathUtility.h"

void Enemy::Initialize(Model* model) {
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("white1x1.png");

	worldTransform_.Initialize();


}

void Enemy::Update() {

	velocity_ = TransformNormal(velocity_, worldTransform_.matWorld_);

	switch (phase_) {
	case Phase::Approach:
	

		ApproachUpdate();

		break;
	case Phase::Leave:

		LeaveUpdate();

		break;
	}


	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ApproachUpdate() {
	velocity_ = {0, 0, -0.2f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	velocity_ = {0.2f, 0.2f, 0.2f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}
