
#include "InstanceInc.hlsli"

Texture2D<float4> SimpleTexture : register(t0);
// �[�x�}�b�v
Texture2D g_DepthMap : register(t1);

SamplerState SimpleSampler : register(s0);
SamplerState g_SamplerDepthMap : register(s1);


float4 main(PixelShaderInput input) : SV_TARGET
{
	//�e�����邩�Ȃ���������
	float3 shadowColor = float3(1.0f, 1.0f, 1.0f);
	// �e�N�Z�����v�Z
	float2 texel = float2(input.posLWVP.x / input.posLWVP.w * 0.5f + 0.5f, input.posLWVP.y / input.posLWVP.w * -0.5f + 0.5f);
	float depth = input.posLWVP.z / input.posLWVP.w - 0.00005f;
	float sm = g_DepthMap.Sample(g_SamplerDepthMap, texel).x;
	if (texel.x >= 0 && texel.x <= 1 && texel.y >= 0 && texel.y <= 1){
		// ���݂̐[�x�l�Ɛ[�x�}�b�v��̐[�x�l���r
		if (sm < depth){
			//�e��70%�̖��x
			shadowColor.xyz = 0.7f;
		}
		else{
			shadowColor.xyz = 1.0f;
		}
	}
	float3 lightdir = normalize(LightDir.xyz);
	float3 N1 = normalize(input.norm);
	//�ŏI�F�̌���
	//�܂����C�e�B���O�̉e��
	float4 RetColor = saturate(dot(N1, -lightdir) + float4(0.7, 0.7, 0.7, 1.0));
	//���C�g�̓��������͂Ȃ�
	RetColor.a = 1;
	//�e�N�X�`���ƍ��킹��
	RetColor = SimpleTexture.Sample(SimpleSampler, input.tex) * RetColor;
	//�e�ƍ��킹��
	RetColor.rgb = RetColor.rgb * shadowColor;
	//����F���^�[��
	return RetColor;
}

