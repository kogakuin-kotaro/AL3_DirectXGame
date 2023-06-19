#pragma once
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"

#include "EnemyBullet.h"
#include <list>

class Player;

class GameScene;

class Boss {
public:
	~Boss();

	void Initialize(
	    Model* model, Model* bullet1model, Model* bullet2model, Model* wirebullet1model, const Vector3& position);
	void Update();
	void Draw(ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void SpawnUpdate();
	void BattleUpdate();
	void LeaveUpdate();

	void Fire();

	static const int kFireInterval = 70;

	void SpawnInitialize();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();
	Vector3 GetBit1WorldPosition();
	Vector3 GetBit2WorldPosition();
	Vector3 GetBit3WorldPosition();
	Vector3 GetBit4WorldPosition();
	WorldTransform& GetWorldTransform() { return worldTransform_; };


	Vector3 GetScreenPosition(const ViewProjection& viewProjection);

	bool GetisRock() { return isRockon_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetisRock(bool isRock) { isRockon_ = isRock; }

	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
	void SetBit1Parent(const WorldTransform* parent) { bit1WorldTransform_.parent_ = parent; }
	void SetBit2Parent(const WorldTransform* parent) { bit2WorldTransform_.parent_ = parent; }
	void SetBit3Parent(const WorldTransform* parent) { bit3WorldTransform_.parent_ = parent; }
	void SetBit4Parent(const WorldTransform* parent) { bit4WorldTransform_.parent_ = parent; }


	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//弾リストを取得
	// const std::list<EnemyBullet*>& GetBullets() const { return enemyBullets_; }

	const float Range = 10.0f;

private:
	WorldTransform worldTransform_;
	WorldTransform bit1WorldTransform_;
	WorldTransform bit2WorldTransform_;
	WorldTransform bit3WorldTransform_;
	WorldTransform bit4WorldTransform_;
	Model* bossmodel_ = nullptr;
	Model* enemyBullet1model_ = nullptr;
	Model* enemyBullet2model_ = nullptr;
	Model* wireEnemyBullet1model_ = nullptr;



	//速度関係
	Vector3 velocity_ = {0.0f, 0.0f, 1.0f};
	Vector3 velocityR_ = {0.0f, 0.0f, 0.0f};
	float kRotspeed_ = 0.002f;

	//状態
	enum class Phase {
		Spawn,
		Battle,
		Leave,
	};

	//敵番号
	int enemyNum_ = 4;

	//初期状態
	Phase phase_ = Phase::Spawn;

	bool isRockon_ = false;

	uint32_t HP = 1000;

	//死亡判定
	bool isDead_ = false;

	// std::list<EnemyBullet*> enemyBullets_;


	uint32_t patternNum_ = 0;
	//発射タイマー
	int32_t fireTimer = 0;

	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;
};