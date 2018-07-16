#include "stdAfx.h"
#include "input.h"
#include "render.h"

GLuint viewTex;


/*
====================================
====================================
*/
void CL_Start ()
{
	char d [128*64];
	for (int i=0; i<128*64; i++)
		d[i] = rnd ()*255;
		
	viewTex = Texture::Create (128, 64, d, GL_LUMINANCE, true);
}


/*
====================================
====================================
*/
void CL_Frame ()
{
	static float ofx(0);
	ofx += deltaTime * 0.15;
	R::DrawSpriteSC (vec4 (0.1, 0.1, 0.9, 0.8), viewTex, vec4(1,1,1,1), vec4(1,1,ofx,0));

	if (tchList[0].stat)
	{
		R::DrawSpriteSC (vec4 (tchList[0].sx, tchList[0].sy, 0.2, 0.2), 0, vec4(0.5,0.5,1,1), vec4(1,1,0,0));
	}
}