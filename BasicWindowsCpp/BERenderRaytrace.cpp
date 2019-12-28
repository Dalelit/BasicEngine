#include "BERenderPipeline.h"
#include <DirectXCollision.h>

using namespace DirectX;

// Taken the code from DirectXCollision.inl for TriangleTests::Intersects
// then added the barycentric cooridinates of the intersection to the signature and returned them.
// To Do: maybe actually understand this!
inline bool BERenderPipelineRaytrace::TriangleIntersects(FXMVECTOR Origin, FXMVECTOR Direction, FXMVECTOR V0, GXMVECTOR V1, HXMVECTOR V2, float& Dist, float& _u, float& _v)
{
	assert(DirectX::Internal::XMVector3IsUnit(Direction));

	XMVECTOR Zero = XMVectorZero();

	XMVECTOR e1 = XMVectorSubtract(V1, V0);
	XMVECTOR e2 = XMVectorSubtract(V2, V0);

	// p = Direction ^ e2;
	XMVECTOR p = XMVector3Cross(Direction, e2);

	// det = e1 * p;
	XMVECTOR det = XMVector3Dot(e1, p);

	XMVECTOR u, v, t;

	if (XMVector3GreaterOrEqual(det, g_RayEpsilon))
	{
		// Determinate is positive (front side of the triangle).
		XMVECTOR s = XMVectorSubtract(Origin, V0);

		// u = s * p;
		u = XMVector3Dot(s, p);

		XMVECTOR NoIntersection = XMVectorLess(u, Zero);
		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(u, det));

		// q = s ^ e1;
		XMVECTOR q = XMVector3Cross(s, e1);

		// v = Direction * q;
		v = XMVector3Dot(Direction, q);

		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(v, Zero));
		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(XMVectorAdd(u, v), det));

		// t = e2 * q;
		t = XMVector3Dot(e2, q);

		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(t, Zero));

		if (XMVector4EqualInt(NoIntersection, XMVectorTrueInt()))
		{
			Dist = 0.f;
			return false;
		}
	}
	else if (XMVector3LessOrEqual(det, g_RayNegEpsilon))
	{
		// Determinate is negative (back side of the triangle).
		XMVECTOR s = XMVectorSubtract(Origin, V0);

		// u = s * p;
		u = XMVector3Dot(s, p);

		XMVECTOR NoIntersection = XMVectorGreater(u, Zero);
		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(u, det));

		// q = s ^ e1;
		XMVECTOR q = XMVector3Cross(s, e1);

		// v = Direction * q;
		v = XMVector3Dot(Direction, q);

		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(v, Zero));
		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorLess(XMVectorAdd(u, v), det));

		// t = e2 * q;
		t = XMVector3Dot(e2, q);

		NoIntersection = XMVectorOrInt(NoIntersection, XMVectorGreater(t, Zero));

		if (XMVector4EqualInt(NoIntersection, XMVectorTrueInt()))
		{
			Dist = 0.f;
			return false;
		}
	}
	else
	{
		// Parallel ray.
		Dist = 0.f;
		return false;
	}

	t = XMVectorDivide(t, det);

	// (u / det) and (v / dev) are the barycentric cooridinates of the intersection.
	u /= det;
	v /= det;

	// Store the x-component to *pDist
	XMStoreFloat(&Dist, t);
	XMStoreFloat(&_u, u);
	XMStoreFloat(&_v, v);

	return true;
}




BERenderPipelineRaytrace::BERenderPipelineRaytrace(BEScene* _pScene, BECamera* _pCamera, BECanvas* _pCanvas)
{
	pScene = _pScene;
	pCamera = _pCamera;
	pCanvas = _pCanvas;
	stride = pCanvas->width;
	invWidthx2 = 2.0f / (float)pCanvas->width;
	invHeightx2 = 2.0f / (float)pCanvas->height;
	raysToProcess = pCanvas->width * pCanvas->height;
}

BERenderPipelineRaytrace::~BERenderPipelineRaytrace()
{
}

void BERenderPipelineRaytrace::InnerLoop(unsigned int x, unsigned int y)
{
	float px = (float)x * invWidthx2 - 1.0f;
	float py = (float)y * invHeightx2 - 1.0f;

	BECamera::Ray r = pCamera->RelativeScreenPositionToRay(px, py);
	float hitDistance = pCamera->maxDistance; // to do: what distance is the max starting?
	float distance;

	for (unsigned int eindx = 0; eindx < pScene->entityCount; eindx++) // for each entity
	{
		BEMesh* m = pScene->entities[eindx]->mesh; // get it's mesh

		if (m && m->bounds.Intersects(r.position, r.direction)) // if it has a mesh, and we the ray intersects the bounds
		{
			for (unsigned int i = 0; i < m->triCount; i++) // look at each triangle
			{
				//XMVECTOR normal = m->triangles[i].normal;
				XMVECTOR normal = m->verticies[m->triangles[i].indx[0]].normal; // to do: only using the first normal for now
				XMVECTOR v0 = m->verticies[m->triangles[i].indx[0]].position;

				if (!backfaceCull || pCamera->IsVisible(v0, normal))
				{
					XMVECTOR v1 = m->verticies[m->triangles[i].indx[1]].position;
					XMVECTOR v2 = m->verticies[m->triangles[i].indx[2]].position;

					float u = 0.0f;
					float v = 0.0f;

					// TriangleTests::Intersects(r.position, r.direction, v0, v1, v2, distold) // replaced with tweaked version

					if (TriangleIntersects(r.position, r.direction, v0, v1, v2, distance, u, v))
					{
						if (distance < hitDistance)
						{
							XMVECTOR color;

							if (m->IsTextured())
							{
								// texture sampler
								XMFLOAT2 texcoord = BEXMFloat2BaryCentric(
									m->verticies[m->triangles[i].indx[0]].texcoord,
									m->verticies[m->triangles[i].indx[1]].texcoord,
									m->verticies[m->triangles[i].indx[2]].texcoord,
									u, v);
								color = m->pTextureSampler->SampleClosest(texcoord);
							}
							else
							{
								// color
								color = XMVectorBaryCentric(m->verticies[m->triangles[i].indx[0]].color,
									m->verticies[m->triangles[i].indx[1]].color,
									m->verticies[m->triangles[i].indx[2]].color,
									u, v);
							}

							XMVECTOR ambient = color;
							XMVECTOR lights = { 0,0,0,1 };

							for (unsigned int lindx = 0; lindx < pScene->lightCount; lindx++)
							{
								lights += pScene->lights[lindx]->CalculateColor(normal);
							}

							lights = lights / (float)pScene->lightCount;

							XMVECTOR c = XMVectorSaturate(0.5f * ambient + 0.5f * lights);

							pCanvas->buffer[y * stride + x] = c;
							hitDistance = distance;
						}
					}
				}
			}
		}
	}
	//if (hitDistance == pCamera->maxDistance) pCanvas->buffer[y * stride + x] = { 1,0,0,1 };
}

void BERenderPipelineRaytrace::Draw()
{
	//DrawByLine();
	DrawBySampling(0, pCanvas->width, 0, pCanvas->height);
}

void BERenderPipelineRaytrace::Draw(unsigned int xFrom, unsigned int width, unsigned int yFrom, unsigned int height)
{
	DrawBySampling(xFrom, width, yFrom, height);
}

void BERenderPipelineRaytrace::DrawByLine()
{
	for (unsigned int y = 0; y < pCanvas->height; y++)
	{
		for (unsigned int x = 0; x < pCanvas->width; x++)
		{
			InnerLoop(x, y);

			showBuffer = true; // every full line show the buffer

			raysProcessed++;

			if (exitLoop || restartLoop) return;
		}
	}
}

// to do: tidy up types... check for efficiency
void BERenderPipelineRaytrace::DrawBySampling(unsigned int xFrom, unsigned int width, unsigned int yFrom, unsigned int height)
{
	int size = width * height;
	unsigned int x, y;

	int indx = 0;
	int offset = 13;
	int startingOffset = offset - 1;

	int counter = 0;

	while (startingOffset >= 0)
	{
		int indx = startingOffset;

		while (indx < size)
		{
			x = indx % width;
			y = indx / width;
			InnerLoop(xFrom + x, yFrom + y);
			counter++;

			if (counter % 10 == 0)
			{
				showBuffer = true;
				if (exitLoop || restartLoop) return;
			}

			indx += offset;
			raysProcessed++; // to do: not thread safe?
		}

		startingOffset--;
	}
}
