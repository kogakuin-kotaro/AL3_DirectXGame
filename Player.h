#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Sprite.h"
#include "ViewProjection.h"

class Gamescene;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Player();

		void Initialize(Model* model, uint32_t textureHandle,Vector3& position);
	    void Update(const ViewProjection& viewProjection);
	    void Draw(ViewProjection& viewProjection);

		void Rotate();

		void Attack();

		Vector3 GetWorldPosition();

		//衝突を検出したら呼び出されるコールバック関数
	    void OnCollision();

		//弾リストを取得
		const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

		//半径
	    const float Range = 1.0f;

		void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };

		void DrawUI();

	private:
	    WorldTransform worldTransform_;
	    Model* model_ = nullptr;
	    uint32_t textureHandle_ = 0u;

		Input* input_ = nullptr;

	    std::list<PlayerBullet*> bullets_;

		WorldTransform worldTransform3DReticle_;

		Sprite* sprite2DReticle_ = nullptr;

};