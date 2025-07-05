
#include "common.hlsl"


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );		//テクスチャ変数
SamplerState	g_SamplerState : register( s0 );//サンプラー変数

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in float4 inPosition : SV_POSITION,     //変換後頂点座標
		   in float4 inWorldPosition : POSITION0,  //ワールド変換後頂点座標,ポジションゼロ
                     
                     
		   in float4 inNormal : NORMAL0,           //頂点法線
		   in float4 inDiffuse : COLOR0,           //頂点カラー
		   in float2 inTexCoord : TEXCOORD0,       //テクスチャ座標

   		   out float4 outDiffuse	: SV_Target )	//最終的なピクセルのカラー
{
	
	//ピクセルの法線を正規化
    float4 normal = normalize(inNormal);
	// lv = 光源からピクセルへのベクトル 　　光源--------->ピクセル
    float4 lv = inWorldPosition - Light.Position;
	// ld = 光源とピクセルの距離	
    float4 ld = length(lv); //ld.x ld.y ld.z ld.w に長さが入る
	// ofs = 減衰値の計算
    float ofs = 1.0f - (1.0f / Light.PointLightParam.x) * ld;
    ofs = max(0, ofs); //0未満は0にする　ofs=saturate(ofs)=>ofsを0～1の値へ補正

	//光源計算
    float light = -dot(normal.xyz, normalize(lv.xyz));
    light = saturate(light); //0.0f未満は0.0、1.0f以上は1.0fにする
	//明るさを減衰させる
    light *= ofs;

	//テクスチャの色を取得
    outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);
	//色と明るさを合成
    outDiffuse.rgb *= inDiffuse.rgb * Light.Diffuse.rgb * light;
    outDiffuse.rgb += Light.Ambient.rgb;
	//αは別処理
    outDiffuse.a *= inDiffuse.a;
	

	//======================================

	//  C------------>P
	//カメラからピクセルへ向う視線ベクトル
    float3 eyev = inWorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev); //正規化

	//光の反射ベクトルを計算
    float3 refv = reflect(normalize(Light.Direction.xyz), normal.xyz);
    refv = normalize(refv); //正規化

	//鏡面反射の計算
    float specular = -dot(eyev, refv);
    specular = saturate(specular);
    specular = pow(specular, 30.0f); //ここでは30乗してみる

    outDiffuse.rgb += (specular * ofs); //スペキュラをデフューズ値に加算する
	
}
