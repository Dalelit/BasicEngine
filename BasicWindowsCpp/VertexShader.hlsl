cbuffer constObj {
	matrix transformation;
};

struct VSOut {
	float4 color : Color;
	float4 position : SV_POSITION;
};

VSOut main(float3 pos : Position, float4 col: Color)
{
	VSOut result;

	result.position = mul(float4(pos, 1.0), transformation);
	result.color = col;

	return result;

}
