#include "Boss.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "MathUtility.h"
#include "Player.h"
#include <cassert>

Boss::~Boss() {

	// for (EnemyBullet* bullet : bullets_) {
	//	delete bullet;
	// }
}

void Boss::Initialize(
    Model* model, Model* bullet1model, Model* bullet2model, Model* wirebullet1model, const Vector3& position) {
	assert(model);
	assert(bullet1model);
	assert(bullet2model);
	assert(wirebullet1model);


	bossmodel_ = model;

	enemyBullet1model_ = bullet1model;
	enemyBullet2model_ = bullet2model;
	wireEnemyBullet1model_ = wirebullet1model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	bit1WorldTransform_.Initialize();
	bit2WorldTransform_.Initialize();
	bit3WorldTransform_.Initialize();
	bit4WorldTransform_.Initialize();

	bit1WorldTransform_.translation_.x = worldTransform_.translation_.x + 10;
	bit2WorldTransform_.translation_.y = worldTransform_.translation_.y + 30;
	bit3WorldTransform_.translation_.x = worldTransform_.translation_.x - 10;
	bit4WorldTransform_.translation_.y = worldTransform_.translation_.y + 10;

	SpawnInitialize();
}

void Boss::Update() {

	//デスフラグの立った弾を削除
	// enemyBullets_.remove_if([](EnemyBullet* bullet) {
	//	if (bullet->IsDead()) {
	//		delete bullet;
	//		return true;
	//	}
	//	return false;
	//});

	//弾更新
	// for (EnemyBullet* bullet : bullets_) {
	//	    bullet->Update();
	// }

	velocity_ = TransformNormal(velocity_, worldTransform_.matWorld_);


	switch (phase_) {
	case Phase::Spawn:

		SpawnUpdate();

		break;

	case Phase::Battle:

		BattleUpdate();

		break;

	case Phase::Leave:

		LeaveUpdate();

		break;
	}

	worldTransform_.UpdateMatrix();
	bit1WorldTransform_.UpdateMatrix();
	bit2WorldTransform_.UpdateMatrix();
	bit3WorldTransform_.UpdateMatrix();
	bit4WorldTransform_.UpdateMatrix();
}

void Boss::Draw(ViewProjection& viewProjection) {

	bossmodel_->Draw(worldTransform_, viewProjection);
	//bossmodel_->Draw(bit1WorldTransform_, viewProjection);
	//bossmodel_->Draw(bit2WorldTransform_, viewProjection);
	//bossmodel_->Draw(bit3WorldTransform_, viewProjection);
	//bossmodel_->Draw(bit4WorldTransform_, viewProjection);


	// for (EnemyBullet* bullet : bullets_) {
	// bullet->Draw(viewProjection);
	// }
}

void Boss::SpawnUpdate() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	--fireTimer;
	velocity_.y += 0.005f;
	velocity_.z += 0.005f;


	//if (enemyNum_ == 2) {
	//	if (fireTimer <= 0) {
	//		Fire();
	//		fireTimer = kFireInterval;
	//	}
	//}

	if (worldTransform_.translation_.z > 100.0f) {
		//if (enemyNum_ != 3) {
		//	fireTimer = kFireInterval;
		//} else {
		//	Fire();
		//}
		phase_ = Phase::Battle;
	}
}

void Boss::BattleUpdate() {

	velocityR_.z += kRotspeed_;
	if (velocityR_.z > 1.0f) {
		kRotspeed_ = kRotspeed_ * -1;
		patternNum_ = rand() % 3 + 1;
	} else if (velocityR_.z < -1.0f) {
		kRotspeed_ = kRotspeed_ * -1;
		patternNum_ = rand() % 3 + 1;
	}
	worldTransform_.rotation_.z += velocityR_.z;

	--fireTimer;
	if (fireTimer <= 0) {
		Fire();
		fireTimer = 5;
	}



}

void Boss::LeaveUpdate() {

	gameScene_->BossIsAlive(false);
	isDead_ = true;

}

void Boss::Fire() {

	assert(player_);

	const float kBulletSpeedX = (rand() % 21 - static_cast<float>(10)) / 50;
	const float kBulletSpeedY = (rand() % 21 - static_cast<float>(10)) / 50;
	const float kBulletSpeedZ = 1.0f;
	Vector3 velocity = {kBulletSpeedX, kBulletSpeedY, -kBulletSpeedZ};

	//if (patternNum_ == 1) {
	//	//自キャラのワールド座標を取得する
	//	Vector3 b = player_->GetWorldPosition();
	//	//敵キャラのワールド座標を取得する
	//	Vector3 a = GetWorldPosition();
	//	//敵キャラ→自キャラの差分ベクトルを求める
	//	Vector3 c = Subtruct(b, a);
	//	//ベクトルの正規化
	//	float length = sqrtf(c.x * c.x + c.y * c.y + c.z * c.z);
	//	Vector3 dir = {c.x / length, c.y / length, c.z / length};

	//	//ベクトルの長さを、早さに合わせる
	//	//Vector3 velocity(kBulletSpeed * dir.x, kBulletSpeed * dir.y, kBulletSpeed * dir.z);

	//	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	//}



	EnemyBullet* newBullet = new EnemyBullet();

	gameScene_->AddEnemyBullet(newBullet);

	int bitNum = rand() % 4;
	Vector3 bulletPosition;

	if (bitNum == 0) {
		bulletPosition = GetBit1WorldPosition();
	} else if (bitNum == 1) {
		bulletPosition = GetBit2WorldPosition();
	} else if (bitNum == 2) {
		bulletPosition = GetBit3WorldPosition();
	} else if (bitNum == 3) {
		bulletPosition = GetBit4WorldPosition();
	}

	 newBullet->Initialize(
	    enemyBullet1model_, wireEnemyBullet1model_,enemyBullet2model_, bulletPosition, velocity,
	    enemyNum_, patternNum_);

	// enemyBullets_.push_back(newBullet);
}

void Boss::SpawnInitialize() {
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}

Vector3 Boss::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Boss::GetBit1WorldPosition() {
	Vector3 worldPos;

	worldPos.x = bit1WorldTransform_.matWorld_.m[3][0];
	worldPos.y = bit1WorldTransform_.matWorld_.m[3][1];
	worldPos.z = bit1WorldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Boss::GetBit2WorldPosition() {
	Vector3 worldPos;

	worldPos.x = bit2WorldTransform_.matWorld_.m[3][0];
	worldPos.y = bit2WorldTransform_.matWorld_.m[3][1];
	worldPos.z = bit2WorldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Boss::GetBit3WorldPosition() {
	Vector3 worldPos;

	worldPos.x = bit3WorldTransform_.matWorld_.m[3][0];
	worldPos.y = bit3WorldTransform_.matWorld_.m[3][1];
	worldPos.z = bit3WorldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Boss::GetBit4WorldPosition() {
	Vector3 worldPos;

	worldPos.x = bit4WorldTransform_.matWorld_.m[3][0];
	worldPos.y = bit4WorldTransform_.matWorld_.m[3][1];
	worldPos.z = bit4WorldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Boss::GetScreenPosition(const ViewProjection& viewProjection) {

	Vector3 screenPosition = GetWorldPosition();

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewport =
	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	screenPosition = Transform(screenPosition, matViewProjectionViewport);

	return screenPosition;
}

void Boss::OnCollision() {
	HP -= 1;
	gameScene_->ScoreSetter(10);
	if (HP <= 0) {
		phase_ = Phase::Leave;
		isRockon_ = false;
	}
}
