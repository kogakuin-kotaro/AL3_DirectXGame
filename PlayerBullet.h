#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Enemy;

/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet{
public:

	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Rockon(Vector3 targetPos, bool rockonFlag);
	void Draw(ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };
	bool IsHit() { return isHit_; }

	Vector3 GetWorldPosition();

	const float Range = 1.0f;

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 scale_ = {0, 0, 0};
	Vector3 velocity_;
	Vector3 targetPos_;

	static const int32_t kLifeTime = 30;

	int32_t deathTimer_ = kLifeTime;
	bool isHit_ = false;
	bool isDead_ = false;

	Enemy* enemy_ = nullptr;
};