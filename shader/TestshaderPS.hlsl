
#include "common.hlsl"


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );		//テクスチャ変数
SamplerState	g_SamplerState : register( s0 );//サンプラー変数

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in  float4 inPosition	: POSITION0,	//ピクセルの座標
		   in  float4 inNormal		: NORMAL0,		//ピクセルの法線
		   in  float4 inDiffuse		: COLOR0,		//ピクセルのカラー
		   in float2  inTexCoord	: TEXCOORD0,	//ピクセルのテクスチャ座標
   		   out float4 outDiffuse	: SV_Target )	//最終的なピクセルのカラー
{
	//テクセルをフェッチして出力
	outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);

//    outDiffuse.rgb = 0; //色のRGBの値を一気に０にする

//    outDiffuse.rgb = 1.0f - outDiffuse.rgb; //色のRGBの値を反転する
	
//	outDiffuse.rgb *= outDiffuse.rgb;//色の値を２乗する

    outDiffuse.rgb = outDiffuse.r * 0.3f +
	                 outDiffuse.g * 0.6f +
					outDiffuse.b * 0.1f;	//色をグレースケールにする
	
}
