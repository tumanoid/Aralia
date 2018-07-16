#include "stdafx.h"
#include <windows.h>
#include "input.h"

LRESULT CALLBACK GLWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HDC hDC;
HGLRC hRC;

struct WinVars_t 
{
	HWND			hWnd;
	HINSTANCE		hInstance;
	bool    		activeApp;
	bool    		isMinimized;
	unsigned		sysMsgTime;
    bool            fullScreen;
};

const char GLWINDOW_CLASS_NAME [] = "app v0.1a";

WinVars_t glb_win;
WinVars_t* p_glb_win = &glb_win;

HDC       g_hDC;
HGLRC     g_hRC;

//	описатель главного окна. нужен для эскпорта. используется в Input.cpp
HWND		hWnd;
HWND		conHW;

int DESCTOP_XRES;
int DESCTOP_YRES;

int XRES, YRES;

/*
============================
CreateWinContext

============================
*/
bool CreateWinContext (const int & width, const int & height)
{	
	WNDCLASSEX  wcx;
	RECT        rect;
	DWORD       style, exStyle;
	int         x, y;

	XRES = width;
	YRES = height;

	// обнуляем стейт окна
	memset ( &glb_win, 0, sizeof(glb_win) );

	glb_win.hInstance = static_cast<HINSTANCE>(GetModuleHandle(NULL));

	// регистрация класса окна
	memset(&wcx, 0, sizeof(wcx));
	wcx.hInstance     = glb_win.hInstance;
	wcx.lpfnWndProc   = reinterpret_cast<WNDPROC>(GLWindowProc);
	wcx.cbSize        = sizeof(wcx);
	wcx.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcx.lpszClassName = GLWINDOW_CLASS_NAME;
	wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
	
	if (!RegisterClassEx (&wcx)) 
	{
		ER ("RegisterClassEx");
		return false;
	}

	//	настроим аспект для оконного режима
	float deskTopAspect = (float)XRES/YRES;

	style   = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX;
	exStyle = WS_EX_WINDOWEDGE | WS_EX_APPWINDOW;
	RECT  WindowRect;             // Grabs Rectangle Upper Left / Lower Right Values
    
    WindowRect.left   = (long)0;        // Set Left Value To 0
    WindowRect.right  = (long)XRES;     // Set Right Value To Requested Width
    WindowRect.top    = (long)0;        // Set Top Value To 0
    WindowRect.bottom = (long)YRES;
    
    //	какого размера нужно окно, чтб вместило и бортики и контекст
    AdjustWindowRectEx (&WindowRect, style, FALSE, exStyle);
    
	//	разрешение рабочего стола
	RECT desktop;
	
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	DESCTOP_XRES = desktop.right;
	DESCTOP_YRES = desktop.bottom;

	// подготовим позицию окна
	x = ( GetSystemMetrics ( SM_CXSCREEN ) - XRES )  / 2 + 000;
	y = ( GetSystemMetrics ( SM_CYSCREEN ) - YRES ) / 2;

	rect.left   = x;
	rect.right  = x + XRES;
	rect.top    = y;
	rect.bottom = y + YRES;

	//	создадим окно заданного стиля, размера и положения
	glb_win.hWnd = CreateWindowEx ( 
		exStyle, 
		GLWINDOW_CLASS_NAME, 
		GLWINDOW_CLASS_NAME, 
	    style, 
	    x, y,
	    WindowRect.right-WindowRect.left,
	    WindowRect.bottom-WindowRect.top, 
	    NULL, 
	    NULL, 
	    glb_win.hInstance, 
	    NULL);
	
	if (!glb_win.hWnd) 
	{
		ER ("Create main window");
		return false;
	}

	//	сохраним значение глобально доступ.
	hWnd = glb_win.hWnd;

	ShowWindow (glb_win.hWnd, SW_SHOW);
	SetForegroundWindow (glb_win.hWnd);
	SetFocus (glb_win.hWnd);
	UpdateWindow (glb_win.hWnd);
	
	return true;
}


/*
============================

============================
*/
void CreateGLContext ()
{
	PIXELFORMATDESCRIPTOR pfd;
	HGLRC                 hRCTemp;
	int                   format;

	// получим дескриптор контекста окна
	g_hDC = GetDC ( glb_win.hWnd );
	
	if (!g_hDC) 
	{
		ER ("GetDC");
		return;
	}

	// описание формата пикселей
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize      = sizeof(pfd);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL  | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	// запросим формат пикселей, ближайший к описанному выше
	format = ChoosePixelFormat ( g_hDC, &pfd );
	if (!format || !SetPixelFormat ( g_hDC, format, &pfd) ) 
	{
		ER ("ChoosePixelFormat");
		return;
	}

	// создадим временный контекст рендеринга
	// он нужен для получения функции wglCreateContextAttribsARB	
    hRCTemp = wglCreateContext (g_hDC);
	if (!hRCTemp)
	{
		ER ("wglCreateContext");
		return;
	}

	if (!wglMakeCurrent(g_hDC, hRCTemp)) 
	{
		ER ("wglMakeCurrent");
		return;
	}

	//	GLEW init
    GLenum err = glewInit();
    if (GLEW_OK != err) 
	{
		ER ("Glew init: %s", glewGetErrorString(err));
		return;
    }
	else 
	{
		DG ("Using GLEW %s", glewGetString (GLEW_VERSION));
	}

	// удалить временный контекст
	wglMakeCurrent (NULL, NULL);
	wglDeleteContext (hRCTemp);

	if (!wglCreateContextAttribsARB) 
	{
		ER ("wglCreateContextAttribsARB not found");
		return;
	}
	
	// расширенный контекст с поддержкой OpenGL 3.3
	// https://www.khronos.org/registry/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
	int attribs [] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3, 
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
	
	g_hRC = wglCreateContextAttribsARB (g_hDC, 0, attribs);
	if (!g_hRC || !wglMakeCurrent (g_hDC, g_hRC))
	{
        ER ("Can't create OpenGL 3.3 context");
		return;
	}

	
	//---------------------------------------------
	//	Все. Контекст создан. можно рисовать.
	glViewport (0, 0, XRES, YRES);

	//	информация о контексте OpenGL
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	
	DG ( "OpenGL info:\n Renderer : %s\n Vendor : %s\n Version : %s\n GLSL version : %s\n OpenGL version : %d.%d",
		(const char*)glGetString(GL_RENDERER),
		(const char*)glGetString(GL_VENDOR),
		(const char*)glGetString(GL_VERSION),
		(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION),
		major, minor
	);
	
	//	СИСТЕМА ВВОДА
	Input::InitMouse ();
}


void TI_Start ();
void TI_Frame ();
void TI_Render ();


#include <time.h>

static clock_t t, pt;

int		currFrame(0);
float   currTime(0);
float	deltaTime(1/60.0);


void TI_Loop ()
{
	MSG msg;

    glb_win.activeApp = true;

	wglSwapIntervalEXT (0); // VSYNC

	clock_t start = clock() + 17;


	while (glb_win.activeApp) 
	{	
		while ( PeekMessage ( &msg, 0, 0, 0, PM_REMOVE ) ) 
		{
			if ( msg.message == WM_QUIT ) 
			{
				glb_win.activeApp = false;
				break;
			}		

			TranslateMessage ( &msg );    
			DispatchMessage ( &msg );
		}
		
		if (start > clock())
			continue;		
		start = clock() + 17;
		
		//	кнопки + тач
		Input::GetInput ();
		
		//	master bit
		TI_Frame ();

		currFrame++;
		deltaTime = 1/60.0;
		currTime += deltaTime;

		//GUI2Core ();
		//ActiveCore ();
	
		SwapBuffers (g_hDC);
		//Sleep (1);
	}

	DestroyWindow	(hWnd);
}


LRESULT CALLBACK GLWindowProc ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) 
{
	switch (msg) 
	{
		case WM_CLOSE: 
			glb_win.activeApp = false;
			break;
		
		case WM_SIZE:
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		case WM_KEYDOWN:			
	    case WM_SYSKEYUP:
        case WM_KEYUP:
			break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEMOVE:
		{
            int m_btn = 0;
            if (wParam & MK_LBUTTON)
			    m_btn |= 1;
            
            if (wParam & MK_RBUTTON)
                m_btn |= 2;

            if (wParam & MK_MBUTTON)
                m_btn |= 4;
            
			Input::MouseEvent (m_btn);

			break;
        }
	}

	return DefWindowProc ( hWnd, msg, wParam, lParam );
}


/*
=============================
=============================
*/
void StartApp ()
{
	srand ( time(NULL) );

	CreateWinContext (800, 480);
	CreateGLContext ();
	TI_Start ();
	TI_Loop ();
}

int main (int argc, _TCHAR* argv[])
{
	StartApp ();
	return 0;
}