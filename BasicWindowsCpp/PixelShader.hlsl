Texture2D tex;
SamplerState smplr;

cbuffer sceneConstObj {
	float4 ambientColor;	// background light
	float4 lightDirection;	// scene directional light - direction
	float4 lightColor;		// scene directional light - color
};

cbuffer entityConstObj {
	float4 entityColor; // placeholder value for layout... and maybe future use
	bool textured;
};

float4 main(float4 col : Color, float4 nor : normal, float2 tc : Texcoord) : SV_TARGET
{
	float4 surfaceColor;

	if (textured)
	{
		surfaceColor = float4(tex.Sample(smplr, tc).rgb, 1.0f);
	}
	else
	{
		surfaceColor = col;
	}

	float4 light = ambientColor;
	light += max(0, -dot(nor, lightDirection)) * lightColor;

	return saturate(light * surfaceColor);
}