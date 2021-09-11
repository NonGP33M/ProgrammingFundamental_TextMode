#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<iostream>

struct Ship {
	int x;
	int y;
	int state;
	int shoot;
	int bulletLeft;
	char str[8] = " <-0-> ";
	char erase[8] = "       ";
}ship;

struct Bullet
{
	int x;
	int y;
	char str[8] = "   .   ";
	char erase[8] = "       ";
}bullet[5];

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void draw(int x, int y,char str[])
{
	gotoxy(x, y); printf("%s",str);
}

void erase(int x, int y, char erase[])
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	printf("%s",erase);
}

void bulletUpdate(int i)
{
	setcolor(0, 0);
	erase(bullet[i].x, bullet[i].y, bullet[i].erase);
	setcolor(7, 0);
	draw(bullet[i].x, --bullet[i].y, bullet[i].str);
	if (bullet[i].y <= 0)
	{
		setcolor(0, 0);
		erase(bullet[i].x, bullet[i].y, bullet[i].erase);
		ship.bulletLeft++;
	}
}

int main()
{	
	ship.x = 38, ship.y = 20;
	ship.state = 0;
	ship.bulletLeft = 5;
	int shootingState = 1;
	int bulletCount = 0;
	char ch = 's';
	setcursor(0);
	setcolor(2, 4);
	draw(ship.x, ship.y, ship.str);
	do 
	{
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 'a') { ship.state = 1; }
			if (ch == 'd') { ship.state = 2; }
			if (ch == 's') { ship.state = 0; }
			if (ch == ' ' && ship.bulletLeft > 0)
			{ 
				shootingState = 1;
				bullet[bulletCount].x = ship.x;
				bullet[bulletCount].y = ship.y - 1;
				if (bulletCount >= 5)
				{
					bulletCount = 0;
				}
				else
				{
					bulletCount++;
					ship.bulletLeft--;
				}
			}
		}

		if (ship.state == 1 && ship.x > 0) 
		{ 
			setcolor(0, 0);
			erase(ship.x, ship.y ,ship.erase);
			setcolor(2, 4);
			draw(--ship.x, ship.y, ship.str);
		}

		if (ship.state == 2 && ship.x < 80)
		{
			setcolor(0, 0);
			erase(ship.x, ship.y, ship.erase);
			setcolor(2, 4);
			draw(++ship.x, ship.y, ship.str);
		}

		if (ship.state == 0)
		{
			setcolor(0, 0);
			erase(ship.x, ship.y, ship.erase);
			setcolor(2, 4);
			draw(ship.x, ship.y, ship.str);
		}

		for (int i = 0; i < 5; i++)
		{
			if (bullet[i].y > 0 && ship.bulletLeft >= 0)
			{
				bulletUpdate(i);
			}
		}

		fflush(stdin);
		Sleep(100);
	} while (ch != 'x');
	return 0;
}