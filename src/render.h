#ifndef __RENDER_H__
#define __RENDER_H__

/*
===================================
Texture

===================================
*/
struct Texture 
{
	static GLuint Create (int xres, int yres, void* buff = 0, int format = GL_RGB, bool filter = false);
	static GLuint Load (const char* fname, bool filter);
	static unsigned char* Load2Buff (const char* f_name, int& xres, int& yres, int& bpp);
};


/*
===================================
Shader

===================================
*/
struct Shader 
{
	GLuint sh; // RO

	GLint M;
	GLint V;
	GLint iV;
	GLint P;
	GLint VP;
	GLint MVP;

	GLint sunDir;
	GLint sunColor;
	GLint ambColor;

	GLint mainTex;
	GLint mainColor;
	
	virtual void SetTexture (const GLuint& texID, const GLuint& indexName, const GLuint& slot) = 0;

	static Shader* Create (const char* fname, const char* customDefs = 0);
	static void ReloadAll ();
	static void UpdateCamAndLight ();
	static Shader* Find (const char* name);

	virtual ~Shader () {}
};


/*
===================================
Assets

===================================
*/
struct Assets
{
	static GLuint GetTex (const char* name, bool filter = false);
	static void AddTex (const char* name, const GLuint& idNewTexture, const bool & filter = false);
	
	static GLuint whiteTex;
	static GLuint grayTex;
	static GLuint redTex;
};


/*
===================================
Камера

===================================
*/
struct ICamera 
{
	mat4 V;
	mat4 iV;
	mat4 P;
	mat4 VP;
	
	vec3	pos;
	quat	rot;

    vec3	forward;
	vec3	up;
	vec3	right;
	
	int		xres, yres;
	
	ICamera ();
	void MatrixUpdate ();
	void Init (const int & _xres = XRES, const int & _yres = YRES, const float& fov = 43, const float & znear = 0.1, const float& zfar = 100);
	vec2 SRPos (const vec3 & pos);
};

extern ICamera* currCam;

void ShaderLoadAll ();
void R_UtilLoader ();

struct R
{
	static void DrawSpriteSC (const vec4 & ps, const GLuint & tx, const vec4 & color, const vec4 & mudUVXX);
	static void ScreenQuadDraw ();
};


#endif//__SHADER_H__