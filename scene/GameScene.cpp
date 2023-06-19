#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>


GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
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

	model_ = Model::Create();

	viewProjection_.Initialize();

	player_ = new Player();



	Vector3 playerPosition(0, 0, 50.0f);
	player_->Initialize(model_, textureHandle_,playerPosition);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	enemy_ = new Enemy();
	
	enemy_->Initialize(model_);

	enemy_->SetPlayer(player_);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_ = new Skydome();

	skydome_->Initialize(modelSkydome_);

	railCamera_ = new RailCamera();

	railCamera_->Initialize({0.0f, 0.0f, -50.0f}, {0.0f, 0.0f, 0.0f});

	player_->SetParent(&railCamera_->GetWorldTransform());

	LoadEnemyPopData();
	
}

void GameScene::Update() { 

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

	for (Enemy* enemy : enemys_) {
		enemy->Update();


	}
	//デスフラグの立った敵を削除
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

		    //弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
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


	skydome_->Update();

	railCamera_->Update();
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();

	UpdateEnemyPopCommands();

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
		for (Enemy* enemy : enemys_) {
			enemy->Draw(viewProjection_);
		}
	    for (EnemyBullet* bullet : enemyBullets_) {
		    bullet->Draw(viewProjection_);
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

				enemy->OnCollision();
				bullet->OnCollision();
			};
		};
	};

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

void GameScene::AddEnemy(const Vector3&pos) {
	Enemy* enemy= new Enemy();
	enemy->Initialize(model_,pos);
	enemy->SetGameScene(this);
	enemy->SetPlayer(player_);
	enemys_.push_back(enemy);
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { 
	enemyBullets_.push_back(enemyBullet); }

void GameScene::LoadEnemyPopData() {
	//ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

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

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
	//一行分の文字列をストリームに変換して解析しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで先頭の文字列を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0){
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
		//x座標
		getline(line_stream, word, ',');
		float x = (float)std::atof(word.c_str());

		// y座標
		getline(line_stream, word, ',');
		float y = (float)std::atof(word.c_str()); 

		// z座標
		getline(line_stream, word, ',');
		float z = (float)std::atof(word.c_str());

		//敵発生
		AddEnemy(Vector3(x, y, z));
		} 
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			waitFlag_ = true;
			waitTimer_ = waitTime;

			break;

		}



	}

}
