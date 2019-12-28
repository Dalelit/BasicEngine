#include "BEModelLoaderSTL.h"
#include <fstream>
#include <sstream>
#include <vector>

using namespace DirectX;

//solid xxxxxxxxxx
//facet normal - 0.671345 - 0.197092 - 0.714459
//outer loop
//vertex - 0.468750 0.242188 - 0.757812
//vertex - 0.437500 0.164062 - 0.765625
//vertex - 0.500000 0.093750 - 0.687500
//endloop
//endfacet
//endsolid xxxxxxxxxxx

BEMesh* BEModelLoaderSTL::LoadSTL(std::wstring filename)
{
	std::wifstream srcFile(filename);
	std::wstring token;
	std::vector<XMFLOAT3> verticies;
	std::vector<XMFLOAT3> normals;
	bool done = false;

	if (!srcFile.is_open()) return nullptr;

	srcFile >> token;

	if (token == L"solid")
	{
		std::getline(srcFile, token);
		// To do : create actual models, and use this info in it
	}

	// loop over facet
	while (!done && srcFile.good())
	{
		srcFile >> token;
				
		if (token == L"facet")
		{
			srcFile >> token;
			if (token == L"normal")
			{
				XMFLOAT3 normal;
				srcFile >> normal.x;
				srcFile >> normal.y;
				srcFile >> normal.z;
				// change direction of the normal
				//normal.x = -normal.x;
				//normal.y = -normal.y;
				normal.z = -normal.z;
				normals.push_back(normal);
			}
			else throw "Expected normal";

			srcFile >> token;
			if (token == L"outer")
			{
				std::getline(srcFile, token);
				// to do : check that the rest of the line says loop?

				srcFile >> token;
				while (token == L"vertex")
				{
					XMFLOAT3 vertex;
					srcFile >> vertex.x;
					srcFile >> vertex.y;
					srcFile >> vertex.z;
					vertex.z = -vertex.z;
					verticies.push_back(vertex);
					srcFile >> token;
				}

				// to do: hack to swap the order
				//int lastindx = verticies.size() - 1;
				//XMFLOAT3 tmp = verticies[lastindx - 2];
				//verticies[lastindx - 2] = verticies[lastindx];
				//verticies[lastindx] = tmp;

				// expect token == L"endloop"
				// to do : check there were 3?

			}
			else throw "Expected outer";

			srcFile >> token;
			// expect token == L"endfacet"
		}
		else if (token == L"endsolid")
		{
			std::getline(srcFile, token);
			// To do : create actual models, and use this info in it
			// To do : check that things are correct?
			done = true;
		}
		else throw "Something unexpected in STL load";
	}

	BEMesh* pMesh = new BEMesh(verticies, normals, BEMesh::BEMeshTopology::TRIANGLE_LIST);
	
	pMesh->CalculateTriangleInfo();

	return pMesh;
}
