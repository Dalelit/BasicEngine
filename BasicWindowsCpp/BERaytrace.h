#pragma once
#include "BEScene.h"
#include "BECamera.h"

struct BERaytraceHit
{
	// entity hit
	BEModel* pModel;
	BEEntity* pEntity;
	// mesh vertecies for triangle hit
	BEVertex* pV0;
	BEVertex* pV1;
	BEVertex* pV2;
	// barycentric coords
	float u;
	float v;
};

class BERaytrace
{
public:

	// Use to see if the ray hits something. No info about it.
	// Ray is in world space.
	static bool RayIntersects(BEScene* pScene, DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDirection, float distance);

	static bool RayHit(BEScene* pScene, BECamera* pCamera, DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDirection, float& distance, BERaytraceHit& hitInfo);

	static inline bool TargetOccluded(BEScene* pScene, DirectX::XMVECTOR origin, DirectX::XMVECTOR target) {
		DirectX::XMVECTOR rayDir = DirectX::XMVectorSubtract(target, origin);
		float dist = DirectX::XMVector3Length(rayDir).m128_f32[0];
		return RayIntersects(pScene, origin, rayDir, dist);
	}

	static inline bool TriangleIntersects(DirectX::FXMVECTOR Origin, DirectX::FXMVECTOR Direction, DirectX::FXMVECTOR V0, DirectX::GXMVECTOR V1, DirectX::HXMVECTOR V2, float& Dist, float& _u, float& _v);
};

