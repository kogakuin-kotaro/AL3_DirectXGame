#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cmath>

class Player;

/// <summary>
/// 自キャラの弾
/// </summary>
class EnemyBullet {
public:
	void Initialize(
	    Model* model, Model* wiremodel, Model* BBmodel, Vector3& position, const Vector3& velocity,
	    int enemyNum, int patternNum);
	void Update();
	void Draw(ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition();

	float Range = 1.0f;


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	Model* wiremodel_ = nullptr;
	Model* bossBulletmodel_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

	const float kBulletSpeed_ = 1.0f;
	static const int32_t kLifeTime = 60 * 2;

	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

	int enemyNum_;
	int patternNum_ = 0;

	Player* player_;
};