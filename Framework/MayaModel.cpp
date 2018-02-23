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
	FaceType* faces;
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

	// ����ü 4���� �����͸� �ʱ�ȭ �Ѵ�.
	vertices = new VertexType[_vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoord = new VertexType[_textureCount];
	if (!texcoord)
	{
		return false;
	}

	normals = new VertexType[_normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[_faceCount];
	if (!faces)
	{
		return false;
	}

	vertexIndex = texcoordIndex = normalIndex = faceIndex = 0;

	// Maya(.obj)���� ����
	fin.open(_filename);

	if (fin.fail())
	{
		return false;
	}

	// ������ �������� ����, �ؽ��� ��ǥ �׸��� �������͸� �д´�.
	// �߿��� ���� ������ ��� ������ ��ǥ�踦 ����ϱ� ������ �޼� ��ǥ��� ��ȯ���־�� �Ѵ�.
	fin.get(input);

	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// ���� ������ �б�
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				//Z ������ �޼� ��ǥ��� ��ȯ�Ѵ�.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// �ؽ�����ǥ ������ �б�
			if (input == 't')
			{
				fin >> texcoord[texcoordIndex].x >> texcoord[texcoordIndex].y;

				// �ؽ��� ��ǥ V�� �޼� ��ǥ��� ��ȯ�Ѵ�.
				texcoord[texcoordIndex].y = 1.0f - texcoord[texcoordIndex].y;
				texcoordIndex++;
			}

			// ������ǥ ������ �б�
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// �� ������ �б�
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;

				faceIndex++;
			}
		}

		// ������ ������ �д´�.
		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	fout.open("model.txt");

	fout << "Vertex Count : " << (_faceCount * 3) << endl;
	fout << endl;
	fout << "Data : " << endl;
	fout << endl;

	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoord[tIndex].x << ' ' << texcoord[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoord[tIndex].x << ' ' << texcoord[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoord[tIndex].x << ' ' << texcoord[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
	}

	fout.close();

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(texcoord);
	SAFE_DELETE_ARRAY(normals);
	SAFE_DELETE_ARRAY(faces);
}