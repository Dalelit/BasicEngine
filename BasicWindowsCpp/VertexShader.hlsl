cbuffer sceneConstObj {
	matrix cameraTransform;
};

cbuffer entityConstObj {
	matrix entityTransform;
	matrix entityTransformRotation;
};

struct VSOut {
	float4 color : Color;
	float4 normal : Normal;
	float2 tc : Texcoord;
	float4 position : SV_POSITION;
};

VSOut main(float4 pos : Position, float4 nor : Normal, float4 col : Color, float2 texcoord : Texcoord)
{
	VSOut result;

	result.position = mul(mul(pos, entityTransform), cameraTransform);
	result.color = col;
	result.normal = nor;
	result.tc = texcoord;

	return result;

}
