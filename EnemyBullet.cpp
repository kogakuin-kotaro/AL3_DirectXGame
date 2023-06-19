#include "EnemyBullet.h"
#include <cassert>
#include "MathUtility.h"
#include "Player.h"
#include "ImGuiManager.h"

void EnemyBullet::Initialize(
    Model* model, Model* wiremodel, Model* BBmodel, Vector3& position, const Vector3& velocity,
    int enemyNum, int patternNum) {
	assert(model);
	assert(wiremodel);
	assert(BBmodel);

	model_ = model;
	wiremodel_ = wiremodel;
	bossBulletmodel_ = BBmodel;
	enemyNum_ = enemyNum;

	velocity_ = velocity;
	patternNum_ = patternNum;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	if (enemyNum_ == 3) {
		worldTransform_.scale_ = Multiply(worldTransform_.scale_, 2);
		Range = 2.0f;
		deathTimer_ = 10;
	}

	if (enemyNum_ == 2) {
		//// y
		//worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

		//// x
		//float velocityXZ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
		//worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);
	}

	if (patternNum_ == 3) {
		deathTimer_ = 110;
	}

}

void EnemyBullet::Update() {
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	if (enemyNum_ == 2 || patternNum_ == 2) {
		Vector3 toPlayer = Subtruct(player_->GetWorldPosition(), worldTransform_.translation_);

		Normalize(toPlayer);
		Normalize(velocity_);


		velocity_ = Multiply(Lerp(velocity_, toPlayer, 0.0005f), kBulletSpeed_);

		// y
		worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

		// x
		float velocityXZ = sqrtf((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
		worldTransform_.rotation_.x = std::atan2(-velocity_.y, velocityXZ);


	}

	if (patternNum_ == 3 && GetWorldPosition().z < 0) {
		velocity_ = {0, 0, 0};
		worldTransform_.translation_.z = 0.0f;
		worldTransform_.rotation_ = {0, 0, 0};
		worldTransform_.scale_ = Multiply(worldTransform_.scale_, 2);
		Range = 2.0f;

	}
	
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(ViewProjection& viewProjection) {
	if (patternNum_ == 3 && GetWorldPosition().z > 0) {

		bossBulletmodel_->Draw(worldTransform_, viewProjection);

	} else if (GetWorldPosition().z >= -3) {

		model_->Draw(worldTransform_, viewProjection);

	} else {
		wiremodel_->Draw(worldTransform_, viewProjection);
	}
}

void EnemyBullet::OnCollision() { isDead_ = true; }

Vector3 EnemyBullet::GetWorldPosition() { 
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
