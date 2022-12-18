//3D�ϊ��s��
cbuffer ConstBufferData : register(b0)
{
	matrix mat;	//3D�ϊ��s��
	matrix matBillboard;	//�r���{�[�h�s��
};

//���_�V�F�[�_�[�̏o�͍\����
//(���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����)
struct VSOutput
{
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	float scale : TEXCOORD;
};

//�W�I���g���V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̏o��
struct GSOutput
{
	//�V�X�e���p���_���W
	float4 svpos : SV_POSITION;
	//uv�l
	float2 uv : TEXCOORD;
};