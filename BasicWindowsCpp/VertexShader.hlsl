cbuffer constObj {
	matrix transformation;
};

struct VSOut {
	float4 color : Color;
	float4 position : SV_POSITION;
};

VSOut main(float4 pos : Position, float4 col: Color)
{
	VSOut result;

	result.position = mul(pos, transformation);
	result.color = col;

	return result;

}
