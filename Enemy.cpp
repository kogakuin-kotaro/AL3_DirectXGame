#include "Enemy.h"
#include <cassert>
#include "MathUtility.h"
#include "Player.h"
#include "GameScene.h"
#include "ImGuiManager.h"

Enemy::~Enemy() {
	
	//for (EnemyBullet* bullet : bullets_) {
	//	delete bullet;
	//}
	
}

void Enemy::Initialize(
    Model* model1, Model* model2, Model* model3, Model* bulletmodel,
	Model* wiremodel1,Model* wiremodel2, Model* wirebulletmodel,
	const Vector3& position,
    const int& enemyNum) {
	assert(model1);
	assert(model2);
	assert(model3);
	assert(bulletmodel);
	assert(wiremodel1);
	assert(wiremodel2);
	assert(wirebulletmodel);

	enemy1model_ = model1;
	enemy2model_ = model2;
	enemy3model_ = model3;
	enemyBulletmodel_ = bulletmodel;
	wireEnemy1model_ = wiremodel1;
	wireEnemy2model_ = wiremodel2;
	wireEnemyBulletmodel_ = wirebulletmodel;
	enemyNum_ = enemyNum;

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	ApproachInitialize();
}

void Enemy::Update() {

	//デスフラグの立った弾を削除
	//enemyBullets_.remove_if([](EnemyBullet* bullet) {
	//	if (bullet->IsDead()) {
	//		delete bullet;
	//		return true;
	//	}
	//	return false;
	//});

	    //弾更新
	//for (EnemyBullet* bullet : bullets_) {
	//	    bullet->Update();
	//}

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
	if (enemyNum_ == 1) {
		if (GetWorldPosition().z >= 0) {
			enemy1model_->Draw(worldTransform_, viewProjection);
		} else {
			wireEnemy1model_->Draw(worldTransform_, viewProjection);
		}
	} else if (enemyNum_ == 2) {
		if (GetWorldPosition().z >= 0) {
			enemy2model_->Draw(worldTransform_, viewProjection);
		} else {
			wireEnemy2model_->Draw(worldTransform_, viewProjection);
		}
	} else if (enemyNum_ == 3) {
		enemy3model_->Draw(worldTransform_, viewProjection);
	}


	//for (EnemyBullet* bullet : bullets_) {
	//bullet->Draw(viewProjection);
	//}
}

void Enemy::ApproachUpdate() {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	--fireTimer;
	velocity_.y += 0.005f;
	velocity_.z -= 0.006f;
	if (enemyNum_ == 2) {
		if (fireTimer <= 0) {
			Fire();
			fireTimer = kFireInterval;
		}
	}


	if (worldTransform_.translation_.z < 0.0f) {
		if (enemyNum_ != 3) {
			fireTimer = kFireInterval;
		} else {
			Fire();
		}
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveUpdate() {
	

	if (enemyNum_ != 3) {
		velocity_ = {0.0f, 0.0f, -1.0f};
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
		--fireTimer;
		if (fireTimer <= 0) {
			isDead_ = true;
		}
	} else {
		isDead_ = true;
	}

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
	
	if (enemyNum_ == 3) {
		velocity = {0, 0, 0};
	}

	EnemyBullet* newBullet = new EnemyBullet();

	gameScene_->AddEnemyBullet(newBullet);

	newBullet->Initialize(
	    enemyBulletmodel_, wireEnemyBulletmodel_, enemy3model_, worldTransform_.translation_, velocity,
	    enemyNum_, 0);
	

	//enemyBullets_.push_back(newBullet);
	

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

Vector3 Enemy::GetScreenPosition(const ViewProjection& viewProjection) {

	Vector3 screenPosition = GetWorldPosition();

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewport =
	    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	screenPosition = Transform(screenPosition, matViewProjectionViewport);



	
	return screenPosition;
}

void Enemy::OnCollision() { 
	isDead_ = true;
	isRockon_ = false;
	gameScene_->ScoreSetter(100);
}
