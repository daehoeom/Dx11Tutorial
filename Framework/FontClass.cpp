#include "FontClass.h"



FontClass::FontClass()
	: m_Font(nullptr)
	, m_Texture(nullptr)
{
}

FontClass::FontClass(const FontClass& arg)
{
}

FontClass::~FontClass()
{
}

bool FontClass::Init(ID3D11Device* _device, char* _fontFilename, WCHAR* _textureFilename)
{
	if (!LoadFontData(_fontFilename))
	{
		return false;
	}

	if (!LoadTexture(_device, _textureFilename))
	{
		return false;
	}

	return true;
}

void FontClass::Destroy()
{
	ReleaseTexture();

	ReleaseFontData();

	return;
}

bool FontClass::LoadFontData(char* _filename)
{
	ifstream fin;
	char temp;

	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	fin.open(_filename);

	if (fin.fail())
	{
		return false;
	}

	for (int i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	safe_delete_array(m_Font);

	return;
}

bool FontClass::LoadTexture(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, WCHAR* _filename)
{
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	if (!m_Texture->Init(_device, _deviceContext, _filename))
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	safe_delete_destroy(m_Texture);

	return;
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void FontClass::BuildVertexArray(void* _vertices, char* _sentence, float _drawX, float _drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, letter;

	vertexPtr = (VertexType*)_vertices;

	numLetters = (int)strlen(_sentence);

	index = 0;

	for (int i = 0; i < numLetters; i++)
	{
		letter = ((int)_sentence[i]) - 32;

		if (letter == 0)
		{
			_drawX = _drawX + 3.0f;
		}
		else
		{
			vertexPtr[index].position = XMFLOAT3(_drawX, _drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((_drawX + m_Font[letter].size), (_drawY - 16), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(_drawX, (_drawY - 16), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(_drawX, _drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(_drawX + m_Font[letter].size, _drawY, 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((_drawX + m_Font[letter].size), (_drawY - 16), 0.0f);
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			_drawX = _drawX + m_Font[letter].size + 1.0f;
		}
	}

	return;
}

