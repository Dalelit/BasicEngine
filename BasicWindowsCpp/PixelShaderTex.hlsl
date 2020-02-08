#include "PixelShaderSceneConst.hlsl"

cbuffer entityConstObj {
	float4 entityColor; // placeholder value for layout... and maybe future use
};

Texture2D tex;
SamplerState smplr;

float4 main(float4 col : Color, float3 nor : Normal, float2 tc : Texcoord, float3 posWS: PositionWS) : SV_TARGET
{
	float4 surfaceColor;

	nor = normalize(nor);

	surfaceColor = float4(tex.Sample(smplr, tc).rgb, 1.0f);

	// ambient light
	float4 light = ambientColor;

	// direcitonal light
	light += max(0, -dot(nor, lightDirection.xyz)) * lightColor;

	// point light
	float3 toPL = pointLightPosition.xyz - posWS;

	float plNorDot = dot(nor, normalize(toPL));

	if (plNorDot > 0.0)
	{
		float len = length(toPL);
		float att = 1.0 + 0.045 * len + 0.0075 * len * len;

		light += (1.0 / att) * plNorDot * pointLightColor;
	}

	// point light spec
	float3 toCam = normalize(cameraPosition.xyz - posWS);

	float3 reflected = normalize(2.0 * dot(nor, toPL) * nor - toPL);

	float spec = saturate(dot(reflected, toCam));
	const float specPower = 40.0;
	const float specIntensity = 3.0;
	spec = pow(spec, specPower) * specIntensity;
	light += spec * pointLightColor;

	return saturate(light * surfaceColor);
}