#include "pch.h"
#include "BEMeshLoaderPLY.h"

// To Do: proper error handling

using namespace DirectX;

// ply
// format xxx
// comment xxx
// element vertex ###
// property float x
// property float y
// property float z
// property float nx
// property float ny
// property float nz
// property float s
// property float t
// element face ###
// property list uchar uint vertex_indices
// end_header
// -0.000000 1.000000 -0.000000 0.000000 0.999969 0.000000 0.791667 1.000000
// 3 0 1 2

BEMesh* BEMeshLoaderPLY::LoadPLY(std::wstring filename)
{
	std::wifstream srcFile(filename);
	std::wstring token;
	std::vector<XMFLOAT3> verticies;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;
	std::vector<unsigned int> indices;
	bool headerDone = false;

	unsigned int vertexCount = 0;
	unsigned int faceCount = 0;

	if (!srcFile.is_open()) return nullptr;

	// to do: check the header is correct

	srcFile >> token;
	while (!headerDone && srcFile.good())
	{
		if (token == L"element") {
			srcFile >> token; // get type of element

			if (token == L"vertex") {
				srcFile >> vertexCount;
			}
			else if (token == L"face") {
				srcFile >> faceCount;
			}
		}
		else if (token == L"end_header") {
			headerDone = true;
		}

		if (!headerDone)
		{
			std::getline(srcFile, token); // do nothing for now
			srcFile >> token; // next line
		}
	}

	for (unsigned int i = 0; i < vertexCount; i++)
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 texc;
		srcFile >> position.x;
		srcFile >> position.y;
		srcFile >> position.z;
		srcFile >> normal.x;
		srcFile >> normal.y;
		srcFile >> normal.z;
		srcFile >> texc.x;
		srcFile >> texc.y;

		verticies.push_back(position);
		normals.push_back(normal);
		texcoords.push_back(texc);
	}

	for (unsigned int i = 0; i < faceCount; i++)
	{
		unsigned int vcount;
		unsigned int vindx;

		srcFile >> vcount;
		if (vcount != 3u) throw "Unexpected count in face line";

		srcFile >> vindx;
		indices.push_back(vindx);
		srcFile >> vindx;
		indices.push_back(vindx);
		srcFile >> vindx;
		indices.push_back(vindx);
	}
	BEMesh* pMesh = new BEMesh(verticies, normals, texcoords, indices);
	pMesh->name = BEUtil::ToString(filename);

	pMesh->SwapIndexOrder();

	return pMesh;
}
