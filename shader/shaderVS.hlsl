
#include "common.hlsl"

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

//=============================================================================
// 頂点シェーダ
//=============================================================================
void main( in  float4 inPosition	: POSITION0,	//頂点座標
		   in  float4 inNormal		: NORMAL0,		//頂点法線
		   in  float4 inDiffuse		: COLOR0,		//頂点カラー
		   in  float2 inTexCoord	: TEXCOORD0,	//テクスチャ座標

		   out float4 outPosition	: SV_POSITION,	//変換後頂点座標
		   out float4 outNormal		: NORMAL0,		//頂点法線
		   out float4 outDiffuse	: COLOR0,		//頂点カラー
		   out float2 outTexCoord	: TEXCOORD0		 //テクスチャ座標
		)
{
	//頂点座標をWVP変換して出力
	outPosition = mul( inPosition, WorldViewProjection );
	//法線を出力
	outNormal =	inNormal;
	//テクスチャ座標を出力
	outTexCoord = inTexCoord;
	//頂点カラーとマテリアル色を合成して出力
	outDiffuse = inDiffuse * Diffuse;

}


