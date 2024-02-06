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
	float4    ambientColor;
	float4     speculerColor;
	float    scroll;
	float     shininess;
	int		isTexture;		   // �e�N�X�`���\���Ă��邩�ǂ���
	int     hasNormalMap;
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
	float4 normal: NORMAL;
	float4 light : POSITION2;
	float4 color : COLOR;	//�F�i���邳�j
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

	float3 binormal = cross(tangent, normal);
	binormal = mul(binormal, matNormal);
	binormal = normalize(binormal);

	outData.normal = normalize(mul(normal, matNormal));
	normal.w = 0;

	tangent = mul(tangent, matNormal);
	tangent = normalize(tangent);
	tangent.w = 0;

	float4 eye = normalize(mul(pos, matW) - eyePos);
	outData.eyev = eye;

	outData.Neyev.x = dot(eye, tangent);
	outData.Neyev.y = dot(eye, binormal);
	outData.Neyev.z = dot(eye, outData.normal);
	outData.Neyev.w = 0;

	float4 light = normalize(lightPos);
	light.w = 0;
	light = normalize(light);

	outData.color = mul(light, outData.normal);
	outData.color.w = 0.0f;

	outData.light.x = dot(light, tangent);
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, outData.normal);
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
	float4 Specular;

	float2 tmpNormalUV = inData.uv;
	tmpNormalUV.x = tmpNormalUV.x + scroll;
	tmpNormalUV.y = tmpNormalUV.y + scroll;
	if (hasNormalMap){
		

		float4 tmpNormal = normalTeX.Sample(g_sampler, tmpNormalUV) * 2.0f - 1.0f;
		tmpNormal = normalize(tmpNormal);
		tmpNormal.w = 0;

		float4 NL = clamp(dot(normalize(inData.light), tmpNormal), 0, 1);

		float4 reflection = reflect(normalize(inData.light), tmpNormal);
		float4 Specular = pow(saturate(dot(reflection, inData.Neyev)), shininess) * speculerColor;
		if (isTexture!=0)
		{
			diffuse = lightSource*g_texture.Sample(g_sampler, inData.uv) * NL;
			ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
		}
		else
		{
			diffuse = lightSource * diffuseColor * NL;
			ambient = lightSource * diffuseColor * ambientColor;
		}
		//return g_texture.Sample(g_sampler, inData.uv);

		return  diffuse +ambient + Specular;
	}
	else
	{
		float4 reflection = reflect(normalize(lightPos), inData.normal);

		float4 Specular = pow(saturate(dot(reflection, inData.eyev)), shininess) * speculerColor;
		if (isTexture==0)
		{
			diffuse = lightSource * diffuseColor * inData.color;
			ambient = lightSource * diffuseColor * ambientColor;
		}
		else
		{
			diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * inData.color;
			ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambientColor;
		}
		float4 result = diffuse +ambient+Specular ;
		if (isTexture)
			result.a = inData.uv;
		return result;
	}
}
