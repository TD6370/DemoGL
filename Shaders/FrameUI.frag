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

bool CheckParam(in float param, in float chekValue)
{
    float correctParam = 0.01;
    return param > chekValue - correctParam && param < chekValue + correctParam;
}

    //y = mod(x,0.5); // x по модулю 0.5
    //y = fract(x); // возвращает дробную часть аргумента
    //y = ceil(x);  // ближайшее целое, большее либо равное x
    //y = floor(x); // ближайшее целое, меньшее либо равное x
    //y = sign(x);  // знак x
    //y = abs(x);   // абсолютное значение x
    //y = clamp(x,0.0,1.0); // ограничение x промежутком от 0.0 до 1.0
    //y = min(0.0,x);   // меньшее из x и 0.0
    //y = max(0.0,x);   // большее из x и 0.0 

// ---------------- FRAME
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
    vec2 uv = UV;
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
    float smA = 0.58;
    float smB = 1.55;

  	float y =  smoothstep(smSt, smEn, uv.y + smA);
    float x =  smoothstep(smSt, smEn, uv.x + smA);
    float x1 = smoothstep(smSt, smEn, smB - uv.x);
    float y1 = smoothstep(smSt, smEn, smB - uv.y);

    float al;
    //float hor = x * x1;
    //float ver = y * y1;
    float korrUV = 3.5;
    float hor = (x * x1) * korrUV;
    float ver = (y * y1) * korrUV;

    al = ver * hor;
    float vh = ver * hor;
    float kvad;
    float romb;
    //kvad = 0.64 - (fract(ver * 1.8)  * fract(hor * 1.8));
    //romb = 0.41 - fract(sin(ver * 3.)*cos(hor * 1.5));
    float circle = ((ver * hor)-0.5);
    float korrBordBox = 1.;
    float korrBordBox2 = 1.;

    kvad = ( fract(ver * .7)  * fract(hor * .7) ) + .5;
    romb = 1.5 - fract(sin(ver * 2.) * cos(hor * 1.));

    //----- Test
    //al = UV.y;
    //----------------------------  X
    	//al = clamp(vh ,0.1,.7);
        //al*=1.5;
    //----------------------------
    	// al = clamp(ver,0.,.8)  * clamp(hor,0.,.8);
        // al*=1.5;
    //---------------------------- V
        //korrBordBox = .7;
        //al =  1. - (fract(ver * korrBordBox)  * fract(hor * korrBordBox));
    //---------------------------- V
        // korrBordBox = .7;
        // al =  (fract(ver * korrBordBox)  * fract(hor * korrBordBox));
        // al += 0.7;
//--------------    V
        // float korrBordBox = 0.38;//krest
        // al = 1. - (fract(ver * korrBordBox)  * fract(hor * korrBordBox));
//-------------- krest + circle     V
        // float korrBordBox = 0.379;//krest
        // al =.1 - (fract(ver * korrBordBox)  * fract(hor * korrBordBox));
        // al *= 5.5;
        // al = al + circle * 1.;
        // al += 0.4;
    //---------------------------- V
        // kvad = ( fract(ver * .7)  * fract(hor * .7) ) + .5;
        // romb = 1.5 - fract(sin(ver * 2.) * cos(hor * 1.));
        // al = (romb * kvad);
    //----------------------------  V
        korrBordBox =.1;
        al = (kvad-0.5) + ((vh*0.5)-0.5);
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
    if(CheckParam(fragParamCase, m_startResizeParamShaderID))   //= 3 Rsize
    {
        //alpha = al;
    } else if(CheckParam(fragParamCase,m_startMoveParamShaderID))   //= 2    Move
    {
        //alpha = al;
    } else if(CheckParam(fragParamCase,m_startClickParamShaderID))   //= 5   Click (Work)
    {
        //alpha = al;
    } else if(CheckParam(fragParamCase,m_startCheckBorderParamShaderID))   //= 4 Border
    {
        //alpha = al;
    } else if(CheckParam(fragParamCase,m_startFocusParamShaderID))   //= 1   Focus
    {
        alpha = al;
    } else if(CheckParam(fragParamCase,m_startDefaultParamShaderID))   //= 0     Default
    {
        //alpha = al;
    }   

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
    //vec4 text1 = vec4(texture( textureSampler, result ).rgb, 1.0 );	
    //-------------------------------------------------
    vec4 textureBase = vec4(texture( textureSampler, result ).rgb, 1.0 );	
    //vec3 startColor = fragmentColor * vec3(0.5);
    vec3 startColor = fragmentColor;
    vec4 colorText = vec4(startColor, 1.);

    //--------- Color + Mask
    vec4 text = vec4(1.);
    float contrastColorM = 0.3;
    //float contrastColor = 0.5;
    //float dark = (1. - alpha) * contrastColor;
    float darkM = (1. - alpha) * contrastColorM;
    
//==================
    alpha = clamp(alpha,0.0,1.0);
    //alpha = min(1.0, alpha);
    //alpha = max(0.0, alpha);

    //======================================================
    //bool isTest = true;
    bool isTest = false;
    // if(!isTest)
    // {
        //======================================================
        text.r = textureBase.r - (.5 - alpha/2.);
        text.g = textureBase.g - (.5 - alpha/2.);
        text.b = textureBase.b - (.5 - alpha/2.);

        //-- low color 
        //text.r = max(text.r, startColor.r * dark);
        //text.g = max(text.g, startColor.g * dark);
        //text.b = max(text.b ,startColor.b * dark); 
        //-- low color 
        text.r = max(text.r, startColor.r - darkM);
        text.g = max(text.g, startColor.g - darkM);
        text.b = max(text.b ,startColor.b - darkM);
        //-- original color
        // text.r = max(text.r, startColor.r);
        // text.g = max(text.g, startColor.g);
        // text.b = max(text.b ,startColor.b);
    
        // text.r = clamp(text.r, text.r, textureBase.r);
        // text.g = clamp(text.g, text.g, textureBase.g);
        // text.b = clamp(text.b, text.b, textureBase.b);

        text.r = min(textureBase.r, text.r);
        text.g = min(textureBase.g, text.g);
        text.b = min(textureBase.b, text.b);
        //======================================================
    // }
    // else
    // {
    //     text.r = textureBase.r * alpha;
    //     text.g = textureBase.g * alpha;
    //     text.b = textureBase.b * alpha;
    // }
    //==================
    //text.r = alpha;
    //text.g = alpha;
    //text.b = alpha;
    //==================

    color =  text;
}
// ---------------- FRAME