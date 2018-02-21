#pragma once
#include "TSingletone.h"
#include <DirectXMath.h>
using namespace DirectX;

class CameraClass : public TemplateSingleton<CameraClass>
{
public:
	CameraClass();
	~CameraClass();

public:
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);
private:
	CameraClass(const CameraClass&);

private:
	float m_posX;
	float m_posY;
	float m_posZ;
	float m_rotX;
	float m_rotY;
	float m_rotZ;
	XMMATRIX m_viewMatrix;
};

