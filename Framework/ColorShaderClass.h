#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "Define.h"
using namespace DirectX;
using namespace std;

class ColorShaderClass
{
private:
	typedef struct 
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	}MatrixBufferType;

public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

public:
	bool Init(ID3D11Device*, HWND);
	void Destroy();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX&, XMMATRIX&, XMMATRIX&);

private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void DestroyShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&);
	void RenderShader(ID3D11DeviceContext*, int);
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader*	m_pixelShader;
	ID3D11InputLayout*	m_layout;
	ID3D11Buffer*		m_matrixBuffer;
};

