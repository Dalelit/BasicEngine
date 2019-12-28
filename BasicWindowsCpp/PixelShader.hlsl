Texture2D tex;
SamplerState smplr;

cbuffer sceneConstObj {
	float4 lightDirection;
	float4 lightColor;
};

cbuffer entityConstObj {
	float4 entityColor;
	bool textured;
};

float4 main(float4 col : Color, float4 nor : normal, float2 tc : Texcoord) : SV_TARGET
{
	float4 amb;

	if (textured)
	{
		amb = float4(tex.Sample(smplr, tc).rgb, 1.0f);
	}
	else
	{
		amb = col;
	}

	float4 light = saturate(-dot(nor, lightDirection) * lightColor * amb);


	return saturate((amb + light) * 0.5f);
}