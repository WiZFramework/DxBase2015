

#include "BasicBoneParameters.hlsli"

#include "Structures.hlsli"
#include "Common.hlsli"
#include "Lighting.hlsli"


// Pixel shader: vertex lighting, no fog.
float4 main(PSInputTxShadow pin) : SV_Target0
{
	float shadowColor = 1.0f;
	if (activeflags.y == 1){
		// �e�N�Z�����v�Z
		float2 texel = float2(pin.posLWVP.x / pin.posLWVP.w * 0.5f + 0.5f, pin.posLWVP.y / pin.posLWVP.w * -0.5f + 0.5f);
		float depth = 0;
		float sm = 0;
		depth = pin.posLWVP.z / pin.posLWVP.w - 0.00005f;
		sm = g_DepthMap.Sample(g_SamplerDepthMap, texel).x;
		if (texel.x >= 0 && texel.x <= 1 && texel.y >= 0 && texel.y <= 1){
			// ���݂̐[�x�l�Ɛ[�x�}�b�v��̐[�x�l���r
			if (sm > depth){
				shadowColor = 1.0f;
			}
			else{
				shadowColor = 0.6f;
			}
		}
	}
	float4 color = 0;
	if (activeflags.w == 1){
		//�e�N�X�`���I�����[
		//�e�N�X�`���ƍ��킹�Ȃ�
		color = Texture.Sample(Sampler, pin.TexCoord);
	}
	else{
		//�e�N�X�`���ƍ��킹��
		color = Texture.Sample(Sampler, pin.TexCoord) * pin.Diffuse;
	}
	color.rgb = color.rgb * shadowColor;
	AddSpecular(color, pin.Specular.rgb);
	return color;
}

