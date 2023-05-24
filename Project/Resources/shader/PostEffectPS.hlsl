#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv);

	//円内判定
	float2 center = float2(0.5f,0.5f);
	float radius1 = offset.x;
	float radius2 = radius1+0.1f;
	float2 calValue = float2((input.uv.x-center.x)*(input.uv.x-center.x), (input.uv.y-center.y)*(input.uv.y-center.y));

	//ぼかし
	float2 texelSize = float2(1.0f/1280.0f, 1.0f/720.0f);
	float4 colorSum = (0.0f,0.0f,0.0f,0.0f);

    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            colorSum += tex.Sample(smp, (input.uv) + float2(x, y) * texelSize);
        }
    }
    colorSum /= (16.0f*16.0f);  // 周囲9ピクセルの平均値を計算


	if(calValue.x+calValue.y <= radius1*radius1){
		return float4(texcolor.rgb,1);
	}
	else if(calValue.x+calValue.y <= radius2*radius2){
		return float4(colorSum.rgb, 1);
	}

	return float4(texcolor.rgb,1);
}