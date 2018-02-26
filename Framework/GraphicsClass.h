#pragma once

#include <Windows.h>
#include "D3DClass.h"
#include "ColorShaderClass.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"
#include "LightClass.h"
#include "LightShaderClass.h"
#include "MayaModel.h"
#include "BitmapClass.h"

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
	bool Render(float);

private:
	D3DClass* m_Direct3D;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	MayaModel* m_Maya;
	BitmapClass* m_Bitmap;
	char m_fileName[256];
	TextureShaderClass* m_TextureShader;
	//ColorShaderClass* m_ColorShader;
};

