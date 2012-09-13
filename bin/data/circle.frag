#version 120
#extension GL_ARB_texture_rectangle : enable    

uniform sampler2DRect src_tex;
uniform float halfOutputImageWidth;
uniform float halfOutputImageHeight;
//uniform float maskRadius;
//uniform float maskBlendDist;

void main()  
{  

	//float halfOutputImageWidth = 620/2;
	//float halfOutputImageHeight = 460/2;
	
	float x = (gl_TexCoord[0].x - halfOutputImageWidth) / halfOutputImageWidth;
	float y = (gl_TexCoord[0].y - halfOutputImageHeight) / halfOutputImageHeight;
	float xt = (x * 1/sqrt(1 - .5 * y * y));
	float yt = (y * 1/sqrt(1 - .5 * x * x));
	float it = (halfOutputImageWidth * xt + halfOutputImageWidth);
    float jt = (halfOutputImageHeight * yt + halfOutputImageHeight);
	
	//float dist = (halfOutputImageWidth-gl_TexCoord[0].x)*(halfOutputImageWidth-gl_TexCoord[0].x)+
	//(halfOutputImageHeight-gl_TexCoord[0].y)*(halfOutputImageHeight-gl_TexCoord[0].y);
	
	//dist = sqrt(dist);
	
	//if(dist > 240) gl_FragColor = vec4(0,0,0,1);
	//if(it > (halfOutputImageWidth-20) ) gl_FragColor = vec4(0,0,0,1);
	// if(it < 20 ) gl_FragColor = vec4(0,0,0,1);
	//else if(jt < 20 ) gl_FragColor = vec4(0,0,0,1);
	//else if(jt > (halfOutputImageHeight-20) ) gl_FragColor = vec4(0,0,0,1);
	//else{
	
	vec2 pos = vec2(it,jt); 
	vec4 src0 = texture2DRect(src_tex, pos);
	gl_FragColor = src0;
	
	//}
	
} 