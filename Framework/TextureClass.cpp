#include "TextureClass.h"



TextureClass::TextureClass()
	: m_targaData(NULL),
	m_texture(NULL),
	m_textureView(NULL)
{
}

TextureClass::TextureClass(const TextureClass& arg)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Init(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, char* _filename)
{
	int height = 0, width = 0;
	D3D11_TEXTURE2D_DESC textureDesc;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	if (!LoadTarga(_filename, height, width))
	{
		return false;
	}

	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(_device->CreateTexture2D(&textureDesc, NULL, &m_texture)))
	{
		return false;
	}

	rowPitch = (width * 4) * sizeof(unsigned char);

	_deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	if (FAILED(_device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView)))
	{
		return false;
	}

	_deviceContext->GenerateMips(m_textureView);

	safe_delete(m_targaData);

	return true;
}

void TextureClass::Destroy()
{
	safe_release(m_textureView);
	safe_release(m_texture);
	safe_delete_array(m_targaData);

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

bool TextureClass::LoadTarga(char* _filename, int& _height, int& _width)
{
	int error, bpp, imageSize, index;
	FILE* fp;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;

	error = fopen_s(&fp, _filename, "rb");
	if (error != 0)
	{
		return false;
	}

	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, fp);

	if (count != 1)
	{
		return false;
	}

	_height = (int)targaFileHeader.height;
	_width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	if (bpp != 32)
	{
		return false;
	}

	imageSize = _width * _height * 4;

	targaImage = new unsigned char[imageSize];
	if (!targaImage)
	{
		return false;
	}

	count = (unsigned int)fread(targaImage, 1, imageSize, fp);
	if (count != imageSize)
	{
		return false;
	}

	error = fclose(fp);

	if (error != 0)
	{
		return false;
	}

	m_targaData = new unsigned char[imageSize];
	if (!m_targaData)
	{
		return false;
	}

	index = 0;

	int k = (_width * _height * 4) - (_width * 4);

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			// Targa 이미지는 뒤짚혀서 저장되기 때문에 Read할 때 거꾸로 읽어야 한다.
			m_targaData[index + 0] = targaImage[k + 2];	// Red
			m_targaData[index + 1] = targaImage[k + 1];	// Green
			m_targaData[index + 2] = targaImage[k + 0];	// Blue
			m_targaData[index + 3] = targaImage[k + 3];	// Alpha

			k += 4;
			index += 4;
		}
		k -= (_width * 8);
	}

	safe_delete_array(targaImage);

	return true;
}