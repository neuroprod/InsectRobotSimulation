#version 150 core
uniform float alpha;
uniform vec3 uLightPos;
uniform sampler2DShadow uShadowMap;

in vec2 vUV;
in vec4 vColor;
in vec4 vPosition;
in vec3 vNormal;
in vec4 vShadowCoord;

out vec4 Color;

float samplePCF4x4( vec4 sc )
{
	const int r = 1;
	const int s = 2 * r;
	
	float shadow = 0.0;
	shadow += textureProjOffset( uShadowMap,  sc, ivec2(-s,-s) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2(-r,-s) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2( r,-s) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2( s,-s) );
	
	shadow += textureProjOffset( uShadowMap,  sc, ivec2(-s,-r) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2(-r,-r) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2( r,-r) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2( s,-r) );
	
	shadow += textureProjOffset( uShadowMap,  sc, ivec2(-s, r) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2(-r, r) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2( r, r) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2( s, r) );
	
	shadow += textureProjOffset( uShadowMap,  sc, ivec2(-s, s) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2(-r, s) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2( r, s) );
	shadow += textureProjOffset( uShadowMap,  sc, ivec2( s, s) );
		
	return shadow/16.0;
}



void main( void )
{
	vec3 Normal			= normalize( vNormal );
	vec3 LightVec		= normalize( uLightPos - vPosition.xyz );
	float NdotL			=  dot( vNormal, LightVec )*0.5 +0.5;
	
	vec3 Diffuse		= vec3( NdotL );
	vec3 Ambient		= vec3( 0.1 );
	
	vec4 sc = vShadowCoord;
	vec4 ShadowCoord	= vShadowCoord / vShadowCoord.w;


	sc.z -= 0.005;

	float shadow = samplePCF4x4( sc );
	shadow =shadow*0.5+0.5;
	
	
	vec2 uv=step(1.0,mod (vUV*100.0,2.0));
	
	float col =(uv.x +uv.y)*0.5 ;
	col =col *(1.0-(uv.x +uv.y)*0.5 );
	col=col +0.5;

	Color.rgb = ( Diffuse  * shadow  + Ambient )* vec3(col);//*Color.rgb;

	Color.a	= alpha;
	
}