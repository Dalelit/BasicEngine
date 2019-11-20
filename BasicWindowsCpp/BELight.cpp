#include "BELight.h"

Color BELightDirectional::CalculateColor(Vector3 normal)
{
	Color c = normal.Dot(direction) * color;
	c.Saturate();
	return c;
}
