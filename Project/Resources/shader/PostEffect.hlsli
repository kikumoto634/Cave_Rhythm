cbuffer cbuff0 : register(b0)
{
	bool isActive;
	float2 offset;
	float3 color;
};

//���_�V�F�[�_�[����̏o�͍\����
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};