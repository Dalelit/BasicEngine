struct VSOut
{
	float2 tc : Texcoord;
	float4 pos : SV_POSITION;
};

VSOut main(float3 pos : Position, float2 tc : Texcoord)
{
	VSOut vso;

	vso.pos = float4(pos, 1.0f);
	vso.tc = tc;

	return vso;
}