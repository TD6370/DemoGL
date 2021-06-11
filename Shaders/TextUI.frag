#version 330 core

in vec3 fragmentColor;
in vec2 UV;
in float fragParamCase;
in float fragTime;
//in int message[50];

out vec4 color;

uniform sampler2D textureSampler;

vec4 SDF(in vec4 inColor )
{
    const float threshold = .21;
    vec3 c = inColor.xxx;
	//vec3 res = smoothstep(.5-threshold, .5+threshold, c);
    vec3 res = smoothstep(.3-threshold, .3+threshold, c);
	return vec4(res,1.0);
}

void main()
{
    float alpha = 1.0f;
    float bordB = 0.97;
    float bordR = 0.975;
    //float bordB = 0.01;
    //float bordR = 0.5;
    vec2 uv = UV;

    //-----------------------------------
    int numSyb = 0;
    int text[27] = int[27](14, 15, 8, 2, 5, 17, 33, 11, 8, 15, 33, 33,  33,  12,  18,  33, 8,  33, 6, 13, 14, 0, 33, 33, 33, 33, 33);
    //int text[27] = int[27](0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 29, 30);
    //int text[50] = message;
    
    int colSymbRow = 7;
    int legnText = text.length();
    
    vec2 result = vec2(0);
    int index =0;
    for( index = 0; index < legnText; index++)
    {
        numSyb = text[index];
        
        float stepNext =  float(index);
        vec2 sempS = uv;
        //--- Size
        sempS.x *= 5.5;
        sempS.y *= 5.5;
        
        //--- offset X
        float sizeofNextSymb = .1426;
        float offsetStartSymb = stepNext * 0.15;
        float stepX = (sizeofNextSymb * float(numSyb)) - offsetStartSymb - 0.01;
        sempS.x += stepX;

        //--- offset Y
        float nextY =  -.145 * (floor(float(numSyb)/float(colSymbRow)));
        float stepY = -.475 + nextY;

        sempS.y += stepY;
        
        //---- border
        float offsetBorder = stepNext * 0.55 * 0.05;
        vec2 bl = vec2(step(0.0,uv.x - offsetBorder) , step(bordB,uv.y));       // bottom-left
        vec2 tr = vec2(step(bordR - offsetBorder,1.0-uv.x) , step(0.0,1.0-uv.y));   // top-right

        float brd = bl.x * bl.y * tr.x * tr.y;
        vec2 res = (brd * sempS);
        
        if(result == vec2(0))
            result = res;
        else 
            result +=res;
    }
    
    vec4 text1 = vec4(texture( textureSampler, result ).rgb, alpha );	
    //vec4 text2 = vec4(texture( textureSampler2, UV ).rgb, alpha );

    //TEST
    //vec3 colorTest = vec3(1.0,1.0,0.);
    //vec4 colorTest = fragmentColor;

    //vec4 colorText = vec4(0.984,0.773,0.196,1.);
    vec4 colorText = vec4(fragmentColor,alpha);
    //vec4 colorText = vec4(vec3(1.0,1.0,0.),alpha);
    //TEST
    //vec4 colorText = vec4(colorTest, alpha);

    text1 =SDF(text1);

    //float limit = 0.3;
    //if(text1.r < limit)
    //    fragColor = text2;
    //else
        color =  text1 * colorText;	

    //TEST
    //color = vec4(texture( textureSampler, uv ).rgb, alpha );	

    //color = text1.xxxx + text2;
    //color = text1.xxxx;


    //color =  vec4(fragmentColor,alpha);
}

