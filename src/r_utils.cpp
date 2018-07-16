#include "stdAfx.h"
#include "render.h"

static GLuint vao(0);
static GLuint vbo(0);

static GLubyte vdata [4 * 2] = {0,1,0,0,1,1,1,0};
static Shader* shader_sc;

/*
========================================
Рисует спрайт
ps.x,y - нижний левый. нормализованный
ps.z,w - размер по x, y. нормализованный
========================================
*/
void R::DrawSpriteSC (const vec4 & ps, const GLuint & tx, const vec4 & color, const vec4 & mudUVXX) 
{
	glUseProgram (shader_sc->sh);
	glUniform4fv (glGetUniformLocation (shader_sc->sh, "pssz"), 1, ps._array);
	glUniform4fv (shader_sc->mainColor, 1, color._array);
	glActiveTexture (GL_TEXTURE0);
	
	if (tx) 
		glBindTexture (GL_TEXTURE_2D, tx);
	else
		glBindTexture (GL_TEXTURE_2D, Assets::whiteTex);

	glUniform1i (shader_sc->mainTex, 0);
	glUniform4fv (glGetUniformLocation (shader_sc->sh, "mudUVXX"), 1, mudUVXX._array);

#ifdef WIN32
	glBindVertexArray (vao);
	glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray (0);
#else
	glBindBuffer ( GL_ARRAY_BUFFER, vbo );
	glVertexAttribPointer (0, 2, GL_BYTE, GL_FALSE, 0, 0);
	glEnableVertexAttribArray (0);
	glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
#endif
}


/*
========================================

========================================
*/
static void ResLoader_A1 ()
{
//	дефолтные текстуры
	long f = 0xffffffff;
	Assets::whiteTex = Texture::Create (1, 1, &f);
	f = 0xff808080;
	Assets::grayTex = Texture::Create (1, 1, &f);
	f = 0xffff0000;
	Assets::redTex = Texture::Create (1, 1, &f);

	shader_sc = Shader::Find ("sprite_sc.shader");

#ifdef WIN32
	//  VAO
	glGenVertexArrays ( 1, &vao );
	glBindVertexArray ( vao ); 

	// VBO vertex
	glGenBuffers ( 1, &vbo );
	glBindBuffer ( GL_ARRAY_BUFFER, vbo );
	glBufferData ( GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLubyte), vdata, GL_STATIC_DRAW ); 

	glVertexAttribPointer (0, 2, GL_BYTE, GL_FALSE, 0, 0);
	glEnableVertexAttribArray (0);

	// Unbind
	glBindVertexArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, 0);    
#else
	// VBO vertex
	glGenBuffers ( 1, &vbo );
	glBindBuffer ( GL_ARRAY_BUFFER, vbo );
	glBufferData ( GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLubyte), vdata, GL_STATIC_DRAW );
#endif
}


/*
========================================
Квадрат на весь экран

========================================
*/
static GLfloat sq_data [3 * (3 + 2)] = {-1,-1,0,0,0,3,-1,0,2,0,-1,3,0,0,2};
static GLuint sq_vao(0);
static GLuint sq_vbo(0);

static void ResLoader_A2 ()
{
#ifdef WIN32
	//  VAO
	glGenVertexArrays ( 1, &sq_vao );
	glBindVertexArray ( sq_vao ); 

	// VBO vertex
	glGenBuffers ( 1, &sq_vbo );
	glBindBuffer ( GL_ARRAY_BUFFER, sq_vbo );
	glBufferData ( GL_ARRAY_BUFFER, 3 * (3 + 2) * sizeof(GLfloat), sq_data, GL_STATIC_DRAW ); 

	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, (3+2) * sizeof(float), 0);
	glEnableVertexAttribArray (0);

	glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, (3+2) * sizeof(float), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray (1);

	// Unbind
	glBindVertexArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, 0);    
#else
	glGenBuffers ( 1, &sq_vbo );
	glBindBuffer ( GL_ARRAY_BUFFER, sq_vbo );
	glBufferData ( GL_ARRAY_BUFFER, 3 * (3 + 2) * sizeof(GLfloat), sq_data, GL_STATIC_DRAW ); 
#endif
}


/*
========================================
ScreenQuadDraw

========================================
*/
void R::ScreenQuadDraw ()
{
#ifdef WIN32
	glBindVertexArray (sq_vao); 
	glDrawArrays ( GL_TRIANGLES, 0, 3 ); 
	glBindVertexArray (0);
#else
	glBindBuffer (GL_ARRAY_BUFFER, sq_vbo);

	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, (3+2) * sizeof(float), 0);
	glEnableVertexAttribArray (0);

	glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, (3+2) * sizeof(float), (void*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray (2); 

	glDrawArrays ( GL_TRIANGLES, 0, 3 ); 
#endif
}


/*
========================================
Загрузка данных в видео.

========================================
*/
void R_UtilLoader ()
{
	ResLoader_A1 ();
	ResLoader_A2 ();
}