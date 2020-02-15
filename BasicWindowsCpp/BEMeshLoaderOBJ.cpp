#include "pch.h"
#include "BEMeshLoaderOBJ.h"

// To Do:
//  proper error handling
//  handling more than one mesh and material at a time

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


std::vector<BEMesh*> BEMeshLoaderOBJ::LoadOBJ(std::wstring filename)
{
	std::vector<BEMesh*> meshes;

	std::wifstream srcFile(filename);
	std::wstring token;
	std::wstring name;
	bool activeMesh = false;
	std::vector<XMFLOAT3> verticies;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;
	std::vector<BEVertex> meshVerticies;
	std::map<std::wstring, BEMaterial> materials;
	std::wstring currentMat;

	if (!srcFile.is_open()) return meshes;

	srcFile >> token;
	while (srcFile.good())
	{
		if (token == L"#")
		{
			std::getline(srcFile, token); // do nothing for now
		}
		else if (token == L"mtllib")
		{
			srcFile >> token; // token = filename
			std::wstring matfilename = std::filesystem::path(filename).replace_filename(token);
			materials = LoadMTL(matfilename);
		}
		else if (token == L"o")
		{
			// create the mesh object if we've already been building it up and clear things out to start again.
			if (activeMesh)
			{
				BEMesh* pMesh = new BEMesh(meshVerticies);
				pMesh->name = name;
				pMesh->material = materials[currentMat];
				meshes.push_back(pMesh);
				// do not clear raw file data as file structure appears to continue adding
				meshVerticies.clear();
			}

			// start the mesh
			std::getline(srcFile, name);
			activeMesh = true;
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
			srcFile >> currentMat;
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

			// use the material. Note - syntax below didn't work in the lambda for some reason tbd.
			v0.color = materials[currentMat].diffuseColor;
			v1.color = materials[currentMat].diffuseColor;
			v2.color = materials[currentMat].diffuseColor;

			// change the order
			meshVerticies.push_back(v0);
			meshVerticies.push_back(v2);
			meshVerticies.push_back(v1);
		}

		srcFile >> token; // next line
	}

	if (activeMesh)
	{
		BEMesh* pMesh = new BEMesh(meshVerticies);
		pMesh->name = name;
		pMesh->material = materials[currentMat];
		meshes.push_back(pMesh);
	}

	return meshes;
}

std::map<std::wstring, BEMaterial> BEMeshLoaderOBJ::LoadMTL(std::wstring filename)
{
	std::map<std::wstring, BEMaterial> materials;

	std::wifstream srcFile(filename);
	std::wstring token;
	std::wstring currentMat;
	XMVECTOR xmv = { 1.0f, 1.0f, 1.0f, 1.0f};

	if (!srcFile.is_open()) return materials;

	srcFile >> token;
	while (srcFile.good())
	{
		if (token == L"#")
		{
			std::getline(srcFile, token); // do nothing for now
		}
		else if (token == L"newmtl")
		{
			srcFile >> currentMat;
			materials[currentMat] = BEMaterial(currentMat);
		}
		else if (token == L"Ka")
		{
			srcFile >> xmv.m128_f32[0];
			srcFile >> xmv.m128_f32[1];
			srcFile >> xmv.m128_f32[2];
			materials[currentMat].ambientColor = xmv;
		}
		else if (token == L"Kd")
		{
			srcFile >> xmv.m128_f32[0];
			srcFile >> xmv.m128_f32[1];
			srcFile >> xmv.m128_f32[2];
			materials[currentMat].diffuseColor = xmv;
		}
		else if (token == L"Ks")
		{
			srcFile >> xmv.m128_f32[0];
			srcFile >> xmv.m128_f32[1];
			srcFile >> xmv.m128_f32[2];
			materials[currentMat].specularColor = xmv;
		}
		else if (token == L"Ns")
		{
			float ns;
			srcFile >> ns;
			materials[currentMat].specularExponent = ns;
		}
		else if (token == L"Ke")
		{
			srcFile >> xmv.m128_f32[0];
			srcFile >> xmv.m128_f32[1];
			srcFile >> xmv.m128_f32[2];
			materials[currentMat].emmissionColor = xmv;
		}
		else if (token == L"Ni") // index of refraction
		{
			std::getline(srcFile, token); // do nothing for now
		}
		else if (token == L"d") // transparency
		{
			std::getline(srcFile, token); // do nothing for now
		}
		else if (token == L"illum") // illumination mode
		{
			std::getline(srcFile, token); // do nothing for now
		}

		srcFile >> token; // next line
	}
	
	return materials;
}
