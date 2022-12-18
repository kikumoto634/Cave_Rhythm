#pragma once
#include "../Vector/Vector3.h"

/// <summary>
/// �C�[�W���O�p����(���)
/// </summary>
/// <param name="frame">�t���[��</param>
/// <param name="second">���b�œ��B</param>
/// <returns></returns>
float& Time_OneWay(float& frame, const float& second);

/// <summary>
/// �C�[�W���O�p����(���[�v)
/// </summary>
/// <param name="frame">�t���[��</param>
/// <param name="second">���b�œ��B</param>
/// <returns></returns>
float& Time_Loop(float& frame, const float& second);

/// <summary>
/// �������� 2�_
/// </summary>
/// <param name="start">�J�n���W</param>
/// <param name="end">�I�����W</param>
/// <param name="frame">�t���[��</param>
/// <returns></returns>
const Vector3 Easing_Linear_Point2(const Vector3& start, const Vector3& end, const float& frame);

/// <summary>
/// �������� 3�_
/// </summary>
/// <param name="start">�J�n���W</param>
/// <param name="center">���ԍ��W</param>
/// <param name="end">�I�����W</param>
/// <param name="frame">�t���[��</param>
/// <returns></returns>
const Vector3 Easing_Linear_Point3(const Vector3& start, const Vector3& center, const Vector3& end, const float& frame);
