cbuffer constObj {
	matrix transformation;
};

//struct VSOut {
//	float4 color : Color;
//	float4 position : SV_POSITION;
//};

float4 main(float3 pos : Position) : SV_POSITION
{
	//VSOut result;

	//result.position = mul(pos, transformation);
	//result.color = float4(1.0,0.0,1.0,1.0);

	//return result;

	return mul(float4(pos, 1.0f), transformation);
}
