#include "stdAfx.h"
#include "render.h"

#define MAX_ASSETS		256

GLuint Assets::whiteTex(0);
GLuint Assets::grayTex(0);
GLuint Assets::redTex(0);


struct rec_t 
{
	uint	hss; // (H)a(S)h(S)um
	GLuint	id;
	bool	filter;
};

static int rec_len(0);
static rec_t resList [MAX_ASSETS];


/*
==========================
Assets::GetTex

==========================
*/
GLuint Assets::GetTex (const char* name, bool filter) 
{
	if (rec_len > MAX_ASSETS - 1)
		return Assets::whiteTex;

	uint hss = SFH (name, strlen(name));

//	ищем в уже загруженном	
	for (int i=0; i<rec_len; i++) 
		if (resList [i].hss == hss && resList [i].filter == filter) 
			return resList [i].id;

//	не нашли. загружаем	
	sprintf (::stb, "tts/%s.png", name);
	resList [rec_len].hss		= hss;
	resList [rec_len].filter	= filter;
	resList [rec_len].id		= Texture::Load (::stb, filter);
	rec_len++;

	return resList [rec_len - 1].id;
}


/*
==========================
Assets::AddTex 

добавить текстуру в стопку
например, рендер.
==========================
*/
void Assets::AddTex (const char* name, const GLuint& idNewTexture, const bool & filter)
{
	uint hs = SFH (name, strlen(name));

//	ищем в уже загруженном	
	for (int i=0; i<rec_len; i++) 
		if (resList [i].hss == hs && resList [i].filter == filter) 
		{
			WR ("Assets::AddTex> texture [ %s ] already in asset base. skip it.", name);
			return;
		}	
	
	resList [rec_len].hss = hs;
	resList [rec_len].id = idNewTexture;
	resList [rec_len].filter = filter;
	rec_len++;
}
