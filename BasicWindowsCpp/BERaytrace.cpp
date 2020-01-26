#include "BERaytrace.h"
#include <DirectXCollision.h>

using namespace DirectX;


bool BERaytrace::RayIntersects(BEScene* pScene, DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDirection, float distance)
{
	for (BEModel* model : pScene->models) // for all the models
	{
		BEMesh* m = model->pMesh; // get it's mesh

		for (BEEntity* entity : model->entities) // loop on each instance the model
		{
			// change the ray to be in the model space
			XMVECTOR rayModelSpaceOrigin = entity->WorldToModelPosition(rayOrigin);
			XMVECTOR rayModelSpaceDirection = XMVector3Normalize(entity->WorldToModelDirection(rayDirection));

			if (m->bounds.Intersects(rayModelSpaceOrigin, rayModelSpaceDirection)) // if the ray intersects the bounds
			{
				for (unsigned int i = 0; i < m->indxCount; ) // look at each triangle
				{
					if (DirectX::TriangleTests::Intersects(
							rayModelSpaceOrigin, rayModelSpaceDirection,
							m->verticies[m->indicies[i++]].position,
							m->verticies[m->indicies[i++]].position,
							m->verticies[m->indicies[i++]].position,
							distance))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool BERaytrace::RayHit(BEScene* pScene, BECamera* pCamera, DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDirection, float& distance, BERaytraceHit& hitInfo)
{
	bool result = false;

	for (BEModel* pModel : pScene->models)
	{
		BEMesh* pMesh = pModel->pMesh; // get it's mesh

		for (BEEntity* pEntity : pModel->entities) // loop on each instance of it
		{
			XMVECTOR rayModelSpaceOrigin = pEntity->WorldToModelPosition(rayOrigin);
			XMVECTOR rayModelSpaceDirection = XMVector3Normalize(pEntity->WorldToModelDirection(rayDirection));

			if (pMesh->bounds.Intersects(rayModelSpaceOrigin, rayModelSpaceDirection, distance)) // if the ray intersects the bounds
			{
				for (unsigned int i = 0; i < pMesh->indxCount; ) // look at each triangle
				{
					BEVertex* pV0 = &(pMesh->verticies[pMesh->indicies[i++]]);
					BEVertex* pV1 = &(pMesh->verticies[pMesh->indicies[i++]]);
					BEVertex* pV2 = &(pMesh->verticies[pMesh->indicies[i++]]);

					auto visibleTest = [rayModelSpaceOrigin](BEVertex* v) { return XMVectorGetX(XMVector3Dot((v->position - rayModelSpaceOrigin), v->normal)) < 0.0f; };

					if ( visibleTest(pV0) || visibleTest(pV1) || visibleTest(pV2))
					{
						float u = 0.0f;
						float v = 0.0f;
						float triangleHitDistance;

						if (BERaytrace::TriangleIntersects(
							rayModelSpaceOrigin, rayModelSpaceDirection,
							pV0->position, pV1->position, pV2->position,
							triangleHitDistance,
							u, v))
						{
							if (triangleHitDistance < distance)
							{
								result = true;

								distance = triangleHitDistance;

								hitInfo.pModel = pModel;
								hitInfo.pEntity = pEntity;
								hitInfo.pV0 = pV0;
								hitInfo.pV1 = pV1;
								hitInfo.pV2 = pV2;
								hitInfo.u = u;
								hitInfo.v = v;
							}
						}
					}
				}
			}
		}
	}

	return result;
}

// Taken the code from DirectXCollision.inl for TriangleTests::Intersects
// then added the barycentric cooridinates of the intersection to the signature and returned them.
// To Do: maybe actually understand this!
inline bool BERaytrace::TriangleIntersects(FXMVECTOR Origin, FXMVECTOR Direction, FXMVECTOR V0, GXMVECTOR V1, HXMVECTOR V2, float& Dist, float& _u, float& _v)
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
