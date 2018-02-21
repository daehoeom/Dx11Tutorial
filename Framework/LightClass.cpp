#include "LightClass.h"



LightClass::LightClass()
{
}

LightClass::LightClass(const LightClass& arg)
{
}

LightClass::~LightClass()
{
}

void LightClass::SetDiffuseColor(float _red, float _green, float _blue, float _alpha)
{
	m_diffuseColor = XMFLOAT4(_red, _green, _blue, _alpha);
	return;
}

void LightClass::SetDirection(float _x, float _y, float _z)
{
	m_direction = XMFLOAT3(_x, _y, _z);
	return;
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}

