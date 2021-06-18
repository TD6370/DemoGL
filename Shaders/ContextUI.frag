#version 330 core

in vec3 fragmentColor;
in vec2 UV;
in float fragParamCase;
in float fragTime;
	
    float m_startDefaultParamShaderID = 0.;     //= 0
	float m_startFocusParamShaderID = 1.;       //= 1
	float m_startMoveParamShaderID = 2.;        //= 2
	float m_startResizeParamShaderID = 3.;      //= 3
	float m_startCheckBorderParamShaderID = 4.; //= 4
	float m_startClickParamShaderID = 5.;       //= 5

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


// ---------------- CONTEXT
void main()
{
    float alpha = 1.0f;
    float bord = 0.01;
    float bordB = bord;
    float bordR = bord;
    float bordT = bord;
    float bordL = bord;
    vec2 result = vec2(0);
    int index =0;
    //vec2 uv = UV;
    result = UV;

    //------------  BORDERS ------------------------------
    //---- standart
    //vec2 bl = vec2(step(bordL,uv.x) , step(bordB,uv.y));       // bottom-left
    //vec2 tr = vec2(step(bordR,1.0-uv.x) , step(bordT,1.0-uv.y));   // top-right
    //float brd = bl.x * bl.y * tr.x * tr.y;
    //    result = (brd * UV);
    //---- ef 
//------------------------
    float smSt = 0.5; //0.3;
    float smEn = 1.2; //1.0;
    //float smA = 0.6;
    //float smB = 1.5;
    //float smA = 0.5;
        //float smA = 0.49;
        //float smB = 1.49;
    //float smB = 1.5;

    float smA = 0.1;
    float smB = 0.9;

  	//float y =  smoothstep(smSt, smEn, uv.y + smA);
    //float x =  smoothstep(smSt, smEn, uv.x + smA);
    //float x1 = smoothstep(smSt, smEn, smB - uv.x);
    //float y1 = smoothstep(smSt, smEn, smB - uv.y);
    //float y =  smoothstep(smSt, smEn, uv.y);
    //float x =  smoothstep(smSt, smEn, uv.x);
    //float x1 = smoothstep(smSt, smEn, uv.x);
    //float y1 = smoothstep(smSt, smEn, uv.y);
     float y =  smoothstep(smSt, smEn, UV.y);
    float x =  smoothstep(smSt, smEn, UV.x);
    float x1 = smoothstep(smSt, smEn, UV.x);
    float y1 = smoothstep(smSt, smEn, UV.y);

    float al;
    float hor = x * x1;
    float ver = y * y1;
    al = ver * hor;
    float vh = ver * hor;
    float kvad;
    float romb;
    kvad = 0.64 - (fract(ver * 1.8)  * fract(hor * 1.8));
    romb = 0.41 - fract(sin(ver * 3.)*cos(hor * 1.5));

    al = UV.y;
    //----------------------------
    	//al = clamp(vh,0.1,.7);
        //al*=1.5;
    //----------------------------
    	//al = clamp(ver,0.,.8)  * clamp(hor,0.,.8);
        //al*=1.5;
        //al = clamp(ver,0.0,1.8)*.4  * clamp(hor,0.,.9)*.3;
    	//al*=13.1;
    //----------------------------
    	//al =0.584 - (fract(ver*1.564)  * fract(hor*1.708));
        //al =0.58 - (fract(ver*1.5)  * fract(hor*1.708));
     //----------------------------
        //al = romb + kvad;
        //al = romb + kvad+(vh*.6);
        //al = kvad + (vh*1.1);
    //---------------------
    	//al = 1.496- max(.6, kvad * 1.1);
    //----------------------------
    	//al = (hor*2.2 -0.7) *  (ver*2.2-0.7);
    //----------------------------
        //float al1 = ver / hor;
        //float al2 = hor / ver;
        //al = (al2*1.5 -0.6) * (al1*1.7-0.7);
    //-------------------
    //alpha = al;
    
    //---- TEST
    //al = 0.480 - fract(sin(ver*3.044)*cos(hor*1.500))*7.212;


    //-- OFF
    //al = 1.0;
    //-----------   PARAM  --------------------------------------
    if(fragParamCase == m_startResizeParamShaderID)   //= 3 Rsize
    {
        //alpha = al;
    } else if(fragParamCase == m_startMoveParamShaderID)   //= 2    Move
    {
        //alpha = al;
    } else if(fragParamCase == m_startClickParamShaderID)   //= 5   Click (Work)
    {
        //alpha = al;
    } else if(fragParamCase == m_startCheckBorderParamShaderID)   //= 4 Border
    {
        //alpha = al;
    } else if(fragParamCase == m_startFocusParamShaderID)   //= 1   Focus
    {
        //alpha = al;
    } else if(fragParamCase == m_startDefaultParamShaderID)   //= 0     Default
    {
        //alpha = al;
    }   
    //alpha = al;
    //alpha += 0.1;
    //alpha *= 6.2;
    //-----------   AMINA   --------------------------------------
    //
    //if(fragParamCase == m_startClickParamShaderID)
    //{
    //    result.x += cos(result.y*cos(fragTime));
    //    result.y +=sin(result.x*sin(fragTime)*.01);
    //}
    //-------------------------------------------------
    //vec4 text1 = vec4(texture( textureSampler, result ).rgb, alpha );	
    //-------------------------------------------------
    vec4 text1 = vec4(texture( textureSampler, result ).rgb, 1.0 );	
    //text1.r *= alpha;
    //text1.g *= alpha;
    //text1.b *= alpha;
    //text1.r *= alpha;
    //text1.r *= UV.y;
    //text1.g *= UV.y;
    //text1.b *= UV.y;
    //text1.r *= alpha;
    //text1.g *= alpha;
    //text1.b *= alpha;

    // ---------------------- 
    //float limitAlpha = 0.9;
    //if( text1.r > limitAlpha && 
    //   text1.g > limitAlpha && 
    //   text1.b > limitAlpha){
        //--------- SDF
	    //vec4 text2 =SDF1(text1);
        //vec4 text3 =SDF2(text1);
        //text1 = text2 - text3;
        //alpha =0.0;
        //alpha =1. - text1.r;
    //}

    //-----------------
        //vec4 colorText = vec4( fragmentColor * vec3(0.5), alpha);
    //color =  text1 * colorText;
    //-----------------
    //vec4 colorText = vec4( fragmentColor * vec3(0.5), 1.0);
    //color =  text1 * colorText;
    //-----------------
    //vec4 colorText = vec4( fragmentColor * vec3(0.5), 1.0);
    //vec4 colorE =  text1 * colorText;
    //color =  vec4(colorE.xyz, alpha);
    //-----------------
    //color =  vec4(text1.xyz, alpha);
    color =  text1;
}
// ---------------- CONTEXT