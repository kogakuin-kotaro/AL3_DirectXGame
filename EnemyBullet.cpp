#include "EnemyBullet.h"
#include <cassert>
#include "MathUtility.h"

void EnemyBullet::Initialize(Model* model, Vector3& position, const Vector3& velocity) {
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("sample.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;
}

void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() { 
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
