//--------------------------------------------------------------------------------------
// File: VSSimple3D.hlsl
//
//--------------------------------------------------------------------------------------

#include "PRSimple3D.hlsli"



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput vertexShaderOutput;
	float4 pos = input.Position;
	pos.w = 1.0;
	pos = mul(pos, Model);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	vertexShaderOutput.Position = pos;
	vertexShaderOutput.Normal = mul(float4(input.Normal, 1.0f), Model).xyz;
	vertexShaderOutput.Diffuse = input.Diffuse;
	return vertexShaderOutput;
}

