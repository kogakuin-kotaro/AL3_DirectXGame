#pragma once
#include "Model.h"
#include "WorldTransform.h"

#include "EnemyBullet.h"
#include <list>



class Player;

class GameScene;

class Enemy {
public:

	~Enemy();

	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void ApproachUpdate();
	void LeaveUpdate();

	void Fire();

	static const int kFireInterval = 60;

	void ApproachInitialize();

	void SetPlayer(Player* player) { player_ = player;}

	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	const float Range = 2.0f;


private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_ = {0, 0, -0.2f};

	enum class Phase {
		Approach,
		Leave,
	};

	Phase phase_ = Phase::Approach;

	bool isDead_ = false;

	//std::list<EnemyBullet*> enemyBullets_;

	//発射タイマー
	int32_t fireTimer = 0;

	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

};