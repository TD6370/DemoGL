#version 330 core

in vec3 fragmentColor;
in vec2 UV;
in float fragParamCase;
in float fragTime;

out vec4 color;

uniform sampler2D textureSampler;


void main()
{
	float alpha = 1.0f;
    float bordB = 0.1;
    float bordR = 0.1;
    int colSymbRow = 7;
    vec2 result = vec2(0);
    int index =0;

    vec2 uv = UV;
    vec2 sempS = UV;
    
    //---- border
    vec2 bl = vec2(step(0.05,uv.x) , step(bordB,uv.y));       // bottom-left
    vec2 tr = vec2(step(bordR,1.0-uv.x) , step(0.05,1.0-uv.y));   // top-right
    float brd = bl.x * bl.y * tr.x * tr.y;
    result = (brd * sempS);
  
    //anima
    //result.x += cos(result.y*cos(fragTime));
    //result.y +=sin(result.x*sin(fragTime)*.01);
    
    vec4 text1 = vec4(texture( textureSampler, result ).rgb, alpha );	

    //vec4 colorText = vec4(0.984,0.773,0.196,1.);

    color =  text1 * vec4(max(vec3(.2),fragmentColor), alpha );
}