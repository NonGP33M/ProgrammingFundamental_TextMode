#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <cstring>
#define scount 80
#define screen_x 80
#define screen_y 25
HANDLE wHnd;
HANDLE rHnd;
DWORD fdwMode;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
COORD star[scount];
int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void clear_buffer()
{
	for (int y = 0; y < screen_y; ++y)
	{
		for (int x = 0; x < screen_x; ++x)
		{
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 7;
		}
	}
}
void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,&windowSize);
}
void init_star()
{
	for (int i = 1; i < scount; i++)
	{
		star[i] = { SHORT(rand() % screen_x), SHORT(rand() % screen_y) };
	}
}
class Player
{
public:
	Player()
	{
		HP = 10;
		x = 2;
		y = 0;
		strcpy_s(pp, "<-0->");
		color = 7;
		for (short i = 0; i < 5; i++)
		{
			Pawn[i] = { short(x - 2 + i), (short)y };
		}
	}
	void draw()
	{
		for (int i = 0; i < 5; ++i)
		{
			consoleBuffer[Pawn[i].X + screen_x * Pawn[i].Y].Char.AsciiChar = pp[i];
			consoleBuffer[Pawn[i].X + screen_x * Pawn[i].Y].Attributes = color;
		}
	}
	void setPosition(COORD pos)
	{
		x = pos.X;
		y = pos.Y;
		for (short i = 0; i < 5; i++)
		{
			Pawn[i] = { short(x - 2 + i), (short)y };
		}
	}
	void changeColor(int c)
	{
		color = c;
	}
	bool isContain(COORD pos)
	{
		for (short i = 0; i < 5; i++)
		{
			if (Pawn[i].X == pos.X && Pawn[i].Y == pos.Y)
			{
				return true;
			}
		}
		return false;
	}
	int HP;
private:
	int x;
	int y;
	COORD Pawn[5];
	char pp[6];
	WORD color;
};
void star_fall(Player& player)
{
	int i;
	for (i = 0; i < scount; i++)
	{
		if (star[i].Y >= screen_y - 1)
		{
			star[i] = { SHORT(rand() % screen_x),1 };
		}
		else
		{
			star[i].Y++;
			if (player.isContain(star[i]))
			{
				player.HP--;
				star[i] = { SHORT(rand() % screen_x),1 };
			}
		}
	}
}
void fill_star_to_buffer()
{
	for (int i = 0; i < scount; ++i)
	{
		consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
		consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 7;
	}
}
int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}
int main()
{
	bool play = true;
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	setConsole(screen_x, screen_y);
	setMode();
	init_star();
	Player player;
	while (play && player.HP > 0)
	{
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0)
		{
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead && i < numEvents; ++i)
			{
				if (eventBuffer[i].EventType == KEY_EVENT &&eventBuffer[i].Event.KeyEvent.bKeyDown == true)
				{
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
					{
						play = false;
					}
					if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c')
					{
						player.changeColor(rand() % 15 + 1);
					}

				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT)
				{
					if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
					{
						player.changeColor(rand() % 15 + 1);
					}
					else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED)
					{
						player.setPosition(eventBuffer[i].Event.MouseEvent.dwMousePosition);
					}
				}
			}
			delete[] eventBuffer;
		}
		star_fall(player);
		clear_buffer();
		fill_star_to_buffer();
		player.draw();
		fill_buffer_to_console();
		Sleep(100);
	}
	return 0;
}