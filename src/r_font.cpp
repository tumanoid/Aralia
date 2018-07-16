#include "stdAfx.h"
#include "render.h"

struct IText
{
	vec4		color;
	
	virtual void SetPos (const vec2& p) = 0;
	virtual vec2 GetPos () = 0;

///	возвращает длину строки в пикселях
	virtual int SetText (const char* buff, const float& _size) = 0;
	
	virtual int GetPixelLen () = 0;
	
	virtual void SetColor (const vec4& color) = 0;
	virtual ~IText ();
};

#define MAX_STRING_LEN		256

GLushort pos [2 * MAX_STRING_LEN] = {0};
GLubyte  uv  [4 * MAX_STRING_LEN] = {0};
uchar smallPixel [256] = {11,22,11,11,11,11,11,11,11,128,0,11,11,0,11,11,11,11,11,11,11,11,11,11,11,11,11,11,0,0,0,0,12,6,12,18,18,18,18,6,10,10,16,14,8,14,6,18,18,14,18,18,18,18,18,18,18,18,6,8,12,14,12,18,18,18,18,18,18,18,18,18,18,10,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,10,18,10,16,18,10,16,16,16,16,16,14,16,16,6,12,16,6,18,16,16,16,16,16,16,14,16,16,18,16,16,16,12,6,12,16,9,18,18,8,16,16,18,14,14,18,18,18,18,18,18,18,18,18,8,8,16,16,6,14,18,0,18,18,12,18,16,18,16,20,18,16,18,12,18,6,14,18,18,18,18,18,14,18,14,12,14,10,8,18,18,18,6,16,18,16,18,12,18,16,14,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,16,16,16,16,16,16,18,16,18,16,16,16,18,16,16,16,16,16,14,16,18,16,18,16,18,18,18,18,18,16,18,16};

static Shader* shader(0);
static GLuint fontTex(0);

struct GUIText : IText
{
	GLuint		vao;
	GLuint		vboPOS;
	GLuint		vboUV;
	
	vec2		ppos;
	vec2		init_pos;
	
	int			ln;
	float		size; // размер плашки
	bool		isVisible;
	
	int			pixelLen;
	
	int			halign; // выравнивание. левое(-1), центр(0), правое(1)
	float	    hln;     // длина строки в пикселях
	
	GUIText () 
	{
		isVisible = true;
		color = vec4(1, 1, 1, 1);
		
		ppos = init_pos = vec2(0, 0);
		
		size = 16;
		ln	 = 0;
		halign = 0;
		#ifdef WIN32
		
		//  VAO
		glGenVertexArrays ( 1, &vao );
		glBindVertexArray ( vao ); 

		//	VBO pos
		glGenBuffers ( 1, &vboPOS );
		glBindBuffer ( GL_ARRAY_BUFFER, vboPOS );
		glBufferData ( GL_ARRAY_BUFFER, 2 * sizeof(GLushort) * MAX_STRING_LEN, 0, GL_DYNAMIC_DRAW );
		
		glVertexAttribPointer (0, 2, GL_UNSIGNED_SHORT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray (0);

		//	VBO uv
		glGenBuffers ( 1, &vboUV );
		glBindBuffer ( GL_ARRAY_BUFFER, vboUV );
		glBufferData ( GL_ARRAY_BUFFER, 2 * sizeof (GLbyte) * MAX_STRING_LEN, 0, GL_DYNAMIC_DRAW );

		glVertexAttribPointer (2, 2, GL_BYTE, GL_FALSE, 0, 0);
		glEnableVertexAttribArray (2);

		// Unbind
		glBindVertexArray (0);
		glBindBuffer (GL_ARRAY_BUFFER, 0);    

		//	!!!
		glEnable (GL_PROGRAM_POINT_SIZE);

		#else
		//	VBO pos
		glGenBuffers ( 1, &vboPOS );
		glBindBuffer ( GL_ARRAY_BUFFER, vboPOS );
		glBufferData ( GL_ARRAY_BUFFER, 2 * sizeof(GLushort) * MAX_STRING_LEN, 0, GL_DYNAMIC_DRAW );

		//	VBO uv
		glGenBuffers ( 1, &vboUV );
		glBindBuffer ( GL_ARRAY_BUFFER, vboUV );
		glBufferData ( GL_ARRAY_BUFFER, 2 * sizeof (GLbyte) * MAX_STRING_LEN, 0, GL_DYNAMIC_DRAW );

		// Unbind
		glBindBuffer (GL_ARRAY_BUFFER, 0);    

		#endif		
	}
	
	virtual void SetVisible (const bool& vis)
	{
		isVisible = vis;
	}
	
	~GUIText () 
	{
		glDeleteBuffers (1, &vboPOS);
		glDeleteBuffers (1, &vboUV);
		#ifdef WIN32
			glDeleteVertexArrays (1, &vao);
		#endif
	}
	
	virtual vec2 GetPos ()
	{
		return init_pos;
	}
	
	virtual void SetPos (const vec2& p) 
	{
		ppos = init_pos = p;		
		if (halign == 1)
			ppos.x -= hln / XRES;
		if (halign == 0)
			ppos.x -= 0.5 * hln / XRES;
	}
	
	
	virtual void SetColor (const vec4& clr) 
	{
		color = clr;
	}
	

//	установить текст
	virtual int SetText (const char* buff, const float& _size) 
	{
		size = _size;
		ln	 = 0;

		float font_stride = 0;

		while (1) {		
			if (buff [ln] == '\0')
				break;
				
			int char_code = ((unsigned int) buff [ln]) & 0x000000ff;
			int row = 15  - char_code / 16;		// 16 - номер строки
			int col = char_code % 16; // 16 - номер столбца

			//if (font_stride == 0)
			//font_stride += 1 * (float)smallPixel [char_code] * size/32.0;
			//font_stride += 0.5 * smallPixel [char_code] * size/32.0;

		//	32 - размер в текстуре. 512/16
			pos [ln*2 + 0] = (GLushort) font_stride;
			pos [ln*2 + 1] = 0; // убрать или нет, для спецэффектов?
			
			uv [ln*2 + 0] = (GLubyte) col;
			uv [ln*2 + 1] = (GLubyte) row;

		//	2 - нужно прибавить для stroke вокруг букв, чтоб не наезжали
			//font_stride += 2.0 + 2.0 * smallPixel [char_code] * size/32.0;
			font_stride += 0.0 + (float)smallPixel [char_code] * size/32.0;
			
			ln++;
		}
		glBindBuffer (GL_ARRAY_BUFFER, vboPOS);
		glBufferSubData (GL_ARRAY_BUFFER, 0, ln * 2 * sizeof(GLushort), pos);

		glBindBuffer (GL_ARRAY_BUFFER, vboUV);
		glBufferSubData (GL_ARRAY_BUFFER, 0, ln * 2 * sizeof(GLubyte), uv);
		
		hln = font_stride;
		return (int) hln;
	}
	
	virtual int GetPixelLen ()
	{
		return pixelLen;
	}
	

	virtual void Draw () 
	{	
		if (!isVisible)
			return;
			
		glUseProgram (shader->sh);
		shader->SetTexture (fontTex, shader->mainTex, 0);

		glUniform1f (glGetUniformLocation (shader->sh, "size"), size);
		
	//	нормализованные координаты
		glUniform2fv (glGetUniformLocation (shader->sh, "ppos"), 1, ppos._array);
		glUniform4fv (glGetUniformLocation (shader->sh, "varRes"), 1, vec4 (XRES, YRES, 800, 480)._array);
		
		glUniform4fv (shader->mainColor, 1, color._array);
		
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
	#ifdef WIN32
		glBindVertexArray (vao); 
		glDrawArrays (GL_POINTS, 0, ln);
		glBindVertexArray (0);
	#else
		//	VBO uv
		glBindBuffer ( GL_ARRAY_BUFFER, vboPOS );
		glVertexAttribPointer (0, 2, GL_UNSIGNED_SHORT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray (0);

		//	VBO uv
		glBindBuffer ( GL_ARRAY_BUFFER, vboUV );
		glVertexAttribPointer (2, 2, GL_BYTE, GL_FALSE, 0, 0);
		glEnableVertexAttribArray (2);

		glDrawArrays (GL_POINTS, 0, ln);
	#endif	
	}
};


void InitFontSystem () 
{
	smallPixel [135] += 6;
	//fontTex = Assets::GetTex ("small_pixel512", false);


	shader = Shader::Find ("guiFont.shader");
}