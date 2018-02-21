#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <iostream>
#include "Define.h"
#include "AlignedAllocationPolicy.h"
using namespace DirectX;
using namespace std;

class LightShaderClass 
	: public AlignedAllocationPolicy<16> 
{
private:
	typedef struct
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	}MatrixBufferType;

	typedef struct
	{
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	}LightBufferType;

public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

public:
	bool Init(ID3D11Device*, HWND);
	void Destroy();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
	
private:
	bool InitShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void DestroyShader();
	void OutputShadererrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT4);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
};

