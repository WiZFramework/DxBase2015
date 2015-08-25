
#include "SimpleInc.hlsli"


float4 main(PixelShaderInput input) : SV_TARGET
{
	//���C�g�̌����𓾂�
	float3 LightDirection = -normalize(float3(LightDir.xyz));
	float3 InputNormal = normalize(input.norm);
	//���C�g�ɂ��s�N�Z���̐F������
	float4 LightColor = saturate(dot(InputNormal, LightDirection) + float4(0.7, 0.7, 0.7, 1.0));
	//���C�g�̓��������͂Ȃ�
	LightColor.a = 1;
	//�s�N�Z���̐F������
	float4 Col = input.color;
	//�e�N�X�`���F�ƃ��C�g�F������
	return Col * LightColor;
}


