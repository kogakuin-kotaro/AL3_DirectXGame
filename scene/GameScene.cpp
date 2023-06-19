#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() { 

	delete screen_;

	delete TitleName_;
	delete Press_;
	delete Score_;
	delete Result_;

	delete digitNum1_;
	delete digitNum2_;
	delete digitNum3_;
	delete digitNum4_;
	delete digitNum5_;
	delete digitNum6_;
	delete digitNum7_;
	delete digitNum8_;


	delete playermodel_;
	delete playerBulletmodel_;
	delete enemy1model_;
	delete enemy2model_;
	delete enemy3model_;
	delete enemyBulletmodel_;
	delete wireEnemy1model_;
	delete wireEnemy2model_;
	delete wireEnemyBulletmodel_;
	delete bossmodel_;

	delete player_;
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
	delete debugCamera_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle_ = TextureManager::Load("white1x1.png");

	BGMsoundDataHandle_ = audio_->LoadWave("./Resources/Sounds/RE_SWARM.wav");
	SEsoundDataHandle_ = audio_->LoadWave("./Resources/Sounds/SE.wav");

	///
	/// Sprite置き場
	///

	//フラッシュとかトランジションとか

	screen_ = Sprite::Create(screenWHandle_, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.0f, 0.0f});
	//screenR_ = Sprite::Create(screenRHandle_, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.0f, 0.0f});
	//screenK_ = Sprite::Create(screenKHandle_, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 0.0f}, {0.0f, 0.0f});

	// UI用
	uint32_t titleNameHandle_ = TextureManager::Load("./Resources/Images/TitleName.png");
	TitleName_ =
	    Sprite::Create(titleNameHandle_, {640.0f, 360.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	uint32_t PressHandle_ = TextureManager::Load("./Resources/Images/Press.png");
	Press_ = Sprite::Create(PressHandle_, {640.0f, 420.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	uint32_t ScoreHandle_ = TextureManager::Load("./Resources/Images/Score.png");
	Score_ = Sprite::Create(
	    ScoreHandle_, {30.0f + (kSpace_ * 4.0f), 638.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	uint32_t ResultHandle_ = TextureManager::Load("./Resources/Images/Result.png");
	Result_ = Sprite::Create(
	    ResultHandle_, {640.0f, 300.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});


	ScoreDisplayCalc();


	numberTextureHandle[0] = TextureManager::Load("./Resources/Images/Number0.png");
	numberTextureHandle[1] = TextureManager::Load("./Resources/Images/Number1.png");
	numberTextureHandle[2] = TextureManager::Load("./Resources/Images/Number2.png");
	numberTextureHandle[3] = TextureManager::Load("./Resources/Images/Number3.png");
	numberTextureHandle[4] = TextureManager::Load("./Resources/Images/Number4.png");
	numberTextureHandle[5] = TextureManager::Load("./Resources/Images/Number5.png");
	numberTextureHandle[6] = TextureManager::Load("./Resources/Images/Number6.png");
	numberTextureHandle[7] = TextureManager::Load("./Resources/Images/Number7.png");
	numberTextureHandle[8] = TextureManager::Load("./Resources/Images/Number8.png");
	numberTextureHandle[9] = TextureManager::Load("./Resources/Images/Number9.png");



	 digitNum1_ = Sprite::Create(
	    numberTextureHandle[digitNumber[0]], {36.0f + (kSpace_ * 1.0f), 684.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	 digitNum2_=Sprite::Create(
	    numberTextureHandle[digitNumber[1]], {36.0f + (kSpace_ * 2.0f), 684.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	 digitNum3_=Sprite::Create(
	    numberTextureHandle[digitNumber[2]], {36.0f + (kSpace_ * 3.0f), 684.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	 digitNum4_=Sprite::Create(
	    numberTextureHandle[digitNumber[3]], {36.0f + (kSpace_ * 4.0f), 684.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	 digitNum5_=Sprite::Create(
	    numberTextureHandle[digitNumber[4]], {36.0f + (kSpace_ * 5.0f), 684.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	 digitNum6_=Sprite::Create(
	    numberTextureHandle[digitNumber[5]], {36.0f + (kSpace_ * 6.0f), 684.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	 digitNum7_=Sprite::Create(
	    numberTextureHandle[digitNumber[6]], {36.0f + (kSpace_ * 7.0f), 684.0f},
	    {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	 digitNum8_ = Sprite::Create(
	     numberTextureHandle[digitNumber[7]], {36.0f + (kSpace_ * 8.0f), 684.0f},
	     {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});

	viewProjection_.Initialize();

	//プレイヤー関係初期化
	playermodel_ = Model::CreateFromOBJ("player",true);
	playerBulletmodel_ = Model::CreateFromOBJ("playerBullet", true);

	player_ = new Player();

	Vector3 playerPosition(0, -2, 30.0f);
	player_->Initialize(playermodel_,playerBulletmodel_,playerPosition);
	player_->SetGameScene(this);

	//敵関係初期化
	enemy1model_ = Model::CreateFromOBJ("enemy1", true);
	enemy2model_ = Model::CreateFromOBJ("enemy2", true);
	enemy3model_ = Model::CreateFromOBJ("enemy3", true);
	enemyBulletmodel_ = Model::CreateFromOBJ("enemyBullet", true);
	wireEnemy1model_ = Model::CreateFromOBJ("wireEnemy1", true);
	wireEnemy2model_ = Model::CreateFromOBJ("wireEnemy2", true);
	wireEnemyBulletmodel_ = Model::CreateFromOBJ("wireEnemyBullet", true);

	bossmodel_ = Model::CreateFromOBJ("boss", true);

	//その他初期化
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_ = new Skydome();

	skydome_->Initialize(modelSkydome_);
	skydome_->SetGameScene(this);

	railCamera_ = new RailCamera();

	railCamera_->Initialize({0.0f, 0.0f, -30.0f}, {0.0f, 0.0f, 0.0f});

	score_ = 0;


	player_->SetParent(&railCamera_->GetWorldTransform());

	//csv読み込み
	LoadEnemyPopData();

}

void GameScene::Update() {

	//ゲームの一時停止
	if (freezeTimer_ > 0) {
		audio_->StopWave(BGMvoiceHandle_);

		--freezeTimer_;
		return;
	}

	//文字演出用
	if (blink_ == 1) {
		blink_ = 2;
	} else if (blink_ == 2) {
		blink_ = 3;
	} else if (blink_ == 3) {
		blink_ = 0;
	} else {
		blink_ = 1;
	}

	//スコアを常に計算
	ScoreDisplayCalc();

	//シーン
	switch (scene_) {
	case GameScene::Scene::Title:
		if (waitTimer_ > 0) {
			--waitTimer_;
		}
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && waitTimer_ <= 0) {
			startFlag = true;
		}

		if (startFlag == true) {
			screen_->SetColor({1.0f, 1.0f, 1.0f, color_.w});
			color_.w += 0.02f;
		}
		if (color_.w >= 1.0f) {
			BGMvoiceHandle_ = audio_->PlayWave(BGMsoundDataHandle_, true);
			scene_ = Scene::GamePlay;
		}

		break;

	case GameScene::Scene::GamePlay:

		if (score_ < 30000) {
			gameRank_ = 1;
		} else if (30000 <= score_ && score_ < 70000) {
			gameRank_ = 2;
		} else if (70000 <= score_ && score_ < 100000) {
			gameRank_ = 3;
		} else if (100000 <= score_) {
			gameRank_ = 4;
		}


		digitNum1_->SetTextureHandle(numberTextureHandle[digitNumber[0]]);
		digitNum2_->SetTextureHandle(numberTextureHandle[digitNumber[1]]);
		digitNum3_->SetTextureHandle(numberTextureHandle[digitNumber[2]]);
		digitNum4_->SetTextureHandle(numberTextureHandle[digitNumber[3]]);
		digitNum5_->SetTextureHandle(numberTextureHandle[digitNumber[4]]);
		digitNum6_->SetTextureHandle(numberTextureHandle[digitNumber[5]]);
		digitNum7_->SetTextureHandle(numberTextureHandle[digitNumber[6]]);
		digitNum8_->SetTextureHandle(numberTextureHandle[digitNumber[7]]);

		if (color_.w > 0.0f && screen_->GetColor().w > 0.0f && player_->IsDead() == false) {
			color_.w -= 0.02f;
			screen_->SetColor({1.0f, 1.0f, 1.0f, color_.w});
		}



		//敵出現パターン切り替え
		enemyPopPattern_ = rand() % 4;

		//パターン読み終わる & ボス退勤まで次のパターンを読まない
		if (patternFlag_ == false && bossisAlive_ == false) {
			--enemySpwanTimer_;
		}

		//一定時間経過でパターン再読み込み
		if (enemySpwanTimer_ <= 0) {
			enemySpwanTimer_ = 180 / gameRank_;
			LoadEnemyPopData();
		}

		//ボスがいないときにザコ敵出勤
		if (bossisAlive_ == false) {
			UpdateEnemyPopCommands();
			--bossTimer_;
		}

		//ボスの出勤時間
		if (bossisAlive_ == false && bossTimer_ <= 0) {
			color_.w = 1.0f;
			screen_->SetTextureHandle(screenRHandle_);
			screen_->SetColor({1.0f, 1.0f, 1.0f, color_.w});
			AddBoss();
			bossTimer_ = 12800;
		}



		//プレイヤー更新
		player_->Update(viewProjection_);
		if (player_->IsDead() == true) {
			SEvoiceHandle_ = audio_->PlayWave(SEsoundDataHandle_, false);

			screen_->SetTextureHandle(screenKHandle_);
			color_.w += 0.02f;
			screen_->SetColor({1.0f, 1.0f, 1.0f, color_.w});
			if (color_.w >= 1.0f) {
				scene_ = Scene::Result;

			}
			
		}

		//自弾更新　ロックオン処理も兼ねる
		for (PlayerBullet* bullet : playerBullets_) {
			bullet->Update();
			for (Enemy* enemy : enemys_) {
				bullet->Rockon(enemy->GetWorldPosition(), enemy->GetisRock());
			}
			if (bossisAlive_ == true) {
				bullet->Rockon(boss_->GetWorldPosition(), boss_->GetisRock());
			}
		}
		//デスフラグの立った弾を削除
		playerBullets_.remove_if([](PlayerBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});


		//敵更新　ロックオン処理も兼ねる
		for (Enemy* enemy : enemys_) {
			enemy->Update();
			player_->Rockon(
			    enemy->GetScreenPosition(railCamera_->GetViewProjection()),
			    enemy->GetWorldPosition());
			if (player_->Rockon(
			        enemy->GetScreenPosition(railCamera_->GetViewProjection()),
			        enemy->GetWorldPosition())) {
				enemy->SetisRock(true);
			} else {
				enemy->SetisRock(false);
			}
		}
		//デスフラグの立った敵を削除
		enemys_.remove_if([](Enemy* enemy) {
			if (enemy->IsDead()) {
				
				delete enemy;
				return true;
			}
			return false;
		});

		//ボス関連処理
		//ボス出現フラグがtrueで出勤
		if (bossisAlive_ == true) {
			boss_->Update();
			player_->Rockon(
			    boss_->GetScreenPosition(railCamera_->GetViewProjection()),
			    boss_->GetWorldPosition());
			if (player_->Rockon(
			        boss_->GetScreenPosition(railCamera_->GetViewProjection()),
			        boss_->GetWorldPosition())) {
				boss_->SetisRock(true);

			} else {
				boss_->SetisRock(false);
			}
			if (boss_->IsDead()) {
				color_.w = 1.0f;
				screen_->SetTextureHandle(screenWHandle_);
				screen_->SetColor({1.0f, 1.0f, 1.0f, color_.w});
				delete boss_;
			}
		}

		//敵弾更新
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->SetPlayer(player_);
			bullet->Update();
		}
		//デスフラグの立った弾を削除
		enemyBullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});

		//ゲームシーンの判定取得関数
		CheckAllCollisions();


		break;
	case GameScene::Scene::Result:

		screen_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
		digitNum1_->SetPosition({433.0f + (kSpace_ * 1), 360.0f});
		digitNum2_->SetPosition({433.0f + (kSpace_ * 2), 360.0f});
		digitNum3_->SetPosition({433.0f + (kSpace_ * 3), 360.0f});
		digitNum4_->SetPosition({433.0f + (kSpace_ * 4), 360.0f});
		digitNum5_->SetPosition({433.0f + (kSpace_ * 5), 360.0f});
		digitNum6_->SetPosition({433.0f + (kSpace_ * 6), 360.0f});
		digitNum7_->SetPosition({433.0f + (kSpace_ * 7), 360.0f});
		digitNum8_->SetPosition({433.0f + (kSpace_ * 8), 360.0f});

		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}


		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {

			scene_ = Scene::Initialize;
		}

		break;
	case GameScene::Scene::Initialize:

		screen_->SetColor({1.0f, 1.0f, 1.0f, 0.0f});

		digitNum1_->SetPosition({36.0f + (kSpace_ * 1), 684.0f});
		digitNum2_->SetPosition({36.0f + (kSpace_ * 2), 684.0f});
		digitNum3_->SetPosition({36.0f + (kSpace_ * 3), 684.0f});
		digitNum4_->SetPosition({36.0f + (kSpace_ * 4), 684.0f});
		digitNum5_->SetPosition({36.0f + (kSpace_ * 5), 684.0f});
		digitNum6_->SetPosition({36.0f + (kSpace_ * 6), 684.0f});
		digitNum7_->SetPosition({36.0f + (kSpace_ * 7), 684.0f});
		digitNum8_->SetPosition({36.0f + (kSpace_ * 8), 684.0f});


		//	for (Enemy* enemy : enemys_) {
		//	delete enemy;
		//}





	//プレイヤー関係初期化
		playermodel_ = Model::CreateFromOBJ("player", true);
		playerBulletmodel_ = Model::CreateFromOBJ("playerBullet", true);

		player_ = new Player();

		Vector3 playerPosition(0, -2, 30.0f);
		player_->Initialize(playermodel_, playerBulletmodel_, playerPosition);
		player_->SetGameScene(this);

		//敵関係初期化
		enemy1model_ = Model::CreateFromOBJ("enemy1", true);
		enemy2model_ = Model::CreateFromOBJ("enemy2", true);
		enemy3model_ = Model::CreateFromOBJ("enemy3", true);
		enemyBulletmodel_ = Model::CreateFromOBJ("enemyBullet", true);
		wireEnemy1model_ = Model::CreateFromOBJ("wireEnemy1", true);
		wireEnemy2model_ = Model::CreateFromOBJ("wireEnemy2", true);
		wireEnemyBulletmodel_ = Model::CreateFromOBJ("wireEnemyBullet", true);

		bossmodel_ = Model::CreateFromOBJ("boss", true);

		//その他初期化
		debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

		AxisIndicator::GetInstance()->SetVisible(true);
		AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

		modelSkydome_ = Model::CreateFromOBJ("skydome", true);

		skydome_ = new Skydome();

		skydome_->Initialize(modelSkydome_);
		skydome_->SetGameScene(this);

		railCamera_ = new RailCamera();

		railCamera_->Initialize({0.0f, 0.0f, -30.0f}, {0.0f, 0.0f, 0.0f});

		score_ = 0;

		player_->SetParent(&railCamera_->GetWorldTransform());
		delete boss_;

		//敵発生タイマー
		enemySpwanTimer_ = 180;

		//待機フラグ
		waitFlag_ = false;

		//待機タイマー
		waitTimer_ = 60;

		//ボス出現フラグ
		bossisAlive_ = false;

		//ボス出現までのタイマー 3分です
		bossTimer_ = 12600;

		//開始フラグ
		startFlag = false;

		//ゲームの一時停止タイマー
		freezeTimer_ = 30;

		//文字演出用
		blink_ = 0;

		//スコア
		score_ = 0;


		scene_ = Scene::Title;

		////GameScene::~GameScene();
		//GameScene::GameScene();
		//GameScene::Initialize();

		break;

	}

	//常に読ませるもの

	//ステージ
	skydome_->Update();

	//debug
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}

#endif

	//デバッグカメラon
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	} else {

		viewProjection_.UpdateMatrix();
	}

	//通常カメラ
	if (isDebugCameraActive_ == false) {

		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

		viewProjection_.TransferMatrix();
	}

	
}

void GameScene::Draw() {

	//スコアを常に計算
	ScoreDisplayCalc();

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

	//シーン別描画
	switch (scene_) {
	case GameScene::Scene::Title:



		break;
	case GameScene::Scene::GamePlay:

		//プレイヤー描画
		player_->Draw(viewProjection_);

		//弾描画
		for (PlayerBullet* bullet : playerBullets_) {
			bullet->Draw(viewProjection_);
		}

		//敵描画
		for (Enemy* enemy : enemys_) {
			if (!enemy->IsDead()) {
				enemy->Draw(viewProjection_);
			}
			
		}

		//敵弾描画
		for (EnemyBullet* bullet : enemyBullets_) {
			bullet->Draw(viewProjection_);
		}

		//ボス描画
		if (bossisAlive_ == true) {
			boss_->Draw(viewProjection_);
		}

		break;
	case GameScene::Scene::Result:


		



		break;
	case GameScene::Scene::Initialize:

		break;

	}


	//ステージ
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

	//UI描画

	switch (scene_) {
	case GameScene::Scene::Title:
		if (blink_ < 2) {
			TitleName_->Draw();
			Press_->Draw();
		}
		screen_->Draw();
		break;
	case GameScene::Scene::GamePlay:
		if (blink_ < 2) {
			digitNum1_->Draw();
			digitNum2_->Draw();
			digitNum3_->Draw();
			digitNum4_->Draw();
			digitNum5_->Draw();
			digitNum6_->Draw();
			digitNum7_->Draw();
			digitNum8_->Draw();
			Score_->Draw();
		}
		player_->DrawUI();
		screen_->Draw();
		break;
	case GameScene::Scene::Result:
		screen_->Draw();
		if (blink_ < 2) {

			Result_->Draw();
			digitNum1_->Draw();
			digitNum2_->Draw();
			digitNum3_->Draw();
			digitNum4_->Draw();
			digitNum5_->Draw();
			digitNum6_->Draw();
			digitNum7_->Draw();
			digitNum8_->Draw();
			Press_->Draw();
		}

		break;
	case GameScene::Scene::Initialize:
		screen_->Draw();
		break;
	default:
		break;
	}







	//screenR_->Draw();
	//screenK_->Draw();




	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() { 

	//判定対象AとBの定座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = GetPlayerBullets();

	//敵リストの取得
	const std::list<Enemy*>& enemys = GetEnemys();

	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = GetEnemyBullets();

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
	for (Enemy* enemy : enemys) {

		posA = enemy->GetWorldPosition();

		//敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			//敵弾の座標
			posB = bullet->GetWorldPosition();

			if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
			        (posB.z - posA.z) * (posB.z - posA.z) <=
			    enemy->Range * enemy->Range + bullet->Range * bullet->Range) {

				if (!bullet->IsHit()) {
					enemy->OnCollision();
					bullet->OnCollision();
				}
			};
		};
	};

#pragma endregion

#pragma region	自キャラと敵キャラの当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();

		//敵キャラの座標
	for (Enemy* enemy : enemys) {

		posB = enemy->GetWorldPosition();

		if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		        (posB.z - posA.z) * (posB.z - posA.z) <=
		    enemy->Range * enemy->Range + player_->Range * player_->Range) {

			enemy->OnCollision();
			player_->OnCollision();
		};
		
	};

	#pragma endregion

	if (bossisAlive_ == true) {
#pragma region 自弾とボスの当たり判定

		//敵キャラの座標
		posA = boss_->GetWorldPosition();

		//敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			//敵弾の座標
			posB = bullet->GetWorldPosition();

			if ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
			        (posB.z - posA.z) * (posB.z - posA.z) <=
			    boss_->Range * boss_->Range + bullet->Range * bullet->Range) {

				if (!bullet->IsHit()) {
					boss_->OnCollision();
					bullet->OnCollision();
				}
			};
		};

#pragma endregion
	}



}

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) { 
	score_ += 1;
	playerBullets_.push_back(playerBullet); }

void GameScene::AddEnemy(const Vector3&pos,const int& enemyNum) {
	Enemy* enemy= new Enemy();
	enemy->Initialize(enemy1model_, enemy2model_, enemy3model_, enemyBulletmodel_,
		wireEnemy1model_,wireEnemy2model_,wireEnemyBulletmodel_, pos, enemyNum);
	enemy->SetGameScene(this);
	enemy->SetPlayer(player_);
	enemys_.push_back(enemy);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { 
	enemyBullets_.push_back(enemyBullet); }

void GameScene::AddBoss() {
	boss_ = new Boss();
	boss_->Initialize(bossmodel_, enemyBulletmodel_, enemy3model_, wireEnemyBulletmodel_, {0.0f, -20.0f, -50.0f});
	boss_->SetGameScene(this);
	boss_->SetPlayer(player_);
	boss_->SetBit1Parent(&boss_->GetWorldTransform());
	boss_->SetBit2Parent(&boss_->GetWorldTransform());
	boss_->SetBit3Parent(&boss_->GetWorldTransform());
	boss_->SetBit4Parent(&boss_->GetWorldTransform());
	bossisAlive_ = true;
}

void GameScene::LoadEnemyPopData() {

	enemyPopCommands_.str("");
	enemyPopCommands_.clear(enemyPopCommands_.goodbit);

	//ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands_ << file.rdbuf();

	//ファイルを閉じる
	file.close();

}

void GameScene::UpdateEnemyPopCommands() {

	//待機処理
	if (waitFlag_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			//待機終了
			waitFlag_ = false;
		}
		return;
	}

	//一行分の文字列を入れる変数
	std::string line;

	if (patternFlag_ == false) {

		//指定されたパターン番号まで飛ばす
		while (getline(enemyPopCommands_, line)) {
			//一行分の文字列をストリームに変換して解析しやすくなる
			std::istringstream line_stream(line);

			std::string word;
			//,区切りで先頭の文字列を取得
			getline(line_stream, word, ',');
			if (word.find(std::to_string(enemyPopPattern_)) == 0) {
				patternFlag_ = true;
				break;
			}
		}
	}


	//コマンド実行ループ
	while (getline(enemyPopCommands_, line)) {
	//一行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで先頭の文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0){
			continue;
		}

						// POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵番号
			getline(line_stream, word, ',');
			int enemyNum = (int)std::atoi(word.c_str());

			//敵発生
			AddEnemy(Vector3(x, y, z), enemyNum);
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			waitFlag_ = true;
			waitTimer_ = waitTime;

			break;

		} else if (word.find("END") == 0) {
			patternFlag_ = false;
			enemyPopCommands_.str("");
			break;
		}


	}



}

void GameScene::ScoreDisplayCalc() {

	digitNumber[0] = tmpScore / 10000000;
	tmpScore = score_ % 10000000;
	digitNumber[1] = tmpScore / 1000000;
	tmpScore = score_ % 1000000;
	digitNumber[2] = tmpScore / 100000;
	tmpScore = score_ % 100000;
	digitNumber[3] = tmpScore / 10000;
	tmpScore = score_ % 10000;
	digitNumber[4] = tmpScore / 1000;
	tmpScore = score_ % 1000;
	digitNumber[5] = tmpScore / 100;
	tmpScore = score_ % 100;
	digitNumber[6] = tmpScore / 10;
	tmpScore = score_ % 10;
	digitNumber[7] = tmpScore / 1;
	tmpScore = score_ % 1;






	//digitNum1_.SetTextureHandle(numberTextureHandle[digitNumber1_]);
	////digitNum1_.SetPosition({36.0f, 20.0f});
	//digitNum1_.GetColor();
	//digitNum1_.GetAnchorPoint();




}
