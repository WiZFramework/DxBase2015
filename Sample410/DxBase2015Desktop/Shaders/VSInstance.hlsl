
#include "InstanceInc.hlsli"


PixelShaderInput main(VertexShaderInput input)
{
	//�s�N�Z���V�F�[�_�[�ɓn���ϐ�
	PixelShaderInput vertexShaderOutput;
	//���_�̈ʒu��ϊ�
	float4 pos = float4(input.pos.xyz, 1.0f);
	//���[���h�ϊ�
	pos = mul(pos, input.mat);
	//�r���[�ϊ�
	pos = mul(pos, View);
	//�ˉe�ϊ�
	pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
	vertexShaderOutput.pos = pos;
	//�e�N�X�`����UV�l��ݒ�
	vertexShaderOutput.tex = input.tex;
	//���C�e�B���O�p�ɖ@�������[���h�ϊ����Đݒ�
	vertexShaderOutput.norm = mul(input.norm, (float3x3)input.mat);
	//�e�p�̒l���v�Z
	float4 modelPos = float4(input.pos.xyz, 1.0f);
	modelPos = mul(modelPos, input.mat);
	float4 lightSpacePos = mul(modelPos, LView);
	lightSpacePos = mul(lightSpacePos, LProjection);
	vertexShaderOutput.lightSpacePos = lightSpacePos;
	// Light ray
	float3 lRay = LPos.xyz - modelPos.xyz;
	vertexShaderOutput.lRay = lRay;
	vertexShaderOutput.View = EyePos.xyz - modelPos.xyz;

	float4x4 LightWorldViewProj = input.mat;
	LightWorldViewProj = mul(LightWorldViewProj, LView);
	LightWorldViewProj = mul(LightWorldViewProj, LProjection);
	vertexShaderOutput.posLWVP = mul(input.pos, LightWorldViewProj);

	//�s�N�Z���V�F�[�_�ɏo��
	//return�̓s�N�Z���V�F�[�_�ɓn�����Ƃ��Ӗ�����
	return vertexShaderOutput;
}
