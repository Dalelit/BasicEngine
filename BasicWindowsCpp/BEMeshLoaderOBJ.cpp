#include "BEMeshLoaderOBJ.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

// To Do:
//  proper error handling
//  materials

using namespace DirectX;

// # comment
// mtllib XXXX.mtl
// o XXXXObjectNameXXXX
// v 0.000000 -1.000000 -1.000000
// vt 0.250000 0.490000
// vn 0.0000 -1.0000 0.0000
// usemtl None
// s off
// f 1/1/1 12/2/1 2/3/1  --> vertex/texture/normal indices


BEMesh* BEMeshLoaderOBJ::LoadOBJ(std::wstring filename)
{
	std::wifstream srcFile(filename);
	std::wstring token;
	std::wstring name;
	std::vector<XMFLOAT3> verticies;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;
	std::vector<BEVertex> meshVerticies;

	if (!srcFile.is_open()) return nullptr;

	srcFile >> token;
	while (srcFile.good())
	{
		if (token == L"#")
		{
			std::getline(srcFile, token); // do nothing for now
		}
		else if (token == L"mtllib")
		{
			std::getline(srcFile, token); // do nothing for now
		}
		else if (token == L"o")
		{
			std::getline(srcFile, name);
		}
		else if (token == L"v")
		{
			XMFLOAT3 v;
			srcFile >> v.x;
			srcFile >> v.y;
			srcFile >> v.z;
			verticies.push_back(v);
		}
		else if (token == L"vt")
		{
			XMFLOAT2 vt;
			srcFile >> vt.x;
			srcFile >> vt.y;
			texcoords.push_back(vt);
		}
		else if (token == L"vn")
		{
			XMFLOAT3 vn;
			srcFile >> vn.x;
			srcFile >> vn.y;
			srcFile >> vn.z;
			normals.push_back(vn);
		}
		else if (token == L"usemtl")
		{
			std::getline(srcFile, token); // do nothing for now
		}
		else if (token == L"s")
		{
			std::getline(srcFile, token); // do nothing for now
		}
		else if (token == L"f")
		{
			// parse function to create a vertex from the index string
			auto parseString = [verticies, texcoords, normals](std::wstring str) -> BEVertex {
				BEVertex vertex;
				std::wstringstream ss(str);
				std::wstring sindx;
				int indx;

				std::getline(ss, sindx, L'/'); // vertex
				indx = std::stoi(sindx) - 1;
				vertex.position = XMLoadFloat3(&verticies[indx]);

				std::getline(ss, sindx, L'/'); // tex coord
				indx = std::stoi(sindx) - 1;
				vertex.texcoord = texcoords[indx];

				std::getline(ss, sindx); // normal
				indx = std::stoi(sindx) - 1;
				vertex.normal = XMLoadFloat3(&normals[indx]);

				vertex.color = {1,1,1,1};

				return vertex;
			};

			// get the 3 verticies
			std::wstring vs;
			srcFile >> vs;
			BEVertex v0 = parseString(vs);
			srcFile >> vs;
			BEVertex v1 = parseString(vs);
			srcFile >> vs;
			BEVertex v2 = parseString(vs);
			// change the order
			meshVerticies.push_back(v0);
			meshVerticies.push_back(v2);
			meshVerticies.push_back(v1);
		}

		srcFile >> token; // next line
	}

	BEMesh* pMesh = new BEMesh(meshVerticies);
	pMesh->name = name;

	return pMesh;
}
