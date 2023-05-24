#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv);

	//フェード

	//ぼかし
	if(isBlurActive){
		//円内判定
		float2 center = float2(0.5f,0.5f);
		float radius1 = 0.1;
		float radius2 = 1.0f;
		float2 calValue = float2((input.uv.x-center.x)*(input.uv.x-center.x), (input.uv.y-center.y)*(input.uv.y-center.y));

		//ぼかし
		float2 texelSize = float2(1.0f/1280.0f, 1.0f/720.0f);
		float4 colorSum = float4(0.0f,0.0f,0.0f,0.0f);
		int BlurMax = blurValue;

		for (int x = -BlurMax; x < BlurMax; x++)
		{
			for (int y = -BlurMax; y < BlurMax; y++)
			{
				colorSum += tex.Sample(smp, (input.uv) + float2(x, y) * texelSize);
			}
		}
		colorSum /= (BlurMax*2*BlurMax*2);  // 周囲9ピクセルの平均値を計算


		if(calValue.x+calValue.y <= radius1*radius1){
			return float4(texcolor.rgb,1);
		}
		else if(calValue.x+calValue.y <= radius2*radius2){
			return float4(colorSum.rgb, 1);
		}
	}

	return float4(texcolor.rgb,1);
}