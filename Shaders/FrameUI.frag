#version 330 core

in vec3 fragmentColor;
in vec2 UV;
in float fragParamCase;
in float fragTime;
in float fragWidth;
in float fragHeight;
	
    float m_startDefaultParamShaderID = 0.;     //= 0
	float m_startFocusParamShaderID = 1.;       //= 1
	float m_startMoveParamShaderID = 2.;        //= 2
	float m_startResizeParamShaderID = 3.;      //= 3
	float m_startCheckBorderParamShaderID = 4.; //= 4
	float m_startClickParamShaderID = 5.;       //= 5

    bool isDefaultParam = false; 
	bool isFocusParam = false; 
	bool isMoveParamS = false; 
	bool isResizeParam = false; 
	bool isCheckBorderParam = false; 
	bool isClickParam = false; 

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

float KrestCircleBorder(in float al, in float ver, in float hor)
{
    //-------------- krest + circle     V
    float circle = ((ver * hor)-0.5);
    float korrBordBox = 0.379;    //krest
    al =.1 - (fract(ver * korrBordBox)  * fract(hor * korrBordBox));
    al *= 5.5;
    al = al + circle * 1.;
    al += 0.4;
    return al;
}

float AngleSharpBorder(in vec2 uv)
{
   
    float smSt, smEn, smA, smB, y, x, x1, y1, hor, ver, vh, kvad, al;
    smSt = 0.56; //0.555; 
    smEn = 0.75; // 0.7; //size
    smA = 0.65; //0.55; //repeat aut +
    smB = 1.65; //1.55; //repeat aut +
    y =  smoothstep(smSt, smEn, uv.y + smA);
    x =  smoothstep(smSt, smEn, uv.x + smA);
    x1 = smoothstep(smSt, smEn, smB - uv.x);
    y1 = smoothstep(smSt, smEn, smB - uv.y);

    hor = (x * x1); 
    ver = (y * y1); 
    vh = x * x1 * y * y1;
    float repair = 1.5555 * 3.; //.7;
    float cotrast = .35; //.42;//.45;
    kvad = ( fract(ver * repair)  * fract(hor * repair) ) + cotrast;
    float bordBase = 0.85; //0.9;
    al = kvad * (vh + bordBase);
    return al;
} 

float ChessBoardBase(vec2 uv)
{
    float xOffset = step(fract(uv.y), 0.5) * 0.5;
    return step(fract(uv.x + xOffset), 0.5);
}

float ChessBoardBase2(vec2 uv)
{
    vec2 bl = step(vec2(0,0.5), uv);
    vec2 bl2 = step(vec2(0.5,0), uv);
    float pct = (bl.x * bl.y) + (bl2.x * bl2.y);
    float res = pct/(1. - pct);
    return res;
}

float ChessBoard(in vec2 uv)
{
    float speed = 0.1;
    float anima = fract(fragTime * speed);

    vec2 uv2 = uv;
    float w;
    float h;
    float res;
    
    w = fragWidth  / 2.;
    h = fragHeight / 2.;
    //----------------------normalize
    w/=w*w;
    h*=(1/h);
    //h/=(1./h)*pow(h,2);

//--------- anima ---- 
uv2.x -=anima;
uv2.y +=anima;
//----------------------

    //--- repeat
    float sizeK = 10.;
    float sizeX = (w * sizeK);
    float sizeY = (h * sizeK);
    uv2.x *= sizeX;
    uv2.y *= sizeY;
    uv2 = fract(uv2);
    //---------------------------
    
    //CHESS BOARD 
    res = ChessBoardBase2(uv2);
    //res = ChessBoardBase(uv2);

    return res;
}

vec2 NormalizeSize()
{
    float sizeK = 10.;
    float w;
    float h;
   
    w = fragWidth  / 2.;
    h = fragHeight / 2.;
    //----------------------normalize
    w/=w*w;
    //h*=(1/h);
    
    h/=h*h;

    return vec2(w,h);
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

//======================================

float ChessBoardAnima(vec2 uv, float f, float numLine)
{
    float direct = 1. + (-2. * numLine);
    float anima = fract(fragTime * 2.);

    uv.x *= direct;
    uv.y *= direct;
    
    uv.x -= anima * (1. - f);
    uv.y -= anima * f;
    
    //float xOffset = step(fract(uv.x), 0.5) * 0.5;
    float xOffset = step(fract(uv.x), 0.5) * 0.5;
    float res = step(fract(uv.y + xOffset), 0.5);
    return res;
}

float BorderBord(vec2 uv1, float HorVer, float numLine)
{
    float step2 = (0.1 * (1. - HorVer));
    float step1=  (0.1 * HorVer);
    float stepLine1 = (0.1 * (1. - numLine));
    float stepLine2 =  (0.1 * numLine);
    
    float boardSize = 0.01;//0.005;
    boardSize = 0.005;
    float bord =.1;
    float korrLB = 0.085;
    float korrRT = 0.985;

    // ========= size correct
    //vec2 size = NormalizeSize();
    //uv1.x *=size.x/1.6;
    //uv1.y *=size.y/1.6;
    //===================
    
    // left
    float lim_blX = uv1.x + step2 + stepLine2 - boardSize + korrLB ;
    //size.x = min(1.,size.x );

    float blX = step(bord, lim_blX) ;
    // bottom
    float lim_blY =uv1.y + step1 + stepLine1 - boardSize + korrLB;
    float blY = step(bord, lim_blY) ;
    // bottom-left
    vec2 bl = vec2(blX , blY);  
    // right
    float lim_rtX = korrRT + step2 - uv1.x + stepLine1 - boardSize;
    float rtX = step(0.0, lim_rtX);
    // top
    float lim_rtY = korrRT + step1 + stepLine2 - uv1.y - boardSize;
    float rtY = step(0.0, lim_rtY);
    // top-right
    vec2 tr = vec2(rtX , rtY);   
    
    float brd = bl.x * bl.y * tr.x * tr.y;
    return brd;
}

float BorderSnake(vec2 uv)
{
    //--- repeat
    vec2 st = uv;
    float sizeK = 20.;

    vec2 size = NormalizeSize();
    st.x *= size.x;
    st.y *= size.y;


    st.x *= sizeK;
    st.y *= sizeK;
    st = fract(st);
    
    //----  border  ----------------------- 
    float brd = BorderBord(uv, 1., 0.);
    float brd1 = BorderBord(uv, 1., 1.);
    float brd2 = BorderBord(uv, 0., 0.);
    float brd3 = BorderBord(uv, 0., 1.);
         
    //----  ChessBoard  -----------------------
    float color = ChessBoardAnima(st, 1., 0.);
    color += brd;
    float color2 = ChessBoardAnima(st, 0., 0.);
    color2 +=brd2;
    float color1 = ChessBoardAnima(st, 1., 1.);
    color1 +=brd1;
    float color3 = ChessBoardAnima(st, 0., 1.);
    color3 +=brd3;

    color = color * color1 * color2 * color3;
    return color;
}

//===========================
const float offset = 1.0 / 300.0;  

vec4 ConvolutionMat3x3(vec2 uv)
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    // float kernel[9] = float[](
    //     -1, -1, -1,
    //     -1,  9, -1,
    //     -1, -1, -1
    // );
    //  float kernel[9] = float[](
    //     0, 1, 0,
    //     1, -3, 1,
    //     0, 1, 0
    // );
    float kernel[9] = float[](
        0.0625,  0.125,  0.0625,
        0.125,   0.25,   0.125,
        0.0625,  0.125,  0.0625
    );

    //--- Anima
    vec2 size = NormalizeSize();
    //uv.x*=size.x;
    //uv.y*=size.y;
    float anima = fract(fragTime * 0.5);
    //uv.x*=anima;
    //uv.y*=anima;
    //uv.x+=anima;
    //uv.y+=anima;

    float a = anima;
    //a = cos(uv.x) * sin(uv.y);// * (1./size.y);
    a *=1./anima;
    // float kernel[9] = float[](
    //     -a, -a, -a,
    //     -a,  9, -a,
    //     -a, -a, -a
    // );

    //-----
    vec3 col = vec3(0.0);
    vec3 sampleTex[9];

    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(textureSampler,uv + offsets[i]));
    }
    //-----
    // vec3 col = vec3(0.0);
    // for(int i = 0; i < 9; i++)
    //     col += sampleTex[i] * kernel[i];
    //-----
    // kernel[9] = float[](
    //     sampleTex[0],  sampleTex[1],  sampleTex[2],
    //     sampleTex[3],  sampleTex[4],  sampleTex[5],
    //     sampleTex[6],  sampleTex[7],  sampleTex[8]
    // );

    // kernel[9] = float[9](
    //     sampleTex[0].r,  sampleTex[1].r,  sampleTex[2].r,
    //     sampleTex[0].r,  sampleTex[1].r,  sampleTex[2].r,
    //     sampleTex[3].r,  sampleTex[4].r,  sampleTex[8].r
    // );

    col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
 
    return vec4(col, 1.0);
}  

//--------- BloomZoom
vec4 BloomZoom(in vec2 uv) {

    vec3 color = vec3(0.0);
    vec2 toCenter = vec2(.5) - uv;
    float offset = 1.;
    float maxIter = 5.;
    for (float t = 0.0; t <= maxIter; t++) {
        float percent = (t + offset) / 20.0;
        //float percent = (t + offset) / fract(iTime*0.1);
        //color += texture(iChannel0, uv + toCenter * percent).rgb;
        color += vec3(texture(textureSampler,uv + toCenter * percent).rgb);
    }
    return vec4(color / maxIter, 1.);
}


//======================

// ---------------- FRAME
void main()
{
     //-----------   PARAM  --------------------------------------
    if(CheckParam(fragParamCase, m_startResizeParamShaderID))   //= 3 Rsize
    {
        isResizeParam = true; 
        //TETS
        //isCheckBorderParam = true; 

    } else if(CheckParam(fragParamCase,m_startMoveParamShaderID))   //= 2    Move
    {
        isMoveParamS = true; 
    } else if(CheckParam(fragParamCase,m_startClickParamShaderID))   //= 5   Click (Work)
    {
        isClickParam = true; 
    } else if(CheckParam(fragParamCase,m_startCheckBorderParamShaderID))   //= 4 Border
    {
        isCheckBorderParam = true; 
    } else if(CheckParam(fragParamCase,m_startFocusParamShaderID))   //= 1   Focus
    {
        isFocusParam = true; 
        //TEST
        //isResizeParam = true;
         //isCheckBorderParam = true; 
         isMoveParamS = true; 
    } else if(CheckParam(fragParamCase,m_startDefaultParamShaderID))   //= 0     Default
    {
        isDefaultParam = true; 
    }   
    //     isResizeParam = true; 
    //     isMoveParamS = true; 
    //   isClickParam = true; 
    //   isCheckBorderParam = true; 
    //   isFocusParam = true; 
    //   isDefaultParam = true; 
    //-----------------------------
    float alpha = 1.0f;
    float bord = 0.01;
    float bordB = bord;
    float bordR = bord;
    float bordT = bord;
    float bordL = bord;
    vec2 result = vec2(0);
    int index =0;
    vec2 uv = UV;
    float sizeX = fragWidth;
    float sizeY = fragHeight;
    float contrastColor = 0.3;
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

    //-----------------------------
    //---- standart
    // vec2 bl = vec2(smoothstep(smSt, smEn,uv.x) , smoothstep(smSt, smEn,uv.y));       // bottom-left
    // vec2 tr = vec2(smoothstep(smSt, smEn,1.0-uv.x) , smoothstep(smSt, smEn,1.0-uv.y));   // top-right
    // float brd = bl.x * bl.y * tr.x * tr.y;
    // float x_1 = smoothstep(smSt, smEn,uv.x);
    // float y_1 = smoothstep(smSt, smEn,uv.y);
    // float x_2 = smoothstep(smSt, smEn,1.0-uv.x)
    // float Y_2 = smoothstep(smSt, smEn,1.0-uv.y)
    //-----------------------------------

  	float y =  smoothstep(smSt, smEn, uv.y + smA);
    float x =  smoothstep(smSt, smEn, uv.x + smA);
    float x1 = smoothstep(smSt, smEn, smB - uv.x);
    float y1 = smoothstep(smSt, smEn, smB - uv.y);

    //-------   Offset size object  ----------------
    // x /= pow(sizeX,sizeX);
    // y /= pow(sizeY,sizeY);
    //TEST -----------------------
    // y =  smoothstep(smSt, smEn, uv.y);
    // x =  smoothstep(smSt, smEn, uv.x);
    // x1 = smoothstep(smSt, smEn, 1.0-uv.x);
    // y1 = smoothstep(smSt, smEn, 1.0-uv.y);
    //x = step(bordL - 0.1,x);
    //y = step(bordL - 0.1,y);
    //-----------------------

    //---------- ANIMA zOrder
    //  y+=(0.5 - x)/2.*cos(fragTime);
    //  x+=(0.5 - y)/2.*cos(fragTime);
    //  y1+=(0.5 - x1)/2.*cos(fragTime);
    //  x1+=(0.5 - y1)/2.*cos(fragTime);
    //------------------------

    bool isTest = true; // false;
    float al;
    float korrUV = 3.5; 

    //----------------  Anima focus
    float speed = 2.5;
    //anima *= - anima; //timeline normaalize
    float anima = cos(fragTime * speed);
    float stepZ = 6.;//5.5;
    float offsetBordA = 11.5;
    if(isFocusParam)
    {
        float animaZ = anima * -anima; //timeline normaalize
        animaZ = animaZ * stepZ;
        korrUV = offsetBordA + animaZ;  
    }
    //-----------------
    
    float hor = (x * x1) * korrUV;
    float ver = (y * y1) * korrUV;
    
    //--- test
    // //float korrUV =1.45;
    // float korrUV =1.25;
    // float hor = (x * (x1+0.04)) + korrUV;
    // float ver = (y * (y1+0.04)) + korrUV;
    //-----------------------

    al = ver * hor;
    float vh = ver * hor;
    
    //------------------
    float kvad = ( fract(ver * .7)  * fract(hor * .7) ) + .5;
    float romb = 1.5 - fract(sin(ver * 2.) * cos(hor * 1.));
    float circle = ((ver * hor)-0.5);
    float korrBordBox = 1.;
    float korrBordBox2 = 1.;

    //----- Test
    //al =  x * x1 * y * y1;

    //al = UV.y;
    //----------------------------  Bord circle
    	//al = clamp(vh ,0.1,.7);
        //al*=1.5;
    //---------------------------- Bord box
    	//al = clamp(ver,0.,.8)  * clamp(hor,0.,.8);
        //al*=1.5;
        //---------------------------- V Plitka
        // korrBordBox = .7;
        // al =  (fract(ver * korrBordBox)  * fract(hor * korrBordBox));
        // al += 0.7;
//--------------    V   Krest
        // korrBordBox = 0.38;
        // al = 1. - (fract(ver * korrBordBox)  * fract(hor * korrBordBox));
//-------------- krest + circle     V
    //if((isFocusParam && !isTest))
    if(isFocusParam)
    {
        al = KrestCircleBorder(al, ver, hor);
    }
    //---------------------------- V   Angle Sharp
    if(isResizeParam)
    {
        //al = AngleSharpBorder(uv);
        al = ChessBoard(uv);
        //-------
        //float an = BorderSnake(uv);
        //al *= an;
        //al = an;
        //-------   Offset size object  ----------------
        //al *= sizeX / sizeY;
        // x /= pow(sizeX,sizeX);
        // y /= pow(sizeY,sizeY);
    }
    //-------------------------
    if(isCheckBorderParam)
    {
        al = BorderSnake(uv);
    }
    
    //----------------------------  V   KrestCircle
        //korrBordBox =.1;
        //al = (kvad-0.5) + ((vh*0.5)-0.5);
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
    if(isResizeParam)   //= 3 Rsize
    {
        alpha = al;
    } else if(isMoveParamS)   //= 2    Move
    {
        //alpha = al;
    } else if(isClickParam)   //= 5   Click (Work)
    {
        //alpha = al;
    } else if(isCheckBorderParam)   //= 4 Border
    {
        alpha = al;
        contrastColor = .9;

    } else if(isFocusParam)   //= 1   Focus
    {
        alpha = al;
    } else if(isDefaultParam)   //= 0     Default
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
    //=------------------------------

    

    //-------------------------------------------------
    //vec4 text1 = vec4(texture( textureSampler, result ).rgb, alpha );	
    //-------------------------------------------------
    //vec4 text1 = vec4(texture( textureSampler, result ).rgb, 1.0 );	
    //-------------------------------------------------
    vec4 textureBase = vec4(texture( textureSampler, result ).rgb, 1.0 );	
    
    //-------------------------------------------------
    if(isMoveParamS)
    {
        //vec2 nextUV = result;
        //vec4 textBlend = vec4(texture( textureSampler, nextUV ).rgb, 1.0 );	    
        //textureBase = ConvolutionMat3x3(result);
        textureBase = BloomZoom(result);
    }   
    //-------------------------------------------------
    vec3 startColor = fragmentColor;
    vec4 colorText = vec4(startColor, 1.);

    //--------- Color + Mask
    vec4 text = vec4(1.);
    float darkM = (1. - alpha) * contrastColor;
    
//==================
    // if(!isResizeParam)
    // {
        alpha = clamp(alpha,0.,1.);
    // }

    //======================================================
    text.r = textureBase.r - (.5 - alpha/2.);
    text.g = textureBase.g - (.5 - alpha/2.);
    text.b = textureBase.b - (.5 - alpha/2.);

// if(!isResizeParam)
// {
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
// }
    // TEST
    //text.rgb = clamp(text.rgb ,textureBase.rgb , startColor.rgb - darkM);
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

    //color = vec4(1.5);
}
// ---------------- FRAME