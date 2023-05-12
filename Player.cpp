#include "Player.h"
#include <cassert>
#include "mathetc.h"
#include "ImGuiManager.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
};

void Player::Update() { 
	worldTransform_.TransferMatrix();

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpead = 0.2f;

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

	//移動制限
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//行列更新
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	worldTransform_.matWorld_ = MakeAffineMatrx(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//debug
	ImGui::Begin("Debug");
	ImGui::SliderFloat("playerX", &worldTransform_.translation_.x, -34.0f, 34.0f);
	ImGui::SliderFloat("playerY", &worldTransform_.translation_.y, -18.0f, 18.0f);
	ImGui::SliderFloat("playerZ", &worldTransform_.translation_.z, -20.0f, 20.0f);
	ImGui::End();

}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
