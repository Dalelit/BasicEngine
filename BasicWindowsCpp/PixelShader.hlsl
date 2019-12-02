cbuffer constObj {
	float4 lightDirection;
	float4 lightColor;
};

float4 main(float4 col : Color, float4 nor : normal) : SV_TARGET
{
	float4 amb = col;

	float4 light = saturate(-dot(nor, lightDirection) * lightColor);


	return saturate( (amb + light) * 0.5f );
}