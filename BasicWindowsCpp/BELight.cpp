#include "BELight.h"

XMVECTOR BELightDirectional::CalculateColor(XMVECTOR normal)
{
	return XMVectorSaturate( XMVector3Dot(normal, direction)* color );
}
