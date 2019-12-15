uniform extern float4x4 gWVP ;
uniform extern texture gTex ;
uniform extern float3  gEyePosL;
uniform extern float3  gAccel ;
uniform extern float   gTime ;
uniform extern float   gViewportHeight ;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = LINEAR ;
	MagFilter = LINEAR ;
	MipFilter = POINT  ;
	AddressU = CLAMP ;
	AddressV = CLAMP ;
};

struct OutputVS
{
	float4 posH : POSITION ;
	float4 color: COLOR0   ;
	float2 tex0 : TEXCOORD0 ;
	float  size : PSIZE ;
};

//VS
OutputVS FireRingVS(
		float3 posL: POSITION,
		float3 vel : TEXCOORD0,
		float size : TEXCOORD1,
		float time : TEXCOORD2,
		float lifeTime: TEXCOORD3,
		float mass : TEXCOORD4,
		float4 color: COLOR0
)
{
	//Clear the output
	OutputVS outVS = (OutputVS)0 ;

	float t = gTime - time ;

	posL = posL + t * vel ;

	outVS.posH = mul(float4(posL,1.0f), gWVP);

	size += 0.8 * t ;

	float d = distance(posL, gEyePosL);
	outVS.size = size ; //gViewportHeight * size / (1.0 + 8.0f*d);
	color.r = 0.0f ;
	color.g = 0.0f ;
	color.b = 1.0f * (t / lifeTime) ; 
	color.a = 1.0f - 1.0f * (t / lifeTime) ; 
	outVS.color = color ;//(1.0f - (t / lifeTime)) ;

	return outVS ;
}

//PS
float4 FireRingPS(float4 color:COLOR0,
                  float2 tex0: TEXCOORD0):COLOR
{
	return color * tex2D(TexS, tex0);
}

technique FireRingTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 FireRingVS();
		pixelShader = compile ps_2_0 FireRingPS();

		PointSpriteEnable = true ;
		AlphaBlendEnable = true ;
		SrcBlend = One ;
		DestBlend = One ;
		ZWriteEnable = false ;
	}
}