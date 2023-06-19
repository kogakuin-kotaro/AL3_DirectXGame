#include "RailCamera.h"
#include "MathUtility.h"
#include "ImGuiManager.h"


void RailCamera::Initialize(Vector3 position, Vector3 rotation) {

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	viewProjection_.Initialize();

}

void RailCamera::Update() {

	Vector3 velocity_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);



	--rotTimer_;
	if (rotTimer_ <= 0) {
		rotTimer_ = kRotTimerMax_;
		kRotSpeed_ = (rand() % 21 - static_cast<float>(10)) / 100000;
	}
	if (kRotParam_ <= kRotLimit_ && kRotParam_ >= -kRotLimit_) {
		kRotParam_ += kRotSpeed_;
	} else if (kRotParam_ > kRotLimit_) {
		kRotParam_ = kRotLimit_;
	} else if (kRotParam_ < -kRotLimit_) {
		kRotParam_ = -kRotLimit_;
	}

	worldTransform_.rotation_.z += kRotParam_;

	worldTransform_.UpdateMatrix(); 

	viewProjection_.matView = Inverce(worldTransform_.matWorld_);



}

