#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// 自キャラの弾
/// </summary>
class EnemyBullet {
public:
	void Initialize(Model* model, Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	Vector3 GetWorldPosition();

	const float Range = 1.0f;


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
};