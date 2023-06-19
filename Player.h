#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
#include "Sprite.h"
#include "ViewProjection.h"

class Enemy;

class GameScene;

/// <summary>
/// 自キャラ
/// </summary>
class Player {
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Player();

		void Initialize(Model* model, Model* bulletmodel, Vector3& position);
	    void Update(const ViewProjection& viewProjection);
	    void Draw(ViewProjection& viewProjection);

		void Rotate();

		void Attack();

		bool Rockon(const Vector3& enemypos_, const Vector3& enemyWorldPos_);

		void WorldTo3DReticle();
	    void ReticleTo2DReticle(const ViewProjection& viewProjection);
	    void CursorTo3DReticle(const ViewProjection& viewProjection, Vector2 spritePosition);

		Vector3 GetWorldPosition();
	    Vector3 GetRotation() { return worldTransform_.rotation_; }
	    Vector3 GetMove() { return move_; }

		bool IsDead() const { return isDead_; }

		//衝突を検出したら呼び出されるコールバック関数
	    void OnCollision();

		void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	    void IncreaseZ(float v) { worldTransform_.translation_.z += v; }


		//弾リストを取得
		//const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

		//半径
	    const float Range = 1.0f;

		void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };

		void DrawUI();



	private:
	    WorldTransform worldTransform_;
	    Model* model_ = nullptr;
	    Model* playerBulletmodel_ = nullptr;
	    uint32_t textureHandle_ = 0u;

		Input* input_ = nullptr;

	    //ゲームパッドの状態を得る変数
	    XINPUT_STATE joyState;


	    //std::list<PlayerBullet*> bullets_;
	    Vector3 move_ = {0, 0, 0};
	    Vector3 scale_ = {0, 0, 0};

	    uint32_t bulletTimer_;

		bool isHit_ = false;
		bool isDead_ = false;






		WorldTransform worldTransform3DReticle_;

		Sprite* sprite2DReticle_ = nullptr;

		Enemy* enemy_ = nullptr;

		GameScene* gameScene_ = nullptr;

};