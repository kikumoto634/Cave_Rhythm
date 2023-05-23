cbuffer cbuff0 : register(b0)
{
	bool isActive;
	float2 offset;
	float3 color;
};

//頂点シェーダーからの出力構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};