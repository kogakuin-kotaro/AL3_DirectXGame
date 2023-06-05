#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"


GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("white1x1.png");

	model_ = Model::Create();

	viewProjection_.Initialize();

	player_ = new Player();

	player_->Initialize(model_, textureHandle_);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	enemy_ = new Enemy();

	enemy_->Initialize(model_);

	enemy_->SetPlayer(player_);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_ = new Skydome();

	skydome_->Initialize(modelSkydome_);


}

void GameScene::Update() { 

	debugCamera_->Update();

	player_->Update();

	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}

#endif

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	} else {

		viewProjection_.UpdateMatrix();
	}

	if (enemy_) {
		enemy_->Update();
	}

	skydome_->Update();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

		player_->Draw(viewProjection_);
		if (enemy_) {
		    enemy_->Draw(viewProjection_);
	    }
	    skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() { 

	//判定対象AとBの定座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
	//敵弾の座標
		posB = bullet->GetWorldPosition();

			if ((posB.x - posA.x) * (posB.x - posA.x) + 
				(posB.y - posA.y) * (posB.y - posA.y) +
		        (posB.z - posA.z) * (posB.z - posA.z) <=
			    player_->Range * player_->Range + 
				bullet->Range * bullet->Range) {

				player_->OnCollision();
			    bullet->OnCollision();
		
			}

	}


#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵キャラの座標
	posA = enemy_->GetWorldPosition();

	//敵キャラと自弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		//敵弾の座標
		posB = bullet->GetWorldPosition();

			if ((posB.x - posA.x) * (posB.x - posA.x) +
				(posB.y - posA.y) * (posB.y - posA.y) +
				(posB.z - posA.z) * (posB.z - posA.z) <=
				enemy_->Range * enemy_->Range +
				bullet->Range * bullet->Range) {

				enemy_->OnCollision();
				bullet->OnCollision();
			};
	}

#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	for (PlayerBullet* bulletP : playerBullets) {
		//敵弾の座標
		posA = bulletP->GetWorldPosition();

			for (EnemyBullet* bulletE : enemyBullets) {
			//敵弾の座標
			posB = bulletE->GetWorldPosition();

				if ((posB.x - posA.x) * (posB.x - posA.x) + 
					(posB.y - posA.y) * (posB.y - posA.y) +
					(posB.z - posA.z) * (posB.z - posA.z) <=
					bulletE->Range * bulletE->Range + 
					bulletP->Range * bulletP->Range) {

				bulletP->OnCollision();
				bulletE->OnCollision();
				};
			}
	}
#pragma endregion

}
