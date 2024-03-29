//#version 330 core
#version 420 core

in vec3 fragmentColor;
in vec2 UV;
in float fragParamCase;
in float fragTime;
flat in int numSymbol;
flat in int fragParamValue;

out vec4 color;

//version 420 core
layout(binding = 0) uniform sampler2D textureSampler;
layout(binding = 1) uniform sampler2D textureSamplerEng;

vec4 SDF1(in vec4 inColor )
{
     float contrast = 40.0;
    vec3 c = inColor.xxx;
	return vec4((c-0.5)*contrast,1.0);
}

vec4 SDF2(in vec4 inColor )
{
    const float threshold = .21;
    //const float threshold = 0.9;
	//vec3 c2 = texture2D(tex,gl_FragCoord.xy/vec2(256., 128.)*.3).xxx;
    vec3 c = inColor.xxx;
	//vec3 res = smoothstep(.5-threshold, .5+threshold, c);
    vec3 res = smoothstep(.3-threshold, .3+threshold, c);
	return vec4(res,1.0);
}

vec4 SDF2_2(in vec4 inColor )
{
    vec3 c = inColor.xxx;
    const float threshold = 0.75;
    float cent = .35;
	vec3 res = smoothstep(cent-threshold, cent+threshold, c);
	return vec4(res,1.0);
}

vec4 Outline(in vec4 inColor )
{
    float contrast = 10.;
    vec3 c = inColor.xxx;
	vec3 c1 = (c-.45) * contrast;
	vec3 c2 = 1.-(c-.5) * contrast;
	vec3 res3 = mix(c1, c2, (c-.5)*contrast);
	return vec4(res3,1.0);
}

vec4 SDF4(in vec4 inColor ){

    //float threshold = .01;
    float threshold = 0.02;
	vec3 c = inColor.xxx;
    vec3 res = smoothstep(.5-threshold, .5+threshold, c);
    return vec4(res,1.);
}

vec4 GlowEffect(in vec4 inColor ){
    float contrast = 36.;
    float glow = 5.;
    
    vec3 c = inColor.xxx;
    vec3 c1 = clamp((c-.5)*contrast,0.,1.);
    vec3 c2 = clamp(1.-(c-.5)/glow, 0., 1.);
    vec3 res = 1.-mix(c1, c2, (c-.5)*contrast);
    //return vec4(res,1.0);
    return vec4(1.) - vec4(res,1.0);
}


void main()
{
    int StartPressedParamShaderID = 6;

	float alpha = 0.0f;
    float alphaOff = 1.0f;
    float bordB = 0.1;
    float bordR = 0.1;
    int colSymbRow = 7;
    vec2 result = vec2(0);
    int index = 0;
    bool isEng = false;
    int paramValue = fragParamValue;

    int numSymb = numSymbol;
    
    if(numSymb >= 100){
        numSymb -= 100;
        isEng = true;
        //paramValue = 7; //TEST
    }

    if(numSymb < 0)
    {
        color = vec4(0.);
        //color = vec4(.0,.7,.0,1.);
        return;
    }

    vec2 uv = UV;
    vec2 sempS = UV;
    //--- size
    sempS.x *= 0.15;
    sempS.y *= 0.15;
    
    //--- offset X Y
    float sizeofNextSymb = .1427;
    float stepX = (sizeofNextSymb * float(numSymb)) - 0.00;
    sempS.x += stepX;

    //------------------------- Y
        float nextY =  -.145 * (floor(float(numSymb)/float(colSymbRow)));
    float stepY = -.15 + nextY;
    sempS.y += stepY;
    
    //---- border
    vec2 bl = vec2(step(0.05,uv.x) , step(bordB,uv.y));       // bottom-left
    vec2 tr = vec2(step(bordR,1.0-uv.x) , step(0.05,1.0-uv.y));   // top-right
    float brd = bl.x * bl.y * tr.x * tr.y;
    result = (brd * sempS);

    //anima
    //result.x += cos(result.y*cos(fragTime));
    //result.y +=sin(result.x*sin(fragTime)*.01);
    
    vec4 text;	
    if(isEng)
        text = texture( textureSamplerEng, result );	
    else
        text = texture( textureSampler, result );	

    vec4 text1 = text;
 
    //--------- SDF
    // if(paramValue == 1) //1
	//     text1 = SDF1(text1);
    //-----------
    if(paramValue == 0) 
    {
        //Text Box - default
        text1 = SDF2_2(text1);
    }
    if(paramValue == 1) 
    {
        //Edit box - disable
        text1 = SDF2_2(text1);
    }
    if(paramValue == StartPressedParamShaderID)
    {
         //Edit box - enaable
        text1 = SDF2_2(text1);
        text1 = SDF2(text1);
    }
    //-----------
    // if(paramValue == 3) //3     
    //     text1 = SDF4(text1);
    // //----------- GlowEffect
    // if(paramValue == 4) //4
    //     text1 = GlowEffect(text1);
    // //----------- outline
    // if(paramValue == 6) //5
    //     text1 = Outline(text1);
    // //---------------------
    // if(paramValue == 7) //5
    //     text1 = SDF2_2(text1);
    //---------------------
    
    vec4 colorText = vec4(0.984,0.773,0.196, 1.);
    colorText = vec4(0.996, 0.996, 0.482, 1.);

    float sumColor = text1.r * text1.g * text1.b;
    alpha = smoothstep(0.0, 0.5, sumColor);
    
    // EditBox
    if(paramValue == 1)
    {
        color =  text1 * vec4(colorText.rgb, alpha );
    }
    else
    {
        color =  text1 * vec4(fragmentColor, alpha );
    }
}