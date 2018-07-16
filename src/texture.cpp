#include "stdAfx.h"
#include "render.h"


static GLenum tga_bpp_mode; // GL_BGR, GL_BGRA
static int tga_xres (0);
static int tga_yres (0);

char* TGA2Buff (const char* f_name);
GLuint LoadTGA (const char* fname, bool filter);


/*
===============================
Texture::Load

===============================
*/
GLuint Texture::Load (const char* fname, bool filter)
{
	return LoadTGA (fname, filter);
}


/*
===============================
Texture::Create

===============================
*/
GLuint Texture::Create (int xres, int yres, void* buff, int format, bool filter) 
{
	GLuint tx;
	glGetError();

	glGenTextures ( 1, &tx );
	glBindTexture ( GL_TEXTURE_2D, tx );

	//	Установка мип уровней. Макс и мин, соответственно
#ifdef WIN32
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0 );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
#endif
	            
	//  Фильтрация
	if (filter) 
	{
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	}
	else 
	{
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	}
	
	//  Врапинг текстуры
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


	//	Выделим память и забьем данные. если есть
	//	Выравнивание на 1 байт
	if (format == GL_RGB) 
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, xres, yres, 0, GL_RGB,  GL_UNSIGNED_BYTE, buff );
	}
	
	if (format == GL_RGBA) 
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, xres, yres, 0, GL_RGBA,  GL_UNSIGNED_BYTE,  buff );
	}

	if (format == GL_LUMINANCE) 
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
		#ifdef WIN32	
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_R8, xres, yres, 0, GL_RED,  GL_UNSIGNED_BYTE, buff );
		#else	
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_LUMINANCE, xres, yres, 0, GL_LUMINANCE,  GL_UNSIGNED_BYTE, buff );
		#endif
	}
	glBindTexture ( GL_TEXTURE_2D, 0 );

	GLenum err = glGetError();
	if ( err != GL_NO_ERROR ) ER ("create 2D Texture. %x \n", err );
	return tx;
}





/*
===============================
LoadTGA

===============================
*/
GLuint LoadTGA (const char* fname, bool filter) 
{
	GLuint txt(0);
	GLenum err = glGetError();
	char * buff = TGA2Buff (fname);

	if (!buff) 
	{
		ER ("LoadTGA. File not found [ %s ]", fname);
		return Assets::whiteTex;
	}

	glGenTextures (1, &txt);
	glBindTexture ( GL_TEXTURE_2D, txt );

	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (filter) 
	{
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else 
	{
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	}

	if (tga_bpp_mode == 32) 
	{
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, tga_xres, tga_yres, 0, GL_RGBA,  GL_UNSIGNED_BYTE, (void*) (4 + buff) );
	} 
	else if (tga_bpp_mode == 24) 
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, tga_xres, tga_yres, 0, GL_RGB,  GL_UNSIGNED_BYTE, (void*) (4 + buff) );
	}

	glBindTexture ( GL_TEXTURE_2D, 0);

	//	Удалить буфер TGA
	delete [] buff;

	err = glGetError(); 
	if ( err != GL_NO_ERROR ) 
		ER ("create 2D Texture. __LoadTGA. %x \n", err);
	return txt;
}


/*
===============================
TGA reader

===============================
*/
static struct tga_header_t 
{
	GLbyte			descSize; //this version always NULL e.g no description
	GLbyte			isPalette;
	GLbyte			imageType;	// 1 - is palette. 2 - TrueColor. 3 - monochrome
	GLbyte			paletteInfo[5];
	GLshort			startX;
	GLshort			startY;
	GLshort			sizeX;
	GLshort			sizeY;
	GLbyte			bpp;
	GLbyte			description; // nil
} tgaHDR; // total 18


/*
===============================
TGA2Buff

===============================
*/
char* TGA2Buff (const char* fname) 
{
	FILE* f = fopen (FNAME(fname), "rb");
	if (!f)
	{
		ER ("TGA2Buff: can't open file [ %s ]", fname);
		return 0;
	}

	fread (&tgaHDR, sizeof (tga_header_t), 1, f);

	int xr = tgaHDR.sizeX;
	int yr = tgaHDR.sizeY;
	
	if (xr*yr>256*256)
	{
		ER ("TGA2Buff> size limit 256^2");
		return 0;
	}

	tga_bpp_mode = tgaHDR.bpp;
	tga_xres = tgaHDR.sizeX;
	tga_yres = tgaHDR.sizeY;

	char* bf = new char [tgaHDR.sizeX * tgaHDR.sizeY * tgaHDR.bpp/8];
	fread (bf, sizeof(char), tgaHDR.sizeX * tgaHDR.sizeY * tgaHDR.bpp/8, f);

	//	swap channel B<->R
	char* tf = bf;
	int g = tgaHDR.bpp/8;
	for (int i=0; i<xr * yr; i++) 
	{
		char a = tf [i*g + 0];
		tf [i*g + 0] = tf [i*g + 2];
		tf [i*g + 2] = a;
	}

	fclose (f);
	return  bf;
}