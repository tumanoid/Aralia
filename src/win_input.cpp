#include "stdAfx.h"
#include "input.h"

#ifdef WIN32
#include "windows.h"

extern int XRES, YRES;

int X_CENTER;
int Y_CENTER;

int  mouse_btns(0);
int  mouse_btn_1(0);
int  mouse_btn_2(0);

//	описатель главного окна. нужен для эскпорта. используется в Input.cpp
extern HWND		hWnd;

bool Input::b_LockCursor ( true );
bool Input::b_GameMode (true);

int Input::mouseX;
int Input::mouseY;


/*
===========================
InitMouse

===========================
*/
void Input::InitMouse () 
{
	RECT window_rect;

	GetWindowRect ( hWnd, &window_rect );
	X_CENTER  = ( window_rect.right + window_rect.left)/2;
	Y_CENTER  = ( window_rect.top + window_rect.bottom)/2;
	SetCursorPos ( X_CENTER, Y_CENTER );
}


/*
===========================
MouseEvent

===========================
*/
void Input::MouseEvent (int m_btn) 
{
    mouse_btns = m_btn;
	mouse_btn_1 = m_btn & 1;
	mouse_btn_2 = m_btn & 2;	
}

/*
===========================
GetInput

===========================
*/
void Input::GetInput (void)
{   
    POINT		current_pos;
    GetCursorPos (&current_pos);

	RECT window_rect;
	RECT client_rect;
	GetWindowRect ( hWnd, &window_rect );
	GetClientRect (hWnd, &client_rect);
	
	X_CENTER  = ( window_rect.right + window_rect.left)/2;
	Y_CENTER  = ( window_rect.top + window_rect.bottom)/2;

	int mx = current_pos.x - X_CENTER;
	int my = current_pos.y - Y_CENTER;
	
	Input::mouseX = mx;
	Input::mouseY = my;

//	из координат монитора в координаты клиентской части окна		
	POINT p;
	p.x = current_pos.x;
	p.y = current_pos.y;
	
	ScreenToClient (hWnd, &p);
	mx = p.x;
	my = YRES - p.y;
		
//	пушим тач
	if (mouse_btn_1) 
	{
	//	id под виндой всегда 0
		PushTouch (0, 1, mx, my);
	}
	else
		PushTouch (0, 0, mx, my);
		
	if (0 && Input::b_GameMode)
		if ( mx || my )
			SetCursorPos ( X_CENTER , Y_CENTER );
}
#endif


//	ТАЧ лист
tchPoint tchList [5];


/*
=================================================
id		просто номер тача [0..4]
stat	нажат или нет
x,y	координаты

// 3 - click / 2-hold / 1-release / 0-nop
=================================================
*/
extern "C" void PushTouch (int id, int stat, int x, int y)
{
	if (stat == 0) // сигнал от сенсора. НЕ_НАЖАТ
	{
		tchList [id].stat--;
		if (tchList [id].stat < 0) 
			tchList [id].stat = 0;
		return;
	}

//	если не было, то клик
	if (tchList [id].stat == 0) 
	{
		tchList [id].x = x;
		tchList [id].y = y;
		
		tchList [id].px = x;
		tchList [id].py = y;

		tchList [id].sx = x / (float) XRES;
		tchList [id].sy = y / (float) YRES;

		tchList [id].ax = 0;
		tchList [id].ay = 0;
		tchList [id].accel = 0;
		tchList [id].dx = 0;
		tchList [id].dy = 0;
		tchList [id].stat = 3;	// CLICK
	}

//	если был, то и оставайся	
	else 
	{
		tchList [id].x = x;
		tchList [id].y = y;
		
		tchList [id].sx = x / (float) XRES;
		tchList [id].sy = y / (float) YRES;

		tchList [id].dx = ((float) (x - tchList [id].px)) / XRES;
		tchList [id].dy = ((float) (y - tchList [id].py)) / YRES;
		
		/*
		RESERVED

		float acc = sqrt (dx*dx + dy*dy);
		if (fabs(acc) > 0.006)
		{
			tchList [id].accel = acc;
		}
		*/
		
		tchList [id].stat = 2; // HOLD
	}
}
