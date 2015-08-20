//--------------------------------------------------------------------------------------
// File: VSBasicPNT.hlsl
//
//--------------------------------------------------------------------------------------

#include "BasicParameters.hlsli"
#include "Structures.hlsli"
#include "Common.hlsli"
#include "Lighting.hlsli"


// Vertex shader: one light + texture.
PixelShaderInputShadow main(VSInputNmTx vin)
{
	PixelShaderInputShadow vout;

	CommonVSOutput cout = ComputeCommonVSOutputWithLighting(vin.Position, vin.Normal, activeflags.z);
	SetCommonVSOutputParams;

	vout.TexCoord = vin.TexCoord;
	if (activeflags.y == 1){
		vout.posLWVP = mul(vin.Position, LightWorldViewProj);
	}
	else{
		vout.posLWVP = 0;
	}

	vout.Normal = mul(float4(vin.Normal, 1.0f), World).xyz;

	float4 modelPos = vin.Position;
	modelPos.w = 1;
	modelPos = mul(modelPos, World);
	float4 lightSpacePos = mul(modelPos, lView);
	lightSpacePos = mul(lightSpacePos, lProjection);
	vout.lightSpacePos = lightSpacePos;
	// Light ray
	float3 lRay = LPos.xyz - modelPos.xyz;
	vout.lRay = lRay;
	vout.View = EyePos.xyz - modelPos.xyz;

	return vout;
}

