#pragma once

#include <ostream>
#include <fstream>
using namespace std;

class MayaModel
{
private:
	typedef struct
	{
		float x, y, z;
	}VertexType;

	typedef struct
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	}FaceType;

public:
	MayaModel();
	MayaModel(const MayaModel&);
	~MayaModel();

	void GetModelFilename(char*);
	bool ReadFileCounts(char*, int&, int&, int&, int&);
	bool LoadDataStructures(char*, int, int, int, int);
};

