#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv);

	//�t�F�[�h
	if(isFadeActive){
		return float4(texcolor.rgb*fadeColor,1);
	}
	//�ڂ���
	else if(isBlurActive){
		float4 col = {0,0,0,0};
		float2 symmetryUV = input.uv - 0.5f;
		float distance = length(symmetryUV);
		float factor = 1.0f / 16 * distance;
		for(int j = 0; j < 16; j++){
			float uvOffset = 1 - factor * j;
			col += tex.Sample(smp, symmetryUV * uvOffset + 0.5);
		}

		col /= 16;
		return col;
	}

	return float4(texcolor.rgb,1);
}