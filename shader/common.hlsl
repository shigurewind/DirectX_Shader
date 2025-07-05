

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
cbuffer ConstantBuffer : register( b0 )
{
	matrix WorldViewProjection;
}


cbuffer WorldBuffer : register(b2)
{
    matrix World;
}


// �}�e���A���o�b�t�@
cbuffer MaterialBuffer : register( b1 )
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit���E�p
}

struct LIGHT
{
    bool		Enable; //�L���t���O
    bool3		Dummy;
    float4		Direction;
    float4		Diffuse;
    float4		Ambient;
    
    float4      Position; //���C�g�̈ʒu
    float4      PointLightParam; //x = ���͂�����
    
    float4      Angle;
};


cbuffer LightBuffer : register(b3)
{
    LIGHT Light;
    //int LightCount;
    //float4 CameraPosition;
    //float4 CameraDirection;
    //float4 CameraUp;
    //float4 CameraRight;
};

cbuffer CameraBufffer : register(b4)
{
    float4 CameraPosition;
    
};
