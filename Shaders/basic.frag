#version 330 core

in vec3 fragmentColor;
in vec3 fragPos;
in vec2 UV;

in float cosTheta;
in float cosAlpha;
in float fragLightDist;

in vec3 fragNormal;
in float fragParamCase;
in vec3 fragPosMouse;
in float fragTime;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
	float alpha = 1.0f;

	vec4 MaterialDiffuseColor = vec4(texture( textureSampler, UV ).rgb, alpha );

	//------------- Light
	float distance = fragLightDist;
	float LightPower = 0.9f;

	vec4 LightColor = vec4(1.0f,1.0f,1.0f,1.0f);
	//------------------ Dark light color
	vec4 MaterialAmbientColor = vec4(0.1f,0.1f,0.1f,1.0f) * MaterialDiffuseColor;

	float intens = 1.0f;
	
	vec4 MaterialSpecularColor = vec4(texture( textureSampler, UV ).rgb, 1.0 );
	
	vec4 color1 = MaterialAmbientColor + MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) ;
	vec4 color2 = MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,intens) / (distance*distance);
	color2 *= 0;
	color = color1  + color2 + vec4(vec3(fragmentColor),0.5f)*0.1;

	//--------
	float borderCurs = 1.5f;
	if(fragPos.x < fragPosMouse.x + borderCurs &&
		fragPos.x > fragPosMouse.x - borderCurs &&
		fragPos.y < fragPosMouse.y + borderCurs &&
		fragPos.y > fragPosMouse.y - borderCurs 
	)
	{		
		
	    vec2 st = fragPos.xy/UV.xy;
	    vec2 pos = vec2(0.5)-st;
	    float r = length(pos)*0.4;
	    float a = atan(pos.y,pos.x);
	    float f = smoothstep(-.5,.3,cos(a*10. - fragTime*10.))*0.1+0.5;
	    float f2 = abs(cos(a*5.+ fragTime*5. ))*0.45;
	    vec4 colorC = vec4(vec3(smoothstep(f2, f2 +0.02,r) - smoothstep(f, f+0.02,r)),1.0f);
     	    //vec4 colorC = vec4(vec3(plot(f,r)),1.0f);
	    color = color + colorC;
	}	
	

}