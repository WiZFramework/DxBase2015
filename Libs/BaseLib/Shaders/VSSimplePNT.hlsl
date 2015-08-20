//--------------------------------------------------------------------------------------
// File: VSSimplePNT.hlsl
//
//--------------------------------------------------------------------------------------

#include "PRSimple.hlsli"



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelShaderInputShadow main(VertexShaderInput input)
{
	PixelShaderInputShadow vertexShaderOutput;
	float4 pos = input.Position;
	pos.w = 1.0;
	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	vertexShaderOutput.Position = pos;
	vertexShaderOutput.TexCoord = input.TexCoord;
	vertexShaderOutput.Normal = mul(float4(input.Normal, 1.0f), model).xyz;

	// Transform the vertex position into projected space from the POV of the light.
	float4 modelPos = input.Position;
	modelPos.w = 1;
	modelPos = mul(modelPos, model);
	float4 lightSpacePos = mul(modelPos, lView);
	lightSpacePos = mul(lightSpacePos, lProjection);
	vertexShaderOutput.lightSpacePos = lightSpacePos;
	// Light ray
	float3 lRay = LPos.xyz -modelPos.xyz;
	vertexShaderOutput.lRay = lRay;
	vertexShaderOutput.View = EyePos.xyz -modelPos.xyz;
	if (activeflags.y == 1){
		vertexShaderOutput.posLWVP = mul(input.Position, LightWorldViewProj);
	}
	else{
		vertexShaderOutput.posLWVP = 0;
	}
	return vertexShaderOutput;
}

