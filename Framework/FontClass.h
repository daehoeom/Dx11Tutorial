#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include "Define.h"
#include "TextureClass.h"
using namespace DirectX;
using namespace std;

class FontClass
{
private:
	typedef struct
	{
		float left;
		float right;
		int size;
	}FontType;

	typedef struct
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	}VertexType;

public:
	FontClass();
	FontClass(const FontClass&);
	~FontClass();

public:
	bool Init(ID3D11Device*, char*, WCHAR*);
	void Destroy();

	ID3D11ShaderResourceView* GetTexture();
	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, WCHAR*);
	void ReleaseTexture();

private:
	FontType* m_Font;
	TextureClass* m_Texture;
};

