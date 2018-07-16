#include "stdAfx.h"
#include "render.h"

void CL_Start ();
void CL_Frame ();

/*
===============================

===============================
*/
void TI_Start ()
{
	ShaderLoadAll ();
	R_UtilLoader ();

	CL_Start ();
}

/*
===============================

===============================
*/
void TI_Frame ()
{
	glClearColor (0.1, 0.12, 0.10, 0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CL_Frame ();
}