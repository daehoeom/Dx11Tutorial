#pragma once

#include <Windows.h>
#include "D3DClass.h"
#include "ColorShaderClass.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"
#include "LightClass.h"
#include "LightShaderClass.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

public:
	bool Init(int, int, HWND);
	void Destroy();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_Direct3D;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	//TextureShaderClass* m_TextureShader;
	//ColorShaderClass* m_ColorShader;
};

