#include "stdAfx.h"
#include "render.h"

ICamera* currCam(0);


/*
================================
ICamera::ICamera

================================
*/
ICamera::ICamera () 
{
	pos = vec3(0,0,0);
	rot = quat(vec3(0,1,0), 0);
	Init ();
}


/*
================================
ICamera::MatrixUpdate

================================
*/
void ICamera::MatrixUpdate () 
{
	mat4 T;
	mat4 R;	

	//	Инверсная камера
	T.make_identity ();
	R.make_identity ();
	T.set_translate ( vec3( pos.x , pos.y , pos.z ) );        
	rot.get_value (R);
	iV = T * R;

	//	Направляющие вектора
	right = vec3(1,0,0);
	rot.mult_vec (right);

	up = vec3(0,1,0);
	rot.mult_vec (up);

	forward = vec3(0,0,1);
	rot.mult_vec (forward);

	//	Видовая матрица	
	T.make_identity ();
	R.make_identity ();

	T.set_translate ( vec3( -pos.x ,-pos.y ,-pos.z ) );        
	inverse (rot).get_value (R);
	V = R * T;

	//	Перспектива * Вид 
	VP = P * V;
}


/*
================================
Экранные координаты

================================
*/
vec2 ICamera::SRPos (const vec3 & pos)
{
	vec4 p = VP * vec4(pos.x, pos.y, pos.z, 1);
	p = p/p.w;
	return 	vec2 (p.x * 0.5 + 0.5, p.y * 0.5 + 0.5);
}


/*
================================
ICamera::Init

================================
*/
void ICamera::Init (const int & _xres, const int & _yres, const float& fov, const float & znear, const float& zfar) 
{
	xres = _xres;
	yres = _yres;

	float fovy  = fov;
	float f = 1 / tanf (fovy * 3.141592 / 360); // = ctg(fovy)	

	// [-1..1]
	float A = (zfar + znear) / ( zfar - znear );
	float B = -2 * zfar * znear / ( zfar - znear );

	mat4 M (f * _yres / (float)_xres, 0, 0, 0,
		0,         f, 0, 0,
		0,         0, A, 1,
		0,         0, B, 0);

	P.set_value (M._array);
}