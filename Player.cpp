#include "Player.h"
#include <cassert>
#include "MathUtility.h"
#include "ImGuiManager.h"
#include "Enemy.h"
#include "GameScene.h"

Player::~Player() {
	//for (PlayerBullet* bullet : bullets_) {
	//	delete bullet;
	//}
	//delete sprite2DReticle_;
}

void Player::Initialize(Model* model, Model* bulletmodel, Vector3& position) {
	assert(model);
	model_ = model;
	playerBulletmodel_ = bulletmodel;
	//textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	worldTransform_.translation_ = Add(worldTransform_.translation_, position);
	worldTransform_.scale_ = Multiply(worldTransform_.scale_, 2);


	worldTransform3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("./Resources/Images/target.png");
	sprite2DReticle_ = Sprite::Create(
	    textureReticle,
	    {640.0f, 320.0f},
	    {1.0f, 0.0f, 0.0f, 0.0f}, {0.5f, 0.5f});

		isHit_ = false;
		isDead_ = false;

};

void Player::Update(const ViewProjection& viewProjection) {


	//キャラクターの移動速さ
	float kCharacterSpead = 0.4f;
	worldTransform_.TransferMatrix();
	move_ = {0, 0, 0};
	//ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move_.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpead;
		move_.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpead;
	}

	if (isHit_ == false) {
		//座標移動（ベクトルの加算）
		worldTransform_.translation_ = Add(worldTransform_.translation_, move_);

		//回転
		Rotate();

		//攻撃
		Attack();
	} else {
		worldTransform_.scale_ = scale_;

		scale_.x -= 0.1f;
		scale_.y -= 0.1f;
		scale_.z -= 0.1f;

		if (scale_.x <= 0.0f && scale_.y <= 0.0f && scale_.z <= 0.0f) {
			isHit_ = false;
			isDead_ = true;
		}

	};


	//移動制限
	const float kMoveLimitX = 18.0f;
	const float kMoveLimitY = 10.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);


	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 10.0f;
		spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 10.0f;

		sprite2DReticle_->SetPosition(spritePosition);
	}

	CursorTo3DReticle(viewProjection, spritePosition);

	sprite2DReticle_->SetColor({1, 1, 1, 0.8f});

	//行列更新
	worldTransform_.UpdateMatrix(); 




}

void Player::Draw(ViewProjection& viewProjection) { 
	//プレイヤー描画
	if (isHit_ == false) {
		model_->Draw(worldTransform_, viewProjection);
	} else {
		playerBulletmodel_->Draw(worldTransform_, viewProjection);
	}

	//弾描画
	//for (PlayerBullet* bullet : bullets_) {
	//	bullet->Draw(viewProjection);
	//}

	//Reticlemodel_->Draw(worldTransform3DReticle_, viewProjection);
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
	

	if (bulletTimer_ > 0) {
		bulletTimer_--;
	}

	/*if (input_->PushKey(DIK_SPACE) && bulletTimer_ <= 0) */
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && bulletTimer_ <= 0) {
		const float kBulletSpeed = 2.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = Subtruct(worldTransform3DReticle_.translation_, GetWorldPosition());
		velocity = Multiply(Normalize(velocity), kBulletSpeed);

		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		PlayerBullet* newBullet = new PlayerBullet();

		gameScene_->AddPlayerBullet(newBullet);

		gameScene_->ScoreSetter(1);

		newBullet->Initialize(
		    playerBulletmodel_, GetWorldPosition(), velocity);

		//bullets_.push_back(newBullet);
		bulletTimer_ = 5;
	}
}

bool Player::Rockon(const Vector3& enemypos_,const Vector3& enemyWorldPos_) {
	if (enemypos_.x < sprite2DReticle_->GetPosition().x + 20 &&
	    enemypos_.x > sprite2DReticle_->GetPosition().x - 20 &&
	    enemypos_.y < sprite2DReticle_->GetPosition().y + 20 &&
	    enemypos_.y > sprite2DReticle_->GetPosition().y - 20 && enemyWorldPos_.z > 0.0f)
	{

		sprite2DReticle_->SetPosition({enemypos_.x, enemypos_.y});
		sprite2DReticle_->SetColor({1, 0, 0, 0.8f});
		return true;

	} else {
		return false;
	}
}

void Player::WorldTo3DReticle() {
	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{
		const float kDistancePlayerTo3DReticle = 50.0f;

		Vector3 offset = {0.0f, 0.0f, 1.0f};

		offset = TransformNormal(offset, worldTransform_.matWorld_);

		offset = Multiply(Normalize(offset), kDistancePlayerTo3DReticle);

		worldTransform3DReticle_.translation_ =  offset;
		worldTransform3DReticle_.TransferMatrix();
		worldTransform3DReticle_.UpdateMatrix();
	
	}
}

void Player::ReticleTo2DReticle(const ViewProjection& viewProjection) {
	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{
		Vector3 positionReticle = worldTransform3DReticle_.translation_;

		Matrix4x4 matViewport =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		Matrix4x4 matViewProjectionViewport =
		    Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

		positionReticle = Transform(positionReticle, matViewProjectionViewport);

		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}
}

void Player::CursorTo3DReticle(const ViewProjection& viewProjection, Vector2 spritePosition) {

	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクルを配置
	{

		// GetCursorPos(&spritePosition);

		// HWND hwnd = WinApp::GetInstance()->GetHwnd();
		// ScreenToClient(hwnd, &spritePosition);

		sprite2DReticle_->SetPosition({(float)spritePosition.x, (float)spritePosition.y});

		Matrix4x4 matViewport =
			MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		Matrix4x4 matVPV =
			Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

		Matrix4x4 matInverseVPV = Inverce(matVPV);

		// Multiply(matVPV, matInverseVPV);

		Vector3 posNear = Vector3((float)spritePosition.x, (float)spritePosition.y, 0);
		Vector3 posFar = Vector3((float)spritePosition.x, (float)spritePosition.y, 1);

		posNear = Transform(posNear, matInverseVPV);
		posFar = Transform(posFar, matInverseVPV);

		Vector3 mouseDirection = Subtruct(posFar, posNear);
		mouseDirection = Normalize(mouseDirection);

		const float kDistanceTestObject = 100.0f;

		worldTransform3DReticle_.translation_ = {
			posNear.x + mouseDirection.x * kDistanceTestObject,
			posNear.y + mouseDirection.y * kDistanceTestObject,
			posNear.z + mouseDirection.z * kDistanceTestObject,
		};

		worldTransform3DReticle_.UpdateMatrix();
		worldTransform3DReticle_.TransferMatrix();


	}
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() { 
	if (!isHit_) {
		gameScene_->FreezeTimeSetter(60);
		scale_ = {10, 10, 10};
	}
	isHit_ = true;
}

void Player::DrawUI() { 
	sprite2DReticle_->Draw();







	//    Novice::DrawSprite(342, 670, numberTexHandle[digitNumber[0]], 1, 1, 0.0f, 0xFFFFFFFF);
	//Novice::DrawSprite(383, 670, numberTexHandle[digitNumber[1]], 1, 1, 0.0f, 0xFFFFFFFF);
	//Novice::DrawSprite(424, 670, numberTexHandle[digitNumber[2]], 1, 1, 0.0f, 0xFFFFFFFF);
	//Novice::DrawSprite(465, 670, numberTexHandle[digitNumber[3]], 1, 1, 0.0f, 0xFFFFFFFF);
	//Novice::DrawSprite(506, 670, numberTexHandle[digitNumber[4]], 1, 1, 0.0f, 0xFFFFFFFF);
	//Novice::DrawSprite(547, 670, numberTexHandle[digitNumber[5]], 1, 1, 0.0f, 0xFFFFFFFF);
	//Novice::DrawSprite(588, 670, numberTexHandle[digitNumber[6]], 1, 1, 0.0f, 0xFFFFFFFF);
}





