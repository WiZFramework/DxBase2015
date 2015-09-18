//--------------------------------------------------------------------------------------
// File: VSTexture3D.hlsl
//
//--------------------------------------------------------------------------------------

#include "PRTextureSkin3D.hlsli"


void Skin(inout VertexShaderInput vin, uniform int boneCount)
{
	float4x3 skinning = 0;

	[unroll]
	for (int i = 0; i < boneCount; i++)
	{
		skinning += Bones[vin.Indices[i]] * vin.Weights[i];
	}

	vin.Position.xyz = mul(vin.Position, skinning);
	vin.Normal = mul(vin.Normal, (float3x3)skinning);
}


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput vertexShaderOutput;

	Skin(input, 4);

	float4 pos = input.Position;
	pos.w = 1.0;
	pos = mul(pos, Model);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	vertexShaderOutput.Position = pos;
	vertexShaderOutput.Normal = mul(float4(input.Normal, 1.0f), Model).xyz;
	vertexShaderOutput.TexCoord = input.TexCoord;
	return vertexShaderOutput;
}

