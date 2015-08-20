
#include "SimpleInc.hlsli"

Texture2D SimpleTexture : register(t0);
SamplerState SimpleSampler : register(s0);


float4 main(PixelShaderInput input) : SV_TARGET
{
	//���C�g�̌����𓾂�
	float3 LightDirection = -normalize(float3(LightDir.xyz));
	float3 InputNormal = normalize(input.norm);
	//���C�g�ɂ��s�N�Z���̐F������
	float4 LightColor = saturate(dot(InputNormal, LightDirection) + float4(0.7, 0.7, 0.7, 1.0));
	//���C�g�̓��������͂Ȃ�
	LightColor.a = 1;
	//�e�N�X�`���ɂ��s�N�Z���̐F������
	float4 TexCol = SimpleTexture.Sample(SimpleSampler, input.tex);
	//�e�N�X�`���F�ƃ��C�g�F������
	return TexCol * LightColor;
}


