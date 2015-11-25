

cbuffer CustomShadowmapConstantBuffer : register(b0)
{
	float4x4 View				: packoffset(c0);
	float4x4 Projection				: packoffset(c4);
};

struct VertexShaderInput
{
	float4 pos : SV_Position;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD0;
	float4x4 mat : MATRIX;          // �C���X�^���X���Ƃɐݒ肳���s��
	uint InstanceId           : SV_InstanceID;   // �C���X�^���X�h�c
};


float4 main(VertexShaderInput input) : SV_POSITION
{
	float4 Output;
	input.pos.w = 1.0f;
	Output = mul(input.pos, input.mat);
	Output = mul(Output, View);
	Output = mul(Output, Projection);
	return Output;
}