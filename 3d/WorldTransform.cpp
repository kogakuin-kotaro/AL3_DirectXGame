#include "WorldTransform.h"
#include "MathUtility.h"

void WorldTransform::UpdateMatrix() { 
	matWorld_ = MakeAffineMatrx(scale_, rotation_, translation_);

	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
		//translation_ = Multiply(translation_, parent_->translation_);
		//rotation_ = Subtruct(rotation_, parent_->rotation_);
	}

	TransferMatrix();
}