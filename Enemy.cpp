#include "Enemy.h"
#include <cassert>
#include "MathUtility.h"
#include "Player.h"

Enemy::~Enemy() {
	
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	
}

void Enemy::Initialize(Model* model) {
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("uvChecker.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = {10.0f, 0.0f, 100.0f};

	ApproachInitialize();
}

void Enemy::Update() {

	//デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	    //弾更新
	for (EnemyBullet* bullet : bullets_) {
		    bullet->Update();
	}

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

	for (EnemyBullet* bullet : bullets_) {
	bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachUpdate() {
	velocity_ = {0, 0, -0.2f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	--fireTimer;

	if (fireTimer <= 0) {
		Fire();
		fireTimer = kFireInterval;
	}

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	velocity_ = {0.2f, 0.2f, 0.2f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
}

void Enemy::Fire() {

	assert(player_);

	const float kBulletSpeed = 1.0f;


	//自キャラのワールド座標を取得する
	Vector3 b = player_->GetWorldPosition();
	//敵キャラのワールド座標を取得する
	Vector3 a = GetWorldPosition();
	//敵キャラ→自キャラの差分ベクトルを求める
	Vector3 c = Subtruct(b, a);
	//ベクトルの正規化
	float length = sqrtf(c.x * c.x + c.y * c.y + c.z * c.z);
	Vector3 dir = {c.x / length, c.y / length, c.z / length};

	//ベクトルの長さを、早さに合わせる
	Vector3 velocity(kBulletSpeed * dir.x, kBulletSpeed * dir.y, kBulletSpeed * dir.z);

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	bullets_.push_back(newBullet);


}

void Enemy::ApproachInitialize() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision() {}
