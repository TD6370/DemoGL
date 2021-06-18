// Author:
// Title:

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

void main1() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;

    vec3 color = vec3(0.);
    color = vec3(st.x,st.y,abs(sin(u_time)));

    gl_FragColor = vec4(color,1.0);
}

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.5);

  
    
    // bottom-left
    vec2 bl = step(vec2(0.1),st);
    float pct = bl.x * bl.y;

    // top-right
     vec2 tr = step(vec2(0.1),1.0-st);
     //pct *= tr.x * tr.y;
    
    vec2 sm; 
  	float y = smoothstep(0.3,1.,st.y+0.6);
    float x = smoothstep(0.3,1.,st.x+0.6);
    float x1 = smoothstep(0.3,1.,1.6 -st.x);
    float y1 = smoothstep(0.3,1.,1.6 -st.y);

    float al;

    //---
    float hor = x*x1;
    float ver = y*y1;
    al = ver * hor;
    float vh = ver * hor;
    //---
    y = mod(x,0.5); // x по модулю 0.5
	y = fract(x); // возвращает дробную часть аргумента
	y = ceil(x);  // ближайшее целое, большее либо равное x
	y = floor(x); // ближайшее целое, меньшее либо равное x
	y = sign(x);  // знак x
	y = abs(x);   // абсолютное значение x
	y = clamp(x,0.0,1.0); // ограничение x промежутком от 0.0 до 1.0
	y = min(0.0,x);   // меньшее из x и 0.0
	y = max(0.0,x);   // большее из x и 0.0 

    float kvad;
    float romb;
    //----------------------------
    //al = clamp(vh,0.1,.7);
    //----------------------------
    //al = clamp(ver,0.,.8)  * clamp(hor,0.,.8);
    //----------------------------
    //al = fract(ver)  * fract(hor);
    //----------------------------
    //al =1.688 - (fract(ver)  * fract(hor));
    //----------------------------
    //al =0.672 - (fract(ver*1.1)  * fract(hor*1.1));
    //kvad =0.640 - (fract(ver*1.8)  * fract(hor*1.8));
    //----------------------------
    //al =0.584 - (fract(ver*1.564)  * fract(hor*1.708));
    //kvad =0.584 - (fract(ver*1.564)  * fract(hor*1.708));
    //----------------------------
    //al = 0.804 - fract(cos(ver*1.276)*cos(hor*1.308))*3.188;
    //----------------------------
    //al = 0.480 - fract(sin(ver*3.044)*cos(hor*1.500))*7.212;
    //romb = 0.41 - fract(sin(ver*3.)*cos(hor*1.5));
    //al = kvad + romb;
    //----------------------------
    kvad =0.640 - (fract(ver*1.8)  * fract(hor*1.8));
    romb = 0.41 - fract(sin(ver*3.)*cos(hor*1.5));
    //al = romb + kvad;
    //al = romb + kvad+(vh*.6);
    //al = kvad + (vh*1.1);
    //---------------------
    //al = 1.- max(.6, kvad * 1.1);
    //---------------------
    //al = 1.496- max(.6, kvad * 1.1);
    //---------------------
    //al = 1.496- max(.6, kvad * 1.1);
    //----------------------------
    //al = (fract(ver*1.980+vh)  / fract(hor*2.988+vh));
    //al *= vh + 0.184;
    //al = al;
    //----------------------------
    //al = (hor*2.2 -0.7) *  (ver*2.2-0.7);
    //----------------------------
    //float al1 = mod((ver*hor)*2.4,0.7);
    //float al2 = hor;
    //al  = al1;
    //----------------
    //float al1 = ver / hor;
    //float al2 = hor / ver;
    
    //al = (al2*1.5 -0.6) * (al1*1.7-0.7);
        //-------------------
	//-------------------
       
    //color = vec3(pct);

    gl_FragColor = vec4(color,al);
}
