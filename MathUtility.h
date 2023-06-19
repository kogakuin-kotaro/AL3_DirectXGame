#pragma once
#include <cmath>
#include "Vector3.h"
#include "Matrix4x4.h"

Vector3 Add(const Vector3& v1, const Vector3& v2);

Vector3 Subtruct(const Vector3& v1, const Vector3& v2);

Vector3 Multiply(float scalar, const Vector3& v);

Vector3 Multiply(const Vector3& v1, const Vector3& v2);

Vector3 Normalize(const Vector3& v);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

Matrix4x4 MakeScaleMatrix(const Vector3& scale);

Matrix4x4 MakeRotateXMatrix(float radian);

Matrix4x4 MakeRotateYMatrix(float radian);

Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);

Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

Matrix4x4 MakeAffineMatrx(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

Matrix4x4 Inverce(const Matrix4x4& m1);

Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);