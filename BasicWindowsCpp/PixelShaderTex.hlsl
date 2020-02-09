#include "PixelShaderSceneConst.hlsl"
#include "PixelShaderEntityConst.hlsl"

Texture2D tex;
SamplerState smplr;

float4 main(float4 col : Color, float3 nor : Normal, float2 tc : Texcoord, float3 posWS: PositionWS) : SV_TARGET
{
	float4 surfaceColor = float4(tex.Sample(smplr, tc).rgb, 1.0f);

	nor = normalize(nor);

	// ambient light
	float4 light = ambientColor * ambientSceneColor;

	// direcitonal light
	light += max(0, -dot(nor, lightDirection.xyz)) * lightDirecitonColor * surfaceColor;

	// point light
	float3 toPL = pointLightPosition.xyz - posWS;

	float plNorDot = dot(nor, normalize(toPL));

	if (plNorDot > 0.0)
	{
		float len = length(toPL);
		float att = 1.0 + 0.045 * len + 0.0075 * len * len;

		light += (1.0 / att) * plNorDot * pointLightColor * surfaceColor;
	}

	// point light spec
	float3 toCam = normalize(cameraPosition.xyz - posWS);

	float3 reflected = normalize(2.0 * dot(nor, toPL) * nor - toPL);

	float spec = saturate(dot(reflected, toCam));
	//const float specIntensity = 1.0;
	spec = pow(spec, specularExponent); // *specIntensity;
	light += spec * pointLightColor * specularColor;

	return saturate(light);
}