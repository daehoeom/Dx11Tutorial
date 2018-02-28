#include "CameraClass.h"

#ifndef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CameraClass::CameraClass()
	: m_posX(0.f), 
	m_posY(0.f),
	m_posZ(0.f),
	m_rotX(0.f),
	m_rotY(0.f),
	m_rotZ(0.f)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float _x, float _y, float _z)
{
	this->m_posX = _x;
	this->m_posY = _y;
	this->m_posZ = _z;

	return;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(this->m_posX, this->m_posY, this->m_posZ);
}

void CameraClass::SetRotation(float _x, float _y, float _z)
{
	this->m_rotX = _x;
	this->m_rotY = _y;
	this->m_rotZ = _z;

	return;
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(this->m_rotX, this->m_rotY, this->m_rotZ);
}

void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	position.x = m_posX;
	position.y = m_posY;
	position.z = m_posZ;

	positionVector = XMLoadFloat3(&position);

	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	pitch = m_rotX * 0.0174532925f;
	yaw = m_rotY * 0.0174532925f;
	roll = m_rotZ * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	return;
}

void CameraClass::GetViewMatrix(XMMATRIX& _viewMatrix)
{
	_viewMatrix = m_viewMatrix;
	return;
}