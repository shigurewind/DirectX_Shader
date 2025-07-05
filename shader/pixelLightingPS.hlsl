
#include "common.hlsl"


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );		//テクスチャ変数
SamplerState	g_SamplerState : register( s0 );//サンプラー変数

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(
           in float4 inPosition : SV_POSITION, //変換後頂点座標
		   in float4 inWorldPosition : POSITION0, //ワールド変換後頂点座標,ポジションゼロ
                     
                     
		   in float4 inNormal : NORMAL0, //頂点法線
		   in float4 inDiffuse : COLOR0, //頂点カラー
		   in float2 inTexCoord : TEXCOORD0, //テクスチャ座標

   		   out float4 outDiffuse	: SV_Target )	//最終的なピクセルのカラー
{
	
	//ピクセルの法線を正規化
    float4 normal = normalize(inNormal);

    //光源計算
    float light = -dot(normal.xyz, Light.Direction.xyz);
    light = saturate(light); //0.0f～1.0fに制限
	
	//テクスチャの色を取得
    outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);
	
	//色と明るさを合成
    outDiffuse.rgb *= inDiffuse.rgb * Light.Diffuse.rgb * light; //頂点カラーを乗算
    outDiffuse.rgb += Light.Ambient.rgb; //環境光を加算
    
	//スペキュラ計算(鏡面反射)
    //float3 viewDir = normalize(-inPosition.xyz); //カメラ方向
    //float3 reflectDir = reflect(Light.Direction.xyz, normal.xyz); //反射ベクトル
    //float specular = pow(saturate(dot(viewDir, reflectDir)), MaterialBuffer.Shininess);
    //outDiffuse.rgb += specular * MaterialBuffer.Specular.rgb; //スペキュラを加算
    
    
	//αは別処理
    outDiffuse.a *= inDiffuse.a; //頂点カラーのα値を乗算
    
    //------------------
    //カメラからの視線ベクトル
    float3 cameraView = inPosition.xyz - CameraPosition.xyz; 
    cameraView = normalize(cameraView); //正規化
    
    //光の反射ベクトル計算
    float3 reflectDir = reflect(normalize(Light.Direction.xyz), normal.xyz); //反射ベクトル
    reflectDir = normalize(reflectDir); //正規化
    
    //鏡面反射の計算
    float specular = -dot(cameraView, reflectDir); //カメラ視線と反射ベクトルの内積
    specular = saturate(specular); //0.0f～1.0fに制限
    //specular = pow(specular, Shininess); //シャイニネスで調整
    specular = pow(specular, 30.0f); //シャイニネスで調整
    
    outDiffuse.rgb += specular; //スペキュラを加算
	
}
