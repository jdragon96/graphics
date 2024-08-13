#include "libEngine/shared/CameraBuffer.h"

namespace libEngine
{
CameraOption::CameraOption()
{
  sensitivity = 0.01f;
  fovy        = 45.f;
  aspect      = 1280.f / 800.f;
  zNear       = 0.01f;
  zFar        = 100.f;
  cam         = Vec3(4, 0, 0);
  target      = Vec3(0, 0, 0);
  up          = Vec3(0, 0, 1);
}
void CameraOption::SetPerspective(float fovy, float aspect, float zNear, float zFar)
{
  this->fovy   = fovy;
  this->aspect = aspect;
  this->zNear  = zNear;
  this->zFar   = zFar;
}
void CameraOption::SetView(Vec3 cam, Vec3 target, Vec3 up)
{
  this->cam    = cam;
  this->target = target;
  this->up     = up;
}

CameraBuffer::CameraBuffer() : m_sensitivity(0.5f)
{
  SetPersepctive(45.f, 1.f, 0.01, 10.f);
  SetView(m_camPosition, m_targetPosition, m_upVector);
}

CameraBuffer::~CameraBuffer()
{
}

void CameraBuffer::SetOption(CameraOption opt)
{
  m_option = opt;
  SetPersepctive(m_option.fovy, m_option.aspect, m_option.zNear, m_option.zFar);
  SetView(m_option.cam, m_option.target, m_option.up);
}

void CameraBuffer::Initialize()
{
  m_frontVector = (m_targetPosition - m_camPosition).normalized();
  m_upVector    = Eigen::Vector3f(0.f, 0.f, 1.f);
  m_rightVector = m_frontVector.cross(m_upVector);
  m_distance    = sqrt((m_targetPosition - m_camPosition).colwise().squaredNorm().sum());
}

void CameraBuffer::SetPersepctive(float fovy, float aspect, float zNear, float zFar)
{
  m_fovy       = fovy / 180.f * 3.141592f;
  m_zNear      = zNear;
  m_zFar       = zFar;
  m_projMatrix = EigenUtils::Perspective(m_fovy, aspect, zNear, zFar);
}

void CameraBuffer::SetView(Vec3 cam, Vec3 target, Vec3 up)
{
  m_camPosition    = cam;
  m_targetPosition = target;
  m_upVector       = up;
  m_viewMatrix     = EigenUtils::LookAtRH(cam, target, up);
}

void CameraBuffer::SetSensitivity(float sensitivity)
{
  m_sensitivity = sensitivity;
}

void CameraBuffer::SetRatio(float aspect)
{
  m_projMatrix = EigenUtils::Perspective(m_fovy, aspect, m_zNear, m_zFar);
}

Mat4* CameraBuffer::GetViewMatPtr()
{
  return &m_viewMatrix;
}

Mat4* CameraBuffer::GetProjMatPtr()
{
  return &m_projMatrix;
}

Vec3& CameraBuffer::GetCameraPos()
{
  return m_camPosition;
}

void CameraBuffer::UpdateOrbit(float deltaX, float deltaY)
{
  m_deltaLon    = -(deltaX * m_sensitivity);
  m_deltaLat    = (deltaY * m_sensitivity);
  m_frontVector = (m_frontVector * cos(m_deltaLon) - m_rightVector * sin(m_deltaLon)).normalized();
  m_rightVector = m_frontVector.cross(m_upVector).normalized();
  m_frontVector = (m_frontVector * cos(m_deltaLat) + m_upVector * sin(m_deltaLat)).normalized();
  m_upVector    = m_rightVector.cross(m_frontVector).normalized();
  m_camPosition = m_targetPosition + m_frontVector * m_distance;

  m_viewMatrix = EigenUtils::LookAtRH(m_camPosition, m_targetPosition, m_upVector);
}

void CameraBuffer::UpdateZoom(float deltaZoom)
{
}
}  // namespace libEngine