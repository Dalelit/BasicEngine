cbuffer sceneConstObj {
	matrix cameraTransform;
};

cbuffer entityConstObj {
	matrix entityTransform;
	matrix entityTransformRotation;
};

struct VSOut {
	float4 color : Color;
	float3 normal : Normal;
	float2 tc : Texcoord;
	float4 position : SV_POSITION;
};

VSOut main(float4 pos : Position, float4 nor : Normal, float4 col : Color, float2 texcoord : Texcoord)
{
	VSOut result;

	// to do: merge the camera and model matricies into a single matrix transform
	result.position = mul(mul(pos, entityTransform), cameraTransform);
	result.color = col;
	result.normal = normalize(mul(nor, entityTransformRotation).xyz);
	result.tc = texcoord;

	return result;

}
