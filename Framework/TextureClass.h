#pragma once

#include <d3d11.h>
#include <stdio.h>
#include "Define.h"

class TextureClass
{
private:
	typedef struct
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	}TargaHeader;

public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Destroy();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTarga(char*, int&, int&);

private:
	unsigned char* m_targaData;
	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
};

