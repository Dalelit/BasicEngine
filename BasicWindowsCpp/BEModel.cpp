#include "BEModel.h"

BEModel::BEModel()
{
}

BEModel::~BEModel()
{
	if (lCount > 0)
	{
		delete verticies;
		delete screenPoints;
		delete lines;
	}
}

void BEModel::Translate(Vector3 v)
{
	unsigned int i = 0;

	while (i < vCount)
	{
		verticies[i++] += v;
	}
}

void BEModel::CreateColorData()
{
	if (colors != NULL) return; // already created

	colors = new Color[vCount];

	for (unsigned int i = 0; i < vCount; i++) colors[i] = color;
}

///////////////////// primatives

BEModel* BEModel::TrianglePrimative()
{
	BEModel* mp = new BEModel();

	mp->vCount = 3;
	mp->lCount = 2 * 3;
	mp->verticies = new Vector3[mp->vCount];
	mp->screenPoints = new Vector3[mp->vCount];
	mp->lines = new unsigned int[mp->lCount];

	int i = 0;
	mp->verticies[i++] = { 0,0,0 };
	mp->verticies[i++] = { 1,0,0 };
	mp->verticies[i++] = { 0,1,0 };

	i = 0;
	mp->lines[i++] = 0; mp->lines[i++] = 1;
	mp->lines[i++] = 1; mp->lines[i++] = 2;
	mp->lines[i++] = 2; mp->lines[i++] = 0;

	return mp;
}

BEModel* BEModel::AxisPrimative()
{
	BEModel* mp = new BEModel();

	mp->vCount = 4;
	mp->lCount = 2 * 3;
	mp->verticies = new Vector3[mp->vCount];
	mp->screenPoints = new Vector3[mp->vCount];
	mp->colors = new Color[mp->vCount];
	mp->lines = new unsigned int[mp->lCount];

	int i = 0;
	mp->verticies[i] = { 0,0,0 };
	mp->colors[i++]  = { 1,1,1 };
	mp->verticies[i] = { 1,0,0 };
	mp->colors[i++]  = { 1,0,0 };
	mp->verticies[i] = { 0,1,0 };
	mp->colors[i++]  = { 0,1,0 };
	mp->verticies[i] = { 0,0,1 };
	mp->colors[i++]  = { 0,0,1 };

	i = 0;
	mp->lines[i++] = 0; mp->lines[i++] = 1;
	mp->lines[i++] = 0; mp->lines[i++] = 2;
	mp->lines[i++] = 0; mp->lines[i++] = 3;

	return mp;
}

BEModel* BEModel::CubePrimative()
{
	BEModel* mp = new BEModel();

	mp->vCount = 8;
	mp->lCount = 2 * 12;
	mp->verticies = new Vector3[mp->vCount];
	mp->screenPoints = new Vector3[mp->vCount];
	mp->lines = new unsigned int[mp->lCount];

	int i = 0;
	mp->verticies[i++] = { -0.5f, 0.5f, -0.5f };
	mp->verticies[i++] = { 0.5f, 0.5f, -0.5f };
	mp->verticies[i++] = { 0.5f, 0.5f,  0.5f };
	mp->verticies[i++] = { -0.5f, 0.5f,  0.5f };
	mp->verticies[i++] = { -0.5f, -0.5f, -0.5f };
	mp->verticies[i++] = { 0.5f, -0.5f, -0.5f };
	mp->verticies[i++] = { 0.5f, -0.5f,  0.5f };
	mp->verticies[i++] = { -0.5f, -0.5f,  0.5f };

	i = 0;
	mp->lines[i++] = 0; mp->lines[i++] = 1;
	mp->lines[i++] = 1; mp->lines[i++] = 2;
	mp->lines[i++] = 2; mp->lines[i++] = 3;
	mp->lines[i++] = 3; mp->lines[i++] = 0;
	mp->lines[i++] = 0; mp->lines[i++] = 4;
	mp->lines[i++] = 1; mp->lines[i++] = 5;
	mp->lines[i++] = 2; mp->lines[i++] = 6;
	mp->lines[i++] = 3; mp->lines[i++] = 7;
	mp->lines[i++] = 4; mp->lines[i++] = 5;
	mp->lines[i++] = 5; mp->lines[i++] = 6;
	mp->lines[i++] = 6; mp->lines[i++] = 7;
	mp->lines[i++] = 7; mp->lines[i++] = 4;

	return mp;
}
