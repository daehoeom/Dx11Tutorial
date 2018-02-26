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

void LightClass::SetAmbientColor(float _red, float _green, float _blue, float _alpha)
{
	m_ambientColor = XMFLOAT4(_red, _green, _blue, _alpha);

	return;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
	return m_specularColor;
}

void LightClass::SetSpecularColor(float _red, float _green, float _blue, float _alpha)
{
	m_specularColor = XMFLOAT4(_red, _green, _blue, _alpha);

	return;
}

float LightClass::GetSpecularPower()
{
	return m_specularPower;
}

void LightClass::SetSpecularPower(float _power)
{
	m_specularPower = _power;

	return;
}