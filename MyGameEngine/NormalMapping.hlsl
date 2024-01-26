//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー
Texture2D      normalTeX : register(t1);
//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global:register(b0)
{
	float4x4	matWVP;			       // ワールド・ビュー・プロジェクションの合成行列
	float4x4    matW;                 //ワールド行列
	float4x4	matNormal;           // ワールド行列
	float4		diffuseColor;		// ディフューズカラー（マテリアルの色）
	float4    ambientColor;
	float4     speculerColor;
	float     shininess;
	int		isTexture;		   // テクスチャ貼ってあるかどうか
	int     hasNormalMap;
};

cbuffer gmodel:register(b1)
{
	float4      lightPos;           //ライトの位置
	float4       eyePos;           //視点
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos   : SV_POSITION;	//位置
	float2 uv	 : TEXCOORD;		//UV座標
	float4 eyev  :  POSITION;
	float4 Neyev : POSITION1;
	float4 normal: NORMAL;
	float4 light : POSITION2;
	float4 color : COLOR;	//色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL,float4 tangent :TANGENT)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData = (VS_OUT)0;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);
	outData.uv = (float2)uv;

	float3 binormal = cross(tangent, normal);
	//float4 binormal = { tmp,0 };
	binormal = mul(binormal, matNormal);
	binormal = normalize(binormal);

	normal.w = 0;
	outData.normal = normalize(mul(normal, matNormal));

	tangent = mul(tangent, matNormal);
	tangent.w = 0;
	tangent = normalize(tangent);

	float posw = mul(pos, matW);
	outData.eyev = normalize(posw - eyePos);

	outData.Neyev.x = dot(outData.eyev, tangent);
	outData.Neyev.y = dot(outData.eyev, binormal);
	outData.Neyev.z = dot(outData.eyev, normal);
	outData.Neyev.w = 0;

	float4 light = normalize(lightPos);
	light.w = 0;
	light = normalize(light);

	outData.color = mul(light, outData.normal);
	outData.color.w = 0.0f;

	outData.light.x = dot(light, tangent);
	outData.light.y = dot(light, binormal);
	outData.light.z = dot(light, normal);
	outData.light.w = 0;

	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{

	float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
	float4 diffuse;
	float4 ambient;
	float4 Specular;
	


	if (hasNormalMap){
		

		float4 tmpNormal = normalTeX.Sample(g_sampler, inData.uv) * 2.0f - 1.0f;
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

		
		return   Specular;
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
		 return   Specular ;
	

		
	}
}
