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
   
    float bordB = 0.965;
    float bordR = 0.975;
    //-----------------------------------
    int numSyb = 0;
    //int text[7] = int[7]( 2, 1, 3, 4, 5, 1, 7);
    //int text[7] = int[7]( 2, 1, 3, 4, 5, 1, 7);
    //int legnText = 2;
    
    //int text[9] = int[9](8, 7, 6, 5, 4, 3, 2, 1, 0);
    //int text[18] = int[18](8, 7, 6, 5, 4, 3, 2, 1, 0, 8, 7, 6, 5, 4, 33, 2, 1, 0);
    int text[27] = int[27](8, 7, 6, 5, 4, 3, 2, 1, 0, 8, 7, 6, 5, 4, 33, 2, 1, 0, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    //int text[7] = int[7]( 6, 5, 4, 3, 2, 1, 0);
    //int text[3] = int[3]( 0, 1, 0);
    //int text[2] = int[2]( 0, 1);
    //int text[1] = int[1]( 2);
    
    int colSymbRow = 7;
    int legnText = text.length();
    
    vec2 result = vec2(0);
    int index =0;
    for( index = 0; index < legnText; index++)
    {
        numSyb = text[index];
        
        float stepNext =  float(index);
        vec2 sempS = fragCoord/iResolution.xy;
        //--- size
        sempS.x *= 5.5;
        sempS.y *= 5.5;
        
        //--- offset X Y
            float sizeofNextSymb = .1426;
            float offsetStartSymb = stepNext * 0.15;
            float stepX = (sizeofNextSymb * float(numSyb)) - offsetStartSymb - 0.01;
            
            //float sizeofNextSymb = .142;
            //float offsetStartSymb = stepNext * 0.151;
            //float stepX = (sizeofNextSymb * float(numSyb)) - offsetStartSymb;
            
        sempS.x += stepX;

        //------------------------- Y
         float nextY =  -.2 * (floor(float(numSyb)/float(colSymbRow)));
        float stepY = -.5 + nextY;
        sempS.y += stepY;
        
        
        //---- border
        float offsetBorder = 0.05 * stepNext * 0.55;
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

    //color = text1.xxxx + text2;
    color = text1.xxxx;
}

