#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv);

	//フェード
	if(isFadeActive){
		return float4(texcolor.rgb*fadeColor,1);
	}
	//ぼかし
	else if(isBlurActive){
		//ガウシアンブラー
		float totalWeight = 0;
		float _Sigma = 0.01;
		float _StepWidth = 0.001;
		float Blur = _Sigma*2;
		float4 col = float4(0,0,0,1);

        for(float py = -Blur; py <= +Blur; py +=_StepWidth){
            for(float px = -Blur; px <= +Blur; px +=_StepWidth){

                float2 pickUV = input.uv + float2(px,py);
                float weight = Gaussian(input.uv, pickUV, _Sigma);
                col += tex.Sample(smp, pickUV) * weight;
                totalWeight += weight;
            }
        }
        col.rgb = col.rgb / totalWeight;


		//円内判定
		float2 center = float2(0.5f,0.45f);
		float radius1 = 0.15;
		float radius2 = 1.0f;
		float2 calValue = float2((input.uv.x-center.x)*(input.uv.x-center.x), (input.uv.y-center.y)*(input.uv.y-center.y));
		float radius1x2 = radius1*radius1;
		float radius2x2 = radius2*radius2;

		//条件により戻り
		if(calValue.x+calValue.y <= radius1x2){
			return float4(texcolor.rgb,1);
		}
		else if(calValue.x+calValue.y <= radius2x2){
			return float4(col.rgb, 1);
		}
	}

	return float4(texcolor.rgb,1);
}