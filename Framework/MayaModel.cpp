#include "MayaModel.h"



MayaModel::MayaModel()
{
}

MayaModel::MayaModel(const MayaModel& _arg)
{
}

MayaModel::~MayaModel()
{
}

void MayaModel::GetModelFilename(char* _filename)
{
	return;
}

bool MayaModel::ReadFileCounts(char* _filename, int& _vertexCount, int& _textureCount, 
	int& _normalCount, int& _faceCount)
{
	ifstream fin;
	char input = '\0';

	_vertexCount = _textureCount = _normalCount = _faceCount = 0;

	fin.open(_filename);

	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { _vertexCount++; }
			if (input == 't') { _textureCount++; }
			if (input == 'n') { _normalCount++; }
		}
		
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { _faceCount++; }
		}

		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	return true;
}

bool MayaModel::LoadDataStructures(char* _filename, int _vertexCount, int _textureCount, 
	int _normalCount, int _faceCount)
{
	VertexType* vertices;
	VertexType* texcoord;
	VertexType* normals;
	FaceType* face;
	ifstream fin;
	int vertexIndex;
	int texcoordIndex;
	int normalIndex;
	int faceIndex;
	int vIndex;
	int tIndex;
	int nIndex;
	char input, input2;
	ofstream fout;


}