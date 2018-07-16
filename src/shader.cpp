#include "stdAfx.h"
#include "render.h"

struct shader_s;
static shader_s* shaderList [32];

/*
===================================
INNER shader_s

===================================
*/
struct shader_s : Shader 
{
	GLuint shVert;
	GLuint shFrag;
	
	char name [64];
	char __customDefs [128]; 
	
	shader_s** slot;

	shader_s () 
	{
		memset (__customDefs, 0, 128);
		
		shVert = 0;
		shFrag = 0;
		Shader::sh = 0;
		slot = 0;
		for (int i=0; i<32; i++) 
		{		
			if (shaderList [i] == 0) 
			{
				shaderList [i] = this;
				slot = shaderList + i;
				break;
			}
		}
		
		if (!slot) 
		{
			ER ("Shader. no free slots");
			return;
		}
		
		strcpy(name, "sh_notbuild");	
	}
	
	
	virtual void SetTexture (const GLuint& texID, const GLuint& indexName, const GLuint& slot) 
	{	
		glActiveTexture (GL_TEXTURE0 + slot);
		glBindTexture (GL_TEXTURE_2D, texID);
		glUniform1i (indexName, slot);
	}
	
	virtual ~shader_s () 
	{	
		glDetachShader (sh, shVert);
		glDetachShader (sh, shFrag);
		
		glDeleteShader (shVert);
		glDeleteShader (shFrag);
		
		glDeleteProgram(sh);
		
		*slot = 0;
	}
};


/*
===================================
поиск по имени

===================================
*/
Shader* Shader::Find (const char* name)
{
	for (int i=0; i<32; i++) 
		if (shaderList [i]) 
			if (!strcmp (name, shaderList [i]->name)) 
				return shaderList [i];
	
	WR("Shader::Find> shader [ %s ] not found", name);
	return shaderList [0];
}


/*
===================================
перезагрузка всех шейдеров	

===================================
*/
static int reloadNum(-1);
	
void Shader::ReloadAll () 
{
	for (int i=0; i<32; i++) 
	{
		if (shaderList [i]) 
		{
			reloadNum = i;
			Shader::Create (shaderList [i]->name);
		}
	}
	reloadNum = -1;
}


void BuildShader (shader_s* retSH, const char* vert, const char* frag, const char* fname);



static char clog_buff [8128];
static char _vert [8128];
static char _frag [8128];
static char fbuff [8128];
static char dbuff [4096]; // дефайны. глобальные + пользовательские


/*
===============================
прозрачность

===============================
*/
static const char* alphaCutTransparent = 
"void TransClip (){mat4 thresholdMatrix = mat4 (1.0 / 17.0,  9.0 / 17.0,  3.0 / 17.0, 11.0 / 17.0, "
"13.0 / 17.0,  5.0 / 17.0, 15.0 / 17.0,  "
"7.0 / 17.0, 4.0 / 17.0, 12.0 / 17.0,  2.0 / 17.0, 10.0 / 17.0, 16.0 / 17.0,  "
"8.0 / 17.0, 14.0 / 17.0,  6.0 / 17.0);"
"mat4 _RowAccess = mat4 ( 1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,1.0 );"
		"float _Transparency = 0.55;"
		"float2 pos = gl_FragCoord.xy;"
		"float4 vc = thresholdMatrix[int(mod(pos.x, 4.0))] * _RowAccess[int(mod(pos.y, 4.0))];"
		"if (_Transparency < vc.x+vc.y+vc.z+vc.w) discard; }";

/*
===============================
создать шейдер

===============================
*/
Shader* Shader::Create (const char* fname, const char* customDefs) 
{
	memset (_vert, 0, 8128);
	memset (_frag, 0, 8128);
	memset (fbuff, 0, 8128);
	memset (dbuff, 0, 4096);

	sprintf (::stb, "shaders/%s", fname);
	FILE * f = fopen (FNAME(::stb), "r");

	if (!f) 
	{
		ER ("SHADER: Can't read file [ %s ]", fname);
		return new shader_s;		
	}
	
//	создать заготовку шейдера. учесть перезагрузку		
	shader_s* retSH(0);
	
	if (reloadNum == -1) 
	{
		retSH = new shader_s;
		strcpy (retSH->name, fname);	
		
		//	сохраним пользовательские дефайны  / первый шаг на пути к убершейдерам ))
		if (customDefs) 
		{
			strcpy (retSH->__customDefs, customDefs);
		}
	}
	else 
	{
		retSH = shaderList [reloadNum];

		//	удалим предыдущие шейдеры из программы
		glDetachShader (retSH->sh, retSH->shVert);
		glDetachShader (retSH->sh, retSH->shFrag);
		
		glDeleteShader (retSH->shVert);
		glDeleteShader (retSH->shFrag);
		
		retSH->shVert = 0;
		retSH->shFrag = 0;
	}		
	
	#ifdef WIN32
		const char* defs =
			"#version 120\n"
			//"#define TRANSPARENT_CLIP\n"
			"#define float2 vec2\n"
			"#define float3 vec3\n"
			"#define float4 vec4\n"
			
			"#define half float\n"
			"#define half2 vec2\n"
			"#define half3 vec3\n"
			"#define half4 vec4\n"
			
			"#define fix float\n"
			"#define fix2 vec2\n"
			"#define fix3 vec3\n"
			"#define fix4 vec4\n";
	#else
		const char* defs =
			"#version 100\n"
			//"#define highp float\n" - нет такого
			"#define float2 highp vec2\n"
			"#define float3 highp vec3\n"
			"#define float4 highp vec4\n"
			
			"#define half mediump float\n"
			"#define half2 mediump vec2\n"
			"#define half3 mediump vec3\n"
			"#define half4 mediump vec4\n"
			
			"#define fix lowp float\n"
			"#define fix2 lowp vec2\n"
			"#define fix3 lowp vec3\n"
			"#define fix4 lowp vec4\n"

			"precision highp float;\n"
			"precision lowp sampler2D;\n";
	#endif
	strncat(dbuff, defs, strlen(defs));
	
	//	добавить еще. это тест
	char addStr [128] = {0};
	
	sprintf (addStr, "#define XRES %i.0\n", XRES);
	strncat (dbuff, addStr, strlen(addStr));

	sprintf (addStr, "#define YRES %i.0\n", YRES);
	strncat (dbuff, addStr, strlen(addStr));
	
	sprintf (addStr, "#define ASPECT %0.2f\n", (float) YRES / XRES);
	strncat (dbuff, addStr, strlen(addStr));
	
	//	пользовательские DEFINES
	strncat (dbuff, retSH->__customDefs, strlen(retSH->__customDefs));
	
//	формируем строки для шейдера		
	char buff [256];
	bool tagVOpen = false;
	bool tagFOpen = false;
	
	while (!feof(f)) 
	{
		fgets (buff, 256, f);
		
		if (!strncmp ("#VERTEX", buff, 7)) 
		{
			//	дефайны
			strncat(_vert, dbuff, strlen (dbuff));
	
			tagVOpen = true;
			tagFOpen = false;
			continue;
		}
		
		if (!strncmp ("#FRAGMENT", buff, 9)) 
		{
			//	дефайны
			strncat (_frag, dbuff, strlen(dbuff));
			strncat (_frag, alphaCutTransparent, strlen(alphaCutTransparent));
			
			tagVOpen = false;
			tagFOpen = true;
			continue;
		}
		
		if (tagVOpen) 
		{
			strncat (_vert, buff, strlen(buff));
		}
		
		if (tagFOpen) 
		{
			strncat (_frag, buff, strlen(buff));
		}
	}
	
	fclose(f);
	
#ifdef __WIN32
	//	посмотреть, что получилось
	FILE* ff = fopen (fname, "w");
	fprintf (ff, "%s // ----------\n %s", _vert, _frag);
	fclose(ff);
#endif
	
//	билд. шейдер		
	BuildShader (retSH, _vert, _frag, fname);
	return retSH;
}


/*
=============================
собрать шейдер

=============================
*/
void BuildShader (shader_s* retSH, const char* vert, const char* frag, const char* fname) 
{
//	вершинный
    retSH->shVert = glCreateShader ( GL_VERTEX_SHADER );          

	glShaderSource ( retSH->shVert, 1, &vert, 0);	
    glCompileShader ( retSH->shVert );
	
//	проверим ошибку
	GLint stat;
	glGetShaderiv ( retSH->shVert, GL_COMPILE_STATUS, &stat );
	
	if (!stat) 
	{
		GLint logLength;
		glGetShaderiv ( retSH->shVert, GL_INFO_LOG_LENGTH, &logLength );
		glGetShaderInfoLog ( retSH->shVert, logLength, NULL, clog_buff );
		WR ("BuildShader>Vertex error: %s", clog_buff );
		retSH->shVert = 0;
		return;
	}
	
//  фрагментный
	retSH->shFrag = glCreateShader (GL_FRAGMENT_SHADER);

	glShaderSource ( retSH->shFrag, 1, &frag, 0);
    glCompileShader ( retSH->shFrag );

//	проверим ошибку
    glGetShaderiv ( retSH->shFrag, GL_COMPILE_STATUS, &stat );
    
	if (!stat) 
	{                    
        GLint logLength;
        glGetShaderiv ( retSH->shFrag, GL_INFO_LOG_LENGTH, &logLength );
        glGetShaderInfoLog ( retSH->shFrag, logLength, NULL, clog_buff );
		WR ("BuildShader>Fragment error: %s", clog_buff );
		
		retSH->shFrag = 0;
		return;
    }
	
	if (reloadNum == -1) 
	{
		retSH->sh = glCreateProgram();
	}

    glAttachShader ( retSH->sh, retSH->shVert );
    glAttachShader ( retSH->sh, retSH->shFrag );    

	//	индексы для атрибутов можно/ НУЖНО. ручками задать ДО ЛИНКОВКИ
	//	бывают такие бл. железки(драйвера), что ты ему здесь все задашь, а он тебе
	// на вопрос glGetAttributeIndex все равно -1 дает. так было на idole 2
    glBindAttribLocation(retSH->sh, 0, "Position");
    glBindAttribLocation(retSH->sh, 1, "Normals");
    glBindAttribLocation(retSH->sh, 2, "UV");
    glBindAttribLocation(retSH->sh, 3, "MAT");
    
    glBindAttribLocation(retSH->sh, 4, "AttAUX2");
    glBindAttribLocation(retSH->sh, 5, "AttAUX3");

    glLinkProgram ( retSH->sh );

	//	проверим как собралось
	glValidateProgram (retSH->sh);	
	glGetProgramiv (retSH->sh, GL_VALIDATE_STATUS, &stat);
	if (stat != GL_TRUE) 
	{
		ER ("BuildShader>Linked. ShaderName [ %s ]", fname);
        glGetProgramInfoLog (retSH->sh, 1024, NULL, clog_buff);
        ER ("BuildShader>Linked log:\n %s", clog_buff );
	}
	
	retSH->M  = glGetUniformLocation ( retSH->sh, "M" );
	retSH->V  = glGetUniformLocation ( retSH->sh, "V" );	
	retSH->P  = glGetUniformLocation ( retSH->sh, "P" );
	retSH->VP  = glGetUniformLocation ( retSH->sh, "VP" );
	retSH->MVP  = glGetUniformLocation ( retSH->sh, "MVP" );
	
	retSH->iV  = glGetUniformLocation ( retSH->sh, "iV" );
	
	retSH->mainTex  = glGetUniformLocation ( retSH->sh, "MainTex" );
	retSH->mainColor = glGetUniformLocation ( retSH->sh, "MainColor" );
}


/*
===============================

===============================
*/
void Shader::UpdateCamAndLight () 
{
	for (int i=0; i<32; i++) 
	{
		if (shaderList [i] == 0) 
			continue;
		
		shader_s* rsh = shaderList [i]; 
		glUseProgram (rsh->sh);
		
		glUniformMatrix4fv (rsh->V, 1, GL_FALSE, currCam->V._array);
		glUniformMatrix4fv (rsh->iV, 1, GL_FALSE, currCam->iV._array);
	}
}


/*
===============================

===============================
*/
void ShaderLoadAll ()
{
	Shader::Create ("blit.shader");
	Shader::Create ("sprite_sc.shader");
}
