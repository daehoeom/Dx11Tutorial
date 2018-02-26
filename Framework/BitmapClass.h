#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Define.h"
#include "TextureClass.h"
using namespace DirectX;


class BitmapClass
{
	typedef struct
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	}VertexType;

public:
	BitmapClass();
	BitmapClass(const BitmapClass&);
	~BitmapClass();

public:
	bool Init(ID3D11Device*, int, int, WCHAR*, int, int);
	void Destroy();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitBuffers(ID3D11Device*);
	void DestroyBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	TextureClass* m_Texture;
	int m_vertexCount;
	int m_indexCount;
	int m_screenWidth;
	int m_screenHeight;
	int m_bitmapWidth;
	int m_bitmapHeight;
	int m_previousPosX;
	int m_previousPosY;
};

