#include "BELight.h"

XMVECTOR BELightDirectional::CalculateColor(XMVECTOR normal)
{
	//XMVECTOR c = normal.Dot(direction) * color;
	//c.Saturate();
	//return c;
	return XMVectorSaturate( XMVector3Dot(normal, direction)* color );
}
