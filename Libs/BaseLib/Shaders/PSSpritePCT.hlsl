

Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

/*
cbuffer Parameters : register(b0)
{
	row_major float4x4 MatrixTransform;
};
*/

float4 main(
		float4 color : COLOR0,
		float2 texCoord : TEXCOORD0
		) : SV_Target0
{
	return Texture.Sample(TextureSampler, texCoord) * color;
}

