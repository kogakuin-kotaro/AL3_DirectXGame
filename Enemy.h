#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Sprite.h"

#include "EnemyBullet.h"
#include <list>

class Player;

class GameScene;

class Enemy {
public:

	~Enemy();

	void Initialize(
	    Model* model1, Model* model2, Model* model3, Model* bulletmodel, 
		Model* wiremodel1, Model* wiremodel2, Model* wirebulletmodel, 
		const Vector3& position,
	    const int& enemyNum);
	void Update();
	void Draw(ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void ApproachUpdate();
	void LeaveUpdate();

	void Fire();

	static const int kFireInterval = 70;

	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player;}

	Vector3 GetWorldPosition();

	Vector3 GetScreenPosition(const ViewProjection& viewProjection);

	bool GetisRock() { return isRockon_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetisRock(bool isRock) { isRockon_ = isRock; }

	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	const float Range = 2.0f;


private:

	WorldTransform worldTransform_;
	Model* enemy1model_ = nullptr;
	Model* enemy2model_ = nullptr;
	Model* enemy3model_ = nullptr;
	Model* enemyBulletmodel_ = nullptr;
	Model* wireEnemy1model_ = nullptr;
	Model* wireEnemy2model_ = nullptr;
	Model* wireEnemyBulletmodel_ = nullptr;



	//初期速度
	Vector3 velocity_ = {0, -1.0f, 0.0f};

	//状態
	enum class Phase {
		Approach,
		Leave,
	};

	//敵番号
	int enemyNum_;

	//初期状態
	Phase phase_ = Phase::Approach;

	bool isRockon_ = false;

	//死亡判定
	bool isDead_ = false;

	//std::list<EnemyBullet*> enemyBullets_;

	//発射タイマー
	int32_t fireTimer = 0;

	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

};