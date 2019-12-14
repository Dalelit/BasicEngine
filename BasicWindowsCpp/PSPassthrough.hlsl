Texture2D tex;
SamplerState smplr;

float4 main(float2 tc : Texcoord) : SV_TARGET
{
	return float4(tex.Sample(smplr, tc).rgb, 1.0f);
}