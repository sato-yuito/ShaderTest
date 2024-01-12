//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D		g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

Texture2D       toon_texture:register(t1);
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
	float4     ambient;
	float4     speculer;
	float     shininess;
	bool		isTexture;		   // テクスチャ貼ってあるかどうか
};

cbuffer global:register(b1)
{
	float4      lightPos;           //ライトの位置
	float4       eyePos;           //視点

};
//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos  : SV_POSITION;	//位置
	float2 uv	: TEXCOORD;		//UV座標
	float4 color	: COLOR;	//色（明るさ）
	float4 eyev  :  POSITION;
	float4 normal : NORMAL;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData = (VS_OUT)0;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;
	normal.w = 0;
	normal = mul(normal, matNormal);
	normal = normalize(normal);
	outData.normal = normal;

	float4 light = normalize(lightPos);
	//light = normalize(light);

	outData.color = saturate(dot(normal, light));
	float4 posw = mul(pos, matW);
	outData.eyev = eyePos - posw;

	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
	float4 ambentSource = ambient;
	float4 diffuse;
	float4 ambient;
	float4 NL = dot(inData.normal, normalize(lightPos));
	float4 reflect = normalize(2 * NL * inData.normal - normalize(lightPos));
	float4 pspecular = pow(saturate(dot(reflect, normalize(inData.eyev))), shininess) * speculer;

	//
	////拡散反射の値を返す(?)
	//float4 n1 = float4(1 / 4.0, 1 / 4.0, 1 / 4.0, 1);
	//float4 n2 = float4(2 / 4.0, 2 / 4.0, 2 / 4.0, 1);
	//float4 n3 = float4(3 / 4.0, 3 / 4.0, 3 / 4.0, 1);
	//
	//float4 tI = 0.1 * step(n1, inData.color) + 0.3 * step(n2, inData.color)
	//	+ 0.3 * step(n3, inData.color);

	float2 uv;
	uv.x = inData.color.x;
	uv.y = abs(dot(inData.normal, normalize(inData.eyev)));


	float4 tI = toon_texture.Sample(g_sampler,uv);
	if (isTexture == false)
	{
		diffuse = lightSource * diffuseColor * tI;
		ambient = lightSource * diffuseColor * ambentSource;
	}
	else
	{
		diffuse = lightSource * g_texture.Sample(g_sampler, inData.uv) * tI;
		ambient = lightSource * g_texture.Sample(g_sampler, inData.uv) * ambentSource;
	}

	return diffuse + ambient+ pspecular;



	//return float4 (1,1,1,1);

}