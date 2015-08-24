
#include "SimpleInc.hlsli"


PixelShaderInput main(VertexShaderInput input)
{
	//�s�N�Z���V�F�[�_�[�ɓn���ϐ�
	PixelShaderInput vertexShaderOutput;
	//���_�̈ʒu��ϊ�
	float4 pos = float4(input.pos.xyz, 1.0f);
	//���[���h�ϊ�
	pos = mul(pos, World);
	//�r���[�ϊ�
	pos = mul(pos, View);
	//�ˉe�ϊ�
	pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
	vertexShaderOutput.pos = pos;
	//���_�F��ݒ�
	vertexShaderOutput.color = input.color;
	//�s�N�Z���V�F�[�_�ɏo��
	//return�̓s�N�Z���V�F�[�_�ɓn�����Ƃ��Ӗ�����
	return vertexShaderOutput;
}
