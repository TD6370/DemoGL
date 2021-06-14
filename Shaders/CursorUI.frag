#version 330 core

in vec3 fragmentColor;
in vec2 UV;
in float fragParamCase;
in float fragTime;

out vec4 color;

uniform sampler2D textureSampler;

vec4 SDF1(in vec4 inColor )
{
     float contrast = 40.0;
    vec3 c = inColor.xxx;
	return vec4((c-0.5)*contrast,1.0);
}

vec4 SDF2(in vec4 inColor )
{
    const float threshold = .21;
    vec3 c = inColor.xxx;
    vec3 res = smoothstep(.3-threshold, .3+threshold, c);
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

// ---------------- CURSOR
void main()
{
	//float alpha = 1.0f;
    float alpha = 1.0f;
    float bord = 0.01;
    float bordB = bord;
    float bordR = bord;
    float bordT = bord;
    float bordL = bord;
    int colSymbRow = 7;
    vec2 result = vec2(0);
    int index =0;

    vec2 uv = UV;
    vec2 sempS = UV;
    
    //---- border
    vec2 bl = vec2(step(bordL,uv.x) , step(bordB,uv.y));       // bottom-left
    vec2 tr = vec2(step(bordR,1.0-uv.x) , step(bordT,1.0-uv.y));   // top-right
    float brd = bl.x * bl.y * tr.x * tr.y;
    //result = (brd * sempS);
    result = UV;
  
    //anima
    //result.x += cos(result.y*cos(fragTime));
    //result.y +=sin(result.x*sin(fragTime)*.01);
    
    vec4 text1 = vec4(texture( textureSampler, result ).rgb, alpha );	

    int off = 0;
    vec4 resultAlpha;
    vec4 textA =SDF1(text1);
    vec4 textB =SDF2(text1);
    //resultAlpha = SDF1(text1);
    //resultAlpha = SDF2(text1);
    //resultAlpha = SDF4(text1);
    //----------- GlowEffect
    //resultAlpha = GlowEffect(text1);
    //----------- outline
    //text1 =Outline(text1);
    //---------------------------
    //resultAlpha = textB - textA;
        //resultAlpha = textA - textB;
        resultAlpha = textA + textB;

        //resultAlpha = text1;

    alpha = resultAlpha.r;
    //-----------------
    //vec4 colorText = vec4( fragmentColor * vec3(0.5), alpha);
    //color =  text1; // * colorText;
    color =  vec4(vec3(text1.xyz), alpha);
}
// ---------------- CURSOR