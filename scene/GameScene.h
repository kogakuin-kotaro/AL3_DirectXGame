#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Boss.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <sstream>


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 自弾を追加する
	/// </summary>
	/// <param name = "playerBullet">自弾</param>
	void AddPlayerBullet(PlayerBullet* playerBullet);

	/// <summary>
	/// 敵を追加する
	/// </summary>
	/// <param name = "enemy">敵</param>
	void AddEnemy(const Vector3& pos,const int& enemyNum);

/// <summary>
/// 敵弾を追加する
/// </summary>
/// <param name = "enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵を追加する
	/// </summary>
	/// <param name = "enemy">敵</param>
	void AddBoss();

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	const std::list<PlayerBullet*>& GetPlayerBullets() const { return playerBullets_; }

	const std::list<Enemy*>& GetEnemys() const { return enemys_; }

	const std::list<EnemyBullet*>& GetEnemyBullets() const { return enemyBullets_; }

	//スコア計算
	void ScoreSetter(int num) { score_ = score_ + num; }
	int ScoreGetter() { return score_; }

	void BossIsAlive(bool isAlive) { bossisAlive_ = isAlive; }
	bool GetBossIsAlive() { return bossisAlive_; }

	void FreezeTimeSetter(int num) { freezeTimer_ = num; }

	void ScoreDisplayCalc();


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;
	
	enum class Scene {
		Title,
		GamePlay,
		Result,
		Initialize,
	};

	Scene scene_ = Scene::Title;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	//sound
	uint32_t BGMsoundDataHandle_ = 0;
	uint32_t BGMvoiceHandle_ = 0;
	uint32_t SEsoundDataHandle_ = 0;
	uint32_t SEvoiceHandle_ = 0;

	//画像格納用変数群

	uint32_t screenWHandle_ = TextureManager::Load("./Resources/Images/screenW.png");
	uint32_t screenRHandle_ = TextureManager::Load("./Resources/Images/screenR.png");
	uint32_t screenKHandle_ = TextureManager::Load("./Resources/Images/screenK.png");
	Sprite* screen_ = nullptr;
	Sprite* TitleName_ = nullptr;
	Sprite* Press_ = nullptr;
	Sprite* Score_ = nullptr;
	Sprite* Result_ = nullptr;

	Vector4 color_{0, 0, 0, 0};

	uint32_t numberTextureHandle[10]{};

	Sprite* digitNum1_;
	Sprite* digitNum2_;
	Sprite* digitNum3_;
	Sprite* digitNum4_;
	Sprite* digitNum5_;
	Sprite* digitNum6_;
	Sprite* digitNum7_;
	Sprite* digitNum8_;


	//自機3Dモデルデータ
	Model* playermodel_ = nullptr;

	//自弾3Dモデルデータ
	Model* playerBulletmodel_ = nullptr;


	//敵3Dモデルデータ
	Model* enemy1model_ = nullptr;
	Model* enemy2model_ = nullptr;
	Model* enemy3model_ = nullptr;
	Model* wireEnemy1model_ = nullptr;
	Model* wireEnemy2model_ = nullptr;

	//敵弾3Dモデルデータ
	Model* enemyBulletmodel_ = nullptr;
	Model* wireEnemyBulletmodel_ = nullptr;

	//ボスモデルデータ
	Model* bossmodel_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//プレイヤー
	Player* player_ = nullptr;

	//自弾
	std::list<PlayerBullet*> playerBullets_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//敵
	std::list<Enemy*> enemys_;

	//ボス
	Boss* boss_ = nullptr;

	//敵弾
	std::list<EnemyBullet*> enemyBullets_;

	//天球
	Skydome* skydome_ = nullptr;

	//天球3Dモデル
	Model* modelSkydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//敵発生コマンド
	std::stringstream enemyPopCommands_;

	//敵発生パターン管理用乱数
	int32_t enemyPopPattern_ = 0;
	int32_t gameRank_ = 1;

	//パターンフラグ(敵出現パターンが実行中かどうか)
	bool patternFlag_ = false;

	//敵発生タイマー
	int32_t enemySpwanTimer_ = 180;

	//待機フラグ
	bool waitFlag_ = false;

	//待機タイマー
	int32_t waitTimer_;

	//ボス出現フラグ
	bool bossisAlive_ = false;

	//ボス出現までのタイマー 3分です
	int32_t bossTimer_ = 12600;

	//開始フラグ
	bool startFlag = false;

	//ゲームの一時停止タイマー
	uint32_t freezeTimer_ = 0;

	//文字演出用
	int blink_ = 0;

	//スコア表示用変数群
	int score_ = 0;

	int digitNumber[8];
	int tmpScore = 0;
	const float kSpace_ = 46.0f;



};
