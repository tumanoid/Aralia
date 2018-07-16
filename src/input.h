#ifndef INPUT_H
#define INPUT_H
	
struct BtStat 
{
	enum Enum {NOP=0, START=1, HOLD=2, RELEASE=3};
};


/*
=================================

=================================
*/
struct GameButton 
{
	BtStat::Enum currStat;
	int* btn;
	
	//	[ctors]
	explicit GameButton (int* _btn);
	virtual ~GameButton () {}

	static void UpdateButtonList ();

	void Update () 
	{
		switch (currStat)
		{
			case BtStat::NOP:
				if (*btn) 
					currStat = BtStat::START;
				break;
			
			case BtStat::START:
				currStat = BtStat::HOLD;
				break;
			
			case BtStat::HOLD:
				if (!(*btn))
					currStat = BtStat::RELEASE;
				break;

			case BtStat::RELEASE:
				currStat = BtStat::NOP;
				break;
		}
	}
};


/*
=================================

=================================
*/
struct Input 
{
	//@ vid.cpp ???
	static void LockHideCursor ();
	static void ShowReleaseCursor ();

	static int CTRL;
	static GameButton	btnCTRL;	//	CTRL
	
	//	Кнопки   
	static int ESC;
	static GameButton	btnEsc;	//	ESC

	static int UpArrow, DownArrow, LeftArrow, RightArrow;
	static GameButton	btnUA;	//	UpArrow
	static GameButton	btnDA;	//	DownArrow
	static GameButton	btnLA;	//	LeftArrow
	static GameButton	btnRA;	//	RightArrow
	
	static int TAB;
	static GameButton	btnTAB;	//	Tab
	
	//	Оси
	static int Forward, Backward;
    static int LeftStrafe, RightStrafe;
    static int Up, Down;
	
	//	Указатель мыши
	static bool b_GameMode;
	static bool b_LockCursor;
	
	static int mouseX;
	static int mouseY;
	
	//	Утилиты
    static void InitMouse ();
    static int  MapKey (int key);
    static void KeyEvent (int key, bool down);
	static void MouseEvent (int btn);
    static void GetInput (void);
	static void RawMouseInput (int lastX, int lastY);
};

/*
=================================

=================================
*/
struct tchPoint 
{
	int stat; // 3 - click / 2-hold / 1-release / 0-nop
	float sx, sy; // [0..1]
	int px, py; // положение первого нажатия
	int x, y;   // текущее положение
	
	float ax, ay, accel; // RESERVED / можно усреднять
	float dx, dy;		 // отклонение от начального
	tchPoint () : stat(0), sx(0), sy(0), px(0), py(0), x(0), y(0), ax(0), ay(0), accel(0), dx(0), dy(0) {}
};


extern tchPoint tchList [5];

extern "C" void ClearTouchList ();
extern "C" void PushTouch (int id, int stat, int x, int y);
extern "C" void PushKey (int keyCode, int val);

#endif//INPUT_H
