
#version 330 core

in vec3 outColor;
out vec4 fragColor;


void main()
{
	fragColor = vec4(outColor,1.0f);
}

//layout(location = 0) in vec3 Pos;
//layout(location = 1) in float normal;
//
//// you can access components like this, and make new vectors, 
//// called SWIZZLING (you can use, "xyzw" for position, "rgba" for color, and "stpq" for texture coords conventionally and in order.
//
//vec2 someVec;
//vec4 differentVec = someVec.xyxx;
//vec3 anotherVec = differentVec.zyw;
//vec4 otherVec = anotherVec.ssss + anotherVec.grgb;		
//
//vec2 vect = vec2(0.5, 0.7);
//vec4 result = vec4(vect, 0.0, 1.0);
//vec4 otherResult = vec4(result.rgb, 10);
//
uniform vec4 color;