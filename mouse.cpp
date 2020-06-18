#include "DxLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int bord[10][15] =
{ {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {2,2,2,2,2,2,2,2,2,2,2,2,2,2,0} };

typedef struct player {
	char name[30];
	int attack;
	int hand[14];
}PLAYER;

PLAYER pooh;
PLAYER piglet;

int x, y, c;
int turn;
int a, b;
int max;
int min;
int tmin, tmax;
int empty;
int e = 1;
int top = 7, end = 7;
int hx = 0, hy = 0;
int HX, HY;
int bx = 0, by = 0;
int BX, BY;
int gh, gt;

void shuffle(void);
int random(int min, int max);
void poohhand(void);
void color(int c, int x, int y);
void vs(void);
void even(void);
void odd(void);
int Max(int hand[14]);
int Min(int hand[14]);
int pigletbord(int hand[14]);
void first(void);
void tem(void);
void point(void);
void score(int k, int x, int y);

int Key[256];
int gpUpdateKey()
{
	char tmpKey[256];
	GetHitKeyStateAll(tmpKey);
	for (int i = 0; i < 256; i++)
	{
		if (tmpKey[i] != 0)
		{
			Key[i]++;
		}
		else
		{
			Key[i] = 0;
		}
	}
	return 0;
}

int card[36] = { 4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,8,8,8 };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(900, 900, 16);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	LoadGraphScreen(0, 0, "back.png", TRUE);
	ScreenFlip();
	LoadGraphScreen(10, 330, "start.png", TRUE);
	ScreenFlip();

	WaitKey();


	LoadGraphScreen(0, 0, "back.png", TRUE);
	ScreenFlip();
	LoadGraphScreen(50, 10, "name.png", TRUE);
	ScreenFlip();

	shuffle();
	poohhand();

	vs();
	WaitKey();

	tmin = Min(piglet.hand);

	first();
	tem();

	for (int t = 0; t < 27; t++)
	{
		if (gh == 1 && gt == 1)
		{
			break;
		}
		 else if (turn == 0)
		{
			while (ProcessMessage() == 0)
			{
				int input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

				GetMousePoint(&hx, &hy);

				if (input & PAD_INPUT_10)
				{
					HX = int((hx - 30) / 60);
					bord[1][13] = pooh.hand[HX];
					odd();
					if (gt == 1)
					{
						turn = 0;
					}
					else
					{
						turn++;
					}
					break;
				}
				else if (input & PAD_INPUT_START)
				{
					gh = 1;
					if (gt == 1)
					{
						turn = 0;
					}
					else
					{
						turn++;
					}
					break;
				}
			}

			WaitKey();
			bord[1][13] = 0;

			while (ProcessMessage() == 0)
			{
				int input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

				if (input & PAD_INPUT_10)
				{
					GetMousePoint(&bx, &by);

					BY = int(by / 90);
					if (BY % 2 == 0)
					{
						BX = int((bx - 30) / 60);
						bord[BY][BX] = pooh.hand[HX];
						pooh.hand[HX] = 0;
						odd();
						even();
						break;
					}
					else
					{

						BX = int(bx / 60);

						if (BY == 7)
						{
							e++;
							if (BX > 7)
							{
								++end;
							}
							else
							{
								--top;
							}
						}

						bord[BY][BX] = pooh.hand[HX];
						pooh.hand[HX] = 0;
						odd();
						even();
						break;
					}
				}
				else if (input & PAD_INPUT_START)
				{
					break;
				}
			}

			WaitKey();
		}
		else if(turn == 1)
		{
			tmax = Max(piglet.hand);
			pigletbord(piglet.hand);
			piglet.hand[empty] = 0;
			if (gh == 1)
			{
				turn = 1;
			}
			else
			{
				turn--;
			}
		}
		else
		{
			break;
		}

		tem();
	}

	point();

	WaitKey();
	DxLib_End();

	return 0;
}

void shuffle(void)
{
	srand((unsigned int)time(NULL));
	int count = 1 + rand() % 6;

	for (int c = 0; c < count; c++)
	{
		for (int i = 35; i > 0; i--)
		{
			int tmp;
			int j = random(0, 35);

			if (i != j)
			{
				tmp = card[i];
				card[i] = card[j];
				card[j] = tmp;
			}
		}
	}

	int h = 0;

	for (int i = 0; i < 14; i++)
	{
		if (card[i] == 4)
		{
			pooh.hand[h] = card[i];
			++h;
		}
	}

	for (int i = 0; i < 14; i++)
	{
		if (card[i] == 5)
		{
			pooh.hand[h] = card[i];
			++h;
		}
	}

	for (int i = 0; i < 14; i++)
	{
		if (card[i] == 6)
		{
			pooh.hand[h] = card[i];
			++h;
		}
	}

	for (int i = 0; i < 14; i++)
	{
		if (card[i] == 7)
		{
			pooh.hand[h] = card[i];
			++h;
		}
	}

	for (int i = 0; i < 14; i++)
	{
		if (card[i] == 8)
		{
			pooh.hand[h] = card[i];
			++h;
		}
	}

	for (int i = 14; i < 28; i++)
	{
		piglet.hand[i - 14] = card[i];
	}
}

int random(int min, int max)
{
	return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

void poohhand(void)
{
	y = 0;

	for (int i = 0; i < 11; i++)
	{
		x = 30;
		for (int j = 0; j < 15; j++)
		{
			if (bord[i][j] == 2)
			{
				c = pooh.hand[j];
				color(c, x, y);
			}
			x += 60;
		}
		y += 90;
	}
}

void color(int c, int x, int y)
{
	switch (c)
	{
	case 4:
		LoadGraphScreen(x, y, "g.png", TRUE);
		ScreenFlip();
		break;
	case 5:
		LoadGraphScreen(x, y, "b.png", TRUE);
		ScreenFlip();
		break;
	case 6:
		LoadGraphScreen(x, y, "y.png", TRUE);
		ScreenFlip();
		break;
	case 7:
		LoadGraphScreen(x, y, "r.png", TRUE);
		ScreenFlip();
		break;
	case 8:
		LoadGraphScreen(x, y, "p.png", TRUE);
		ScreenFlip();
		break;
	}
}

void vs(void)
{
	srand((unsigned int)time(NULL));
	pooh.attack = rand() % 10 + 1;

	if (pooh.attack % 2 == 0)
	{
		LoadGraphScreen(0, -13, "apple.png", TRUE);
		ScreenFlip();
		turn = 0;
	}
	else
	{
		LoadGraphScreen(0, 30, "apple.png", TRUE);
		ScreenFlip();
		turn = 1;
	}
}

void even(void)
{
	b = 0;
	for (int i = 0; i < 10; i++)
	{
		a = 30;
		for (int j = 0; j < 15; j++)
		{
			if (i % 2 == 0)
			{
				c = bord[i][j];
				color(c, a, b);
			}
			a += 60;
		}
		b += 90;
	}
}

void odd(void)
{
	b = 0;
	for (int i = 0; i < 10; i++)
	{
		a = 0;
		for (int j = 0; j < 15; j++)
		{
			if (i % 2 == 1)
			{
				c = bord[i][j];
				color(c, a, b);
			}
			a += 60;
		}
		b += 90;
	}
}

int Max(int hand[14])
{
	max = 0;
	int g = 0, b = 0, y = 0, r = 0, p = 0;
	int no;

	for (int i = 0; i < 14; i++)
	{
		if (hand[i] == 4)
		{
			g++;
		}
		else if (hand[i] == 5)
		{
			b++;
		}
		else if (hand[i] == 6)
		{
			y++;
		}
		else if (hand[i] == 7)
		{
			r++;
		}
		else if (hand[i] == 8)
		{
			p++;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (g > max)
		{
			max = g;
			no = 4;
		}
		else if (b > max)
		{
			max = b;
			no = 5;
		}
		else if (y > max)
		{
			max = y;
			no = 6;
		}
		else if (r > max)
		{
			max = r;
			no = 7;
		}
		else if (p > max)
		{
			max = p;
			no = 8;
		}
	}

	return no;

}

int Min(int hand[14])
{
	min = 8;
	int g = 0, b = 0, y = 0, r = 0, p = 0;
	int no;

	for (int i = 0; i < 14; i++)
	{
		if (hand[i] == 4)
		{
			g++;
		}
		else if (hand[i] == 5)
		{
			b++;
		}
		else if (hand[i] == 6)
		{
			y++;
		}
		else if (hand[i] == 7)
		{
			r++;
		}
		else if (hand[i] == 8)
		{
			p++;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		if (g < min)
		{
			min = g;
			no = 4;
		}
		else if (b < min)
		{
			min = b;
			no = 5;
		}
		else if (y < min)
		{
			min = y;
			no = 6;
		}
		else if (r < min)
		{
			min = r;
			no = 7;
		}
		else if (p < min)
		{
			min = p;
			no = 8;
		}
	}

	return no;
}

int pigletbord(int hand[14])
{
	empty = 0;
	for (int k = 0; k < 14; k++)
	{
		if ((e == 5 || e == 6) && hand[k] == tmin)
		{
			e++;
			--top;
			empty = k;
			return bord[7][top] = hand[k];
		}
	}

	for (int k = 0; k < 14; k++)
	{
		tmax = Max(piglet.hand);
		if (max > 2 && hand[k] == tmax && e != 7)
		{
			e++;
			++end;
			empty = k;
			return bord[7][end] = hand[k];
		}

	}

	for (int k = 0; k < 14; k++)
	{
		if (hand[k] != 0 && hand[k] != tmin)
		{
			for (int i = 0; i < 11; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					if (i % 2 == 0)
					{
						if (bord[i][j] == 1 && (((hand[k] == bord[i + 1][j]) && (bord[i + 1][j + 1] == tmin)) || ((bord[i + 1][j] == tmin) && (hand[k] == bord[i + 1][j + 1]))) && ((bord[i + 1][j] != 1) && (bord[i + 1][j + 1] != 1)))
						{
							empty = k;
							return bord[i][j] = hand[k];
							break;
						}
						else
						{
							continue;
						}
					}
					else
					{
						if (bord[i][j] == 1 && (((hand[k] == bord[i + 1][j]) && (bord[i + 1][j - 1] == tmin)) || ((bord[i + 1][j] == tmin) && (hand[k] == bord[i + 1][j - 1]))) && ((bord[i + 1][j] != 1) && (bord[i + 1][j - 1] != 1)))
						{
							empty = k;
							return bord[i][j] = hand[k];
							break;
						}
						else
						{
							continue;
						}
					}
				}
			}
		}
		else
		{
			continue;
		}
	}

	for (int k = 0; k < 14; k++)
	{
		tmax = Max(piglet.hand);

		if (hand[k] == tmax)
		{
			for (int i = 0; i < 11; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					if (i % 2 == 0)
					{
						if (bord[i][j] == 1 && ((hand[k] == bord[i + 1][j]) || (hand[k] == bord[i + 1][j + 1])) && ((bord[i + 1][j] != 1) && (bord[i + 1][j + 1] != 1)))
						{
							empty = k;
							return bord[i][j] = hand[k];
							break;
						}
						else
						{
							continue;
						}
					}
					else
					{
						if (bord[i][j] == 1 && ((hand[k] == bord[i + 1][j]) || (hand[k] == bord[i + 1][j - 1])) && ((bord[i + 1][j] != 1) && (bord[i + 1][j - 1] != 1)))
						{
							empty = k;
							return bord[i][j] = hand[k];
							break;
						}
						else
						{
							continue;
						}
					}
				}
			}
			if (e != 7)
			{
				e++;
				++end;
				empty = k;
				return bord[7][end] = hand[k];
			}
		}
		else
		{
			continue;
		}
	}

	for (int k = 0; k < 14; k++)
	{
		if (hand[k] != 0)
		{
			for (int i = 0; i < 11; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					if (i % 2 == 0)
					{
						if (bord[i][j] == 1 && ((hand[k] == bord[i + 1][j]) || (hand[k] == bord[i + 1][j + 1])) && ((bord[i + 1][j] != 1) && (bord[i + 1][j + 1] != 1)))
						{
							empty = k;
							return bord[i][j] = hand[k];
							break;
						}
						else
						{
							continue;
						}
					}
					else
					{
						if (bord[i][j] == 1 && ((hand[k] == bord[i + 1][j]) || (hand[k] == bord[i + 1][j - 1])) && ((bord[i + 1][j] != 1) && (bord[i + 1][j - 1] != 1)))
						{
							empty = k;
							return bord[i][j] = hand[k];
							break;
						}
						else
						{
							continue;
						}
					}
				}
			}
			if (e != 7)
			{
				e++;
				++end;
				empty = k;
				return bord[7][end] = hand[k];
				break;
			}
		}
		else
		{
			continue;
		}
	}

	gt = 1;
}

void first(void)
{
	if (turn == 0)
	{
		while (ProcessMessage() == 0)
		{
			int input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			GetMousePoint(&hx, &hy);

			if (input & PAD_INPUT_10)
			{
				HX = int((hx - 30) / 60);
				bord[7][7] = pooh.hand[HX];
				even();
				odd();
				pooh.hand[HX] = 0;
				turn++;
				break;
			}
		}

		WaitKey();
	}
	else
	{
		tmax = Max(piglet.hand);
		for (int i = 0; i < 14; i++)
		{
			if (piglet.hand[i] == tmax)
			{
				bord[7][7] = piglet.hand[i];
				even();
				odd();
				piglet.hand[i] = 0;
				turn--;
				break;
			}
			else
			{
				continue;
			}
		}
	}
}

void tem(void)
{
	ClearDrawScreen();

	LoadGraphScreen(0, 0, "back.png", TRUE);
	ScreenFlip();

	LoadGraphScreen(50, 10, "name.png", TRUE);
	ScreenFlip();

	even();
	odd();

	poohhand();

	if (turn == 0)
	{
		LoadGraphScreen(0, -13, "apple.png", TRUE);
		ScreenFlip();
	}
	else
	{
		LoadGraphScreen(0, 30, "apple.png", TRUE);
		ScreenFlip();
	}

	WaitKey();
}

void point(void)
{
	int k = 0, l = 0;
	for (int i = 0; i < 14; i++)
	{
		if (pooh.hand[i] != 0)
		{
			k++;
		}
	}

	for (int i = 0; i < 14; i++)
	{
		if (piglet.hand[i] != 0)
		{
			l++;
		}
	}

	ClearDrawScreen();
	LoadGraphScreen(0, 0, "back.png", TRUE);
	ScreenFlip();
	score(k,220,350);
	LoadGraphScreen(390, 330, "bou.png", TRUE);
	ScreenFlip();
	score(l, 580, 350);

	WaitKey();

	ClearDrawScreen();
	LoadGraphScreen(0, 0, "back.png", TRUE);
	ScreenFlip();

	if (k > l)
	{
		LoadGraphScreen(10, 330, "lose.png", TRUE);
		ScreenFlip();
	}
	else if (k == l)
	{
		LoadGraphScreen(10, 330, "draw.png", TRUE);
		ScreenFlip();
	}
	else
	{
		LoadGraphScreen(10, 330, "win.png", TRUE);
		ScreenFlip();
	}
}

void score(int k,int x,int y)
{
	switch (k)
	{
	case 0:
		LoadGraphScreen(x, y, "0.png", TRUE);
		ScreenFlip();
		break;
	case 1:
		LoadGraphScreen(x, y, "1.png", TRUE);
		ScreenFlip();
		break;
	case 2:
		LoadGraphScreen(x, y, "2.png", TRUE);
		ScreenFlip();
		break;
	case 3:
		LoadGraphScreen(x, y, "3.png", TRUE);
		ScreenFlip();
		break;
	case 4:
		LoadGraphScreen(x, y, "4.png", TRUE);
		ScreenFlip();
		break;
	case 5:
		LoadGraphScreen(x, y, "5.png", TRUE);
		ScreenFlip();
		break;
	case 6:
		LoadGraphScreen(x, y, "6.png", TRUE);
		ScreenFlip();
		break;
	case 7:
		LoadGraphScreen(x, y, "7.png", TRUE);
		ScreenFlip();
		break;
	case 8:
		LoadGraphScreen(x, y, "8.png", TRUE);
		ScreenFlip();
		break;
	case 9:
		LoadGraphScreen(x, y, "9.png", TRUE);
		ScreenFlip();
		break;
	}
}