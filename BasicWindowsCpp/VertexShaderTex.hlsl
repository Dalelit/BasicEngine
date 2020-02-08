#include "VertexShaderSceneConst.hlsl"

cbuffer entityConstObj {
	matrix entityTransform;
	matrix entityTransformRotation;
};

struct VSOut {
	float4 color : Color;
	float3 normal : Normal;
	float2 tc : Texcoord;
	float3 posWS: PositionWS;
	float4 position : SV_POSITION;
};

VSOut main(float4 pos : Position, float4 nor : Normal, float4 col : Color, float2 texcoord : Texcoord)
{
	VSOut result;

	float4 pws = mul(pos, entityTransform); // position in world space

	result.color = col;
	result.normal = normalize(mul(nor, entityTransformRotation).xyz);
	result.tc = texcoord;
	result.posWS = pws.xyz;
	result.position = mul(pws, cameraTransform);

	return result;

}
