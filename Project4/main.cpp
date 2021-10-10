#include <stdio.h>
#include <windows.h>
#include <time.h>
#define screen_x 80
#define screen_y 25
#define scount 80
HANDLE rHnd;
HANDLE wHnd;
DWORD fdwMode;
COORD bufferSize = { screen_x,screen_y };
CHAR_INFO consoleBuffer[screen_x * screen_y];
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
COORD characterPos = { 0,0 };
COORD star[scount];

struct Ship
{
	char s[6] = "<-0->";
	int x;
	int y;
	int life = 10;
}ship;

int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}
int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void init_star()
{
	for (int i = 0; i < scount; i++)
	{
		star[i] = { short(rand() % screen_x),short(rand() % screen_y) };
	}
}
void star_fall()
{
	for (int i = 0; i < scount; i++)
	{
		if (star[i].Y >= screen_y - 1) {
			star[i] = { short(rand() % screen_x), 1 };
		}
		else {
			star[i] = { short(star[i].X),short(star[i].Y + 1) };
		}
	}
}
void fill_star_to_buffer()
{
	for (int i = 0; i < scount; i++)
	{
		consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
		consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 7;
	}
}
void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,
		&windowSize);
}
void clear_buffer()
{
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 7;
		}
	}
}
void fill_ship_to_buffer(int x, int y, int color)
{
	for (int i = 0; i < 5; i++)
	{
		consoleBuffer[x + i + screen_x * y].Char.AsciiChar = ship.s[i];
		consoleBuffer[x + i + screen_x * y].Attributes = color;
	}
}
void collision_update()
{
	for (int i = 0; i < scount; i++)
	{
		if ((star[i].X >= ship.x && star[i].X <= ship.x + 5) && star[i].Y == ship.y)
		{
			star[i] = { short(rand() % screen_x), 1 };
			ship.life--;
		}
	}
}

int main()
{
	srand(time(NULL));
	bool play = true;
	int color = 7;
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	setcursor(0);
	setConsole(screen_x, screen_y);
	setMode();
	init_star();
	while (play && ship.life > 0)
	{
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true) {
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
						play = false;
					}
					if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c')
						color = rand() % 15 + 1;
				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT)
				{
					if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {

						ship.x = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
						ship.y = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
					}
					else if (eventBuffer[i].Event.MouseEvent.dwButtonState &
						FROM_LEFT_1ST_BUTTON_PRESSED)
					{
						color = rand() % 15 + 1;
					}
				}
			}
			delete[] eventBuffer;
		}
		collision_update();
		star_fall();
		clear_buffer();
		fill_star_to_buffer();
		fill_ship_to_buffer(ship.x, ship.y, color);
		fill_buffer_to_console();
		Sleep(100);
	}
	return 0;
}
