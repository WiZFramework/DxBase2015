
#include "SimpleInc.hlsli"


PixelShaderInput main(VertexShaderInput input)
{
	//�s�N�Z���V�F�[�_�[�ɓn���ϐ�
	PixelShaderInput vertexShaderOutput;
	//���_�̈ʒu��ϊ�
	float4 pos = float4(input.pos.xyz, 1.0f);
	//Pos�𒲐����ėV��ł݂�
	if (input.id == 0 || input.id == 1 || input.id == 2 || input.id == 3){
		pos.y += Param.x;
	}
	//���[���h�ϊ�
	pos = mul(pos, World);
	//�r���[�ϊ�
	pos = mul(pos, View);
	//�ˉe�ϊ�
	pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
	vertexShaderOutput.pos = pos;
	//�e�N�X�`����UV�l��ݒ�
	vertexShaderOutput.tex = input.tex;
	//���C�e�B���O�p�ɖ@�������[���h�ϊ����Đݒ�
	vertexShaderOutput.norm = mul(input.norm, (float3x3)World);
	//�s�N�Z���V�F�[�_�ɏo��
	//return�̓s�N�Z���V�F�[�_�ɓn�����Ƃ��Ӗ�����
	return vertexShaderOutput;
}
