#include "MayaModel.h"

#ifndef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	ifstream fin;
	
	fin.open(_filename);

	if (fin.good())
	{
		strcpy_s(m_fileName, _filename);
		return;
	}
	else
	{
		cout << "File could not be opened" << endl;
	}

	return;
}

bool MayaModel::ReadFileCounts()
{
	ifstream fin;
	char input = '\0';

	m_vertexCount = m_textureCount = m_normalCount = m_faceCount = 0;

	fin.open(m_fileName);

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
			if (input == ' ') { m_vertexCount++; }
			if (input == 't') { m_textureCount++; }
			if (input == 'n') { m_normalCount++; }
		}
		
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { m_faceCount++; }
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

bool MayaModel::LoadDataStructures()
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

	// 구조체 4개의 데이터를 초기화 한다.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoord = new VertexType[m_textureCount];
	if (!texcoord)
	{
		return false;
	}

	normals = new VertexType[m_normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType[m_faceCount];
	if (!faces)
	{
		return false;
	}

	vertexIndex = texcoordIndex = normalIndex = faceIndex = 0;

	// Maya(.obj)파일 열기
	fin.open(m_fileName);

	if (fin.fail())
	{
		return false;
	}

	// 데이터 구조에서 정점, 텍스쳐 좌표 그리고 법선벡터를 읽는다.
	// 중요한 점은 마야의 경우 오른손 좌표계를 사용하기 때문에 왼손 좌표계로 변환해주어야 한다.
	fin.get(input);

	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// 정점 데이터 읽기
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				//Z 정점을 왼손 좌표계로 변환한다.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// 텍스쳐좌표 데이터 읽기
			if (input == 't')
			{
				fin >> texcoord[texcoordIndex].x >> texcoord[texcoordIndex].y;

				// 텍스쳐 좌표 V를 왼손 좌표계로 변환한다.
				texcoord[texcoordIndex].y = 1.0f - texcoord[texcoordIndex].y;
				texcoordIndex++;
			}

			// 법선좌표 데이터 읽기
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// 면 데이터 읽기
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

		// 나머지 라인을 읽는다.
		while (input != '\n')
		{
			fin.get(input);
		}

		fin.get(input);
	}

	fin.close();

	fout.open("../Model/cube.txt");

	fout << "Vertex Count : " << (m_faceCount * 3) << endl;
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

	safe_delete_array(vertices);
	safe_delete_array(texcoord);
	safe_delete_array(normals);
	safe_delete_array(faces);

	return true;
}