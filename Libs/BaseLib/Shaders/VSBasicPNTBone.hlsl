

#include "BasicBoneParameters.hlsli"

#include "Structures.hlsli"
#include "Common.hlsli"
#include "Lighting.hlsli"


void Skin(inout VSInputNmTxWeights vin, uniform int boneCount)
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


// Vertex shader: vertex lighting, four bones.
VSOutputTx main(VSInputNmTxWeights vin)
{
	VSOutputTx vout;

	Skin(vin, 4);

	CommonVSOutput cout = ComputeCommonVSOutputWithLighting(vin.Position, vin.Normal, activeflags.z);
	SetCommonVSOutputParams;

	vout.TexCoord = vin.TexCoord;

	return vout;
}



