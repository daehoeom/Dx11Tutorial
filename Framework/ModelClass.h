#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "TextureClass.h"
#include "Define.h"
using namespace DirectX;

class ModelClass
{
private:
	typedef struct
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	}VertexType;
	
	typedef struct
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	}TexVertexType;

	typedef struct
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	}TNVertexType;
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Destroy();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
private:
	bool InitBuffer(ID3D11Device*);
	void DestroyBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();
private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;

	TextureClass* m_Texture;
};

