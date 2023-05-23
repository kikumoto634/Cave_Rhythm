#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv);

	//return float4(texcolor.rgb, 1.0f);*/

	float2 texelSize = float2(1.0f/1280.0f, 1.0f/720.0f);
	float4 color = (0,0,0,0);

    for (int x = 0; x < 16; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            color += tex.Sample(smp, input.uv + float2(x, y) * texelSize);
        }
    }
    color /= (16.0f*16.0f);  // 周囲9ピクセルの平均値を計算

    return float4(color.rgb, 1);
}