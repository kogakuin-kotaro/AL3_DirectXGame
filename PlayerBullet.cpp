#include "PlayerBullet.h"
#include <cassert>
#include "MathUtility.h"
#include "Enemy.h"

void PlayerBullet::Initialize(
    Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);

	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = Add(worldTransform_.translation_, position);

	velocity_ = velocity;

}

void PlayerBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
	if (!isHit_) {
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	} else {
		worldTransform_.scale_ = scale_;

		scale_.x -= 0.1f;
		scale_.y -= 0.1f;
		scale_.z -= 0.1f;

		if (scale_.x <= 0.0f && scale_.y <= 0.0f && scale_.z <= 0.0f) {
			isHit_ = false;
			isDead_ = true;
		}
	}

	worldTransform_.UpdateMatrix();
}



void PlayerBullet::Rockon(Vector3 targetPos, bool rockonFlag) { 

	if (rockonFlag == true && isHit_ == false) {
	Vector3 toEnemy = Subtruct(targetPos, worldTransform_.translation_);

	Normalize(toEnemy);
	Normalize(velocity_);

	velocity_ = Multiply(Lerp(velocity_, toEnemy, 0.2f), 0.4f);
	}

	worldTransform_.UpdateMatrix(); 
}

void PlayerBullet::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void PlayerBullet::OnCollision() { 
	if (!isHit_) {
		scale_ = {4, 4, 4};
	}
	isHit_ = true;
}

Vector3 PlayerBullet::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}
