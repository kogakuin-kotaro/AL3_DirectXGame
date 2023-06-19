#include "Player.h"
#include <cassert>
#include "MathUtility.h"
#include "ImGuiManager.h"
#include "GameScene.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle,Vector3 &position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	worldTransform_.translation_ = Add(worldTransform_.translation_, position);

	worldTransform3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("./Resources/target.png");
	sprite2DReticle_ = Sprite::Create(
	    textureReticle,
	    {worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y},
	    {1, 1, 1, 1}, {0.5f, 0.5f});
};

void Player::Update(const ViewProjection& viewProjection) { 

	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpead = 0.2f;
	worldTransform_.TransferMatrix();

	//押した方向でベクトル変更
	//左右
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpead;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpead;
	}

	//上下
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpead;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpead;
	}

	//座標移動（ベクトルの加算）
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);


	//回転
	Rotate();

	//攻撃
	Attack();

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//移動制限
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	const float kDistancePlayerTo3DReticle = 50.0f;

	Vector3 offset = {0.0f, 0.0f, 1.0f};

	offset = Multiply(offset, worldTransform_.translation_);

	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));

	worldTransform3DReticle_.translation_ = Add(worldTransform3DReticle_.translation_, offset);
	worldTransform3DReticle_.UpdateMatrix();

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = worldTransform3DReticle_.translation_;

	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	Matrix4x4 matViewProjectionViewport =
	    Multiply(viewProjection.matView, Multiply(viewProjection.matProjection, matViewport));

	positionReticle = Transform(positionReticle, matViewProjectionViewport);

	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	//行列更新
	worldTransform_.UpdateMatrix(); 


	//debug
	ImGui::Begin("PlayerDebug");
	ImGui::SliderFloat("playerX", &worldTransform_.translation_.x, -34.0f, 34.0f);
	ImGui::SliderFloat("playerY", &worldTransform_.translation_.y, -18.0f, 18.0f);
	ImGui::SliderFloat("playerZ", &worldTransform_.translation_.z, -20.0f, 20.0f);
	ImGui::End();

}

void Player::Draw(ViewProjection& viewProjection) { 
	//プレイヤー描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);
}

void Player::Rotate() {
	const float kRotSpead = 0.02f;

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpead;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpead;

	}
}

void Player::Attack() { 
	if (input_->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = Subtruct(worldTransform3DReticle_.translation_, worldTransform_.translation_);
		velocity = Multiply(kBulletSpeed, Normalize(velocity));

		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}

void Player::DrawUI() {}



