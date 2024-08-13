#pragma once

#include <Eigen/dense>

namespace libEngine
{
class EigenUtils
{
public:
  static Eigen::Matrix4f LookAtRH(const Eigen::Vector3f camPos, const Eigen::Vector3f targetPos,
                                  const Eigen::Vector3f upVec)
  {
    Eigen::Vector3f X = targetPos - camPos;
    X.normalize();

    Eigen::Vector3f Y = X.cross(upVec);
    Y.normalize();

    Eigen::Vector3f Z = Y.cross(X);

    Eigen::Matrix4f result = Eigen::Matrix4f::Identity();

    result(0, 0) = Y(0);
    result(1, 0) = Y(1);
    result(2, 0) = Y(2);
    result(3, 0) = -Y.dot(camPos);
    result(0, 1) = Z(0);
    result(1, 1) = Z(1);
    result(2, 1) = Z(2);
    result(3, 1) = -Z.dot(camPos);
    result(0, 2) = -X(0);
    result(1, 2) = -X(1);
    result(2, 2) = -X(2);
    result(3, 2) = X.dot(camPos);

    return result;
  }

  template <typename T>
  static Eigen::Matrix4f Perspective(T fovy, T aspect, T zNear, T zFar)
  {
    T const tanHalfFovy = tan(fovy / static_cast<T>(2));

    Eigen::Matrix4f result = Eigen::Matrix4f::Zero();
    result(0, 0)           = static_cast<T>(1) / (aspect * tanHalfFovy);
    result(1, 1)           = static_cast<T>(1) / (tanHalfFovy);
    result(2, 2)           = -(zFar + zNear) / (zFar - zNear);
    result(2, 3)           = -static_cast<T>(1);
    result(3, 2)           = -(static_cast<T>(2) * zFar * zNear) / (zFar - zNear);

    return result;
  }

  static float Distance(Eigen::Vector3f v1, Eigen::Vector3f v2)
  {
    return 0.f;
  }
};
}  // namespace libEngine