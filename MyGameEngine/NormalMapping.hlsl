//������������������������������������������������������������������������������
 // �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D		g_texture : register(t0);	//�e�N�X�`���[
SamplerState	g_sampler : register(s0);	//�T���v���[
Texture2D      normalTeX : register(t1);
//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global:register(b0)
{
	float4x4	matWVP;			       // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
	float4x4    matW;                 //���[���h�s��
	float4x4	matNormal;           // ���[���h�s��
	float4		diffuseColor;		// �f�B�t���[�Y�J���[�i�}�e���A���̐F�j
	float4     ambient;
	float4     speculer;
	float     shininess;
	bool		isTexture;		   // �e�N�X�`���\���Ă��邩�ǂ���
	bool       hasNormalMap;
};

cbuffer gmodel:register(b1)
{
	float4      lightPos;           //���C�g�̈ʒu
	float4       eyePos;           //���_

};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
	float4 pos   : SV_POSITION;	//�ʒu
	float2 uv	 : TEXCOORD;		//UV���W
	float4 eyev  :  POSITION;
	float4 Neyev : POSITION1;
	float4 normal: POSITION2;
	float4 light : POSITION3;
	float4 color : POSITION4;	//�F�i���邳�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL,float4 tangent :TANGENT)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
	VS_OUT outData = (VS_OUT)0;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
	outData.pos = mul(pos, matWVP);
	outData.uv = (float2)uv;

	float3 binormal = cross(normal, tangent);

	normal.w = 0;
	normal = mul(normal, matNormal);
	normal = normlize(normal);
	outData.normal = normal;

	tangent.w = 0;
	tangent = mul(tangent, matnormal);
	tangent = normlize(tangent);

	binormal = mul(binormal, matNormal);
	binormal = normalize(binormal);

	float4 posw = mul(pos, matW);
	outData.eyev = eyePos - posw;

	outData.Neyev.x = dot(outData.eyev, tangent);
	outData.Neyev.y = dot(outData.eyev, binormal);
	outData.Neyev.z = dot(outData.eyev, normal);
	outData.Neyev.w = 0;

	float4 light = normlize(lightPos);
	light = normalize(light);

	outData.color = mul(light, normal);

	outData.light.x = dot(light, tangent);
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, normal);
	outData.light.w = 0;

	return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{

	float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
	float4 diffuse;
	float4 ambient;
	if (hasNormalMap)
	{
		inData.light = normlize(inData.light);

	}
}
