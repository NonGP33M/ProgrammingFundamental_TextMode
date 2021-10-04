#include<stdio.h>
#include<windows.h>
#include<time.h>
#include<conio.h>

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void draw_ship(int x, int y)
{
	gotoxy(x, y); printf(" <-0-> ");
}
void draw_bullet(int x, int y)
{
	gotoxy(x, y); printf("^");
}
void draw_star(int x, int y)
{
	gotoxy(x, y); printf("*");
}
void erase(int x, int y)
{
	gotoxy(x, y); printf(" ");
}
void clear_bullet(int x, int y)
{
	gotoxy(x, y); printf(" ");
}
void update_score(int x)
{
	gotoxy(100, 1); printf("Score : %d",x);
}
char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];

}
int main()
{
	char ch = '.';
	int x = 38, y = 20;
	int bx, by, i;
	int bullet = 0;
	int score = 0;
	srand(time(NULL));
	for (i = 0; i < 20; i++) 
		draw_star(rand()%61+10,rand()%4+2);
	setcursor(0);
	draw_ship(x, y);
	do {
		if (_kbhit()) 
		{
			ch = _getch();
			if (ch == 'a' && x>0) { draw_ship(--x, y); }
			if (ch == 's' && x<=80) { draw_ship(++x, y); }
			if (bullet != 1 && ch == ' ') { bullet = 1; bx = x + 3; by = y - 1; }
			fflush(stdin);
		}
		if (bullet == 1) 
		{
			clear_bullet(bx, by);
			if (by == 2) 
			{ 
				bullet = 0;
			}
			else if (cursor(bx, by - 1) == '*') 
			{ 
				bullet = 0; 
				erase(bx, by - 1); 
				Beep(700, 100); 
				draw_star(rand() % 61 + 10, rand() % 4 + 2); 
				score++;
			}
			else 
			{ 
				draw_bullet(bx, --by); 
				Beep(30, 100); 
			}

		}
		update_score(score);
		Sleep(100);
	} while (ch != 'x');
	return 0;
}