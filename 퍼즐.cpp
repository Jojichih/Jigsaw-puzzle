#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<mmsystem.h>
#include<time.h>
#include <Windows.h>
#include<stdlib.h>
#include<easyx.h>
#pragma comment(lib,"winmm.lib")


typedef struct
{

	DWORD t1;		
	DWORD t2;		
	unsigned int t;	  
}GAME, * PGAME;


HWND hwnd;           

IMAGE mm[8];       
IMAGE white;        
int map[4][4];       
int NUMBER = -1;     
int main();
int Num = 15;
int temp[15];

struct button
{
	int x;
	int y;
	int width;
	int height;
	COLORREF color;
	char* pText;
};

struct button* createButton(
	int x,
	int y,
	int width,
	int height,
	COLORREF color,
	const char* pText
)
{
	struct button* pB = (struct button*)malloc(sizeof(struct button));
	pB->x = x;
	pB->y = y;
	pB->width = width;
	pB->height = height;
	pB->pText = (char*)malloc(sizeof(strlen(pText) + 1));
	strcpy(pB->pText, pText);
	return pB;
}

void drawButton(struct button* pB)
{
	setfillcolor(pB->color);
	settextstyle(35, 0, "楷体");
	setlinecolor(BLUE);
	settextcolor(BLUE);
	setbkmode(TRANSPARENT);
	fillrectangle(pB->x, pB->y, pB->x + pB->width, pB->y + pB->height);
	outtextxy(pB->x + pB->width / 6, pB->y + pB->height / 6, pB->pText);

}

void loadResoure()
{
	char name[10];
	
	loadimage(&mm[0], "rocket.jpg", 800, 800);      
	loadimage(&white, "white.jpg", 200, 200);   

	for (int i = 1; i < 8; i++)
	{
		sprintf(name, "%d.jpg", i);           
		loadimage(&mm[i], name, 800, 800);   
	}
}

void drawMap()
{
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int x = j * 200;
			int y = i * 200;
			if (map[i][j] == 15)         
				putimage(x, y, &white);
			else
				putimage(x, y, 200, 200, &mm[NUMBER], (map[i][j] % 4 * 200), (map[i][j] / 4 * 200));
		}
	}
}

int searchWhite_i()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (map[i][j] == 15)
				return i;
		}
	}
	return -1; 
}

int searchWhite_j()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (map[i][j] == 15)
				return j;
		}
	}
	return -1;  
}

int gameOver()
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (map[i][j] != count++)
			{
				return 0;
			}
		}
	}
	return 1;
}

void playGame()
{
	char arr[10];
	time_t timep;
	struct tm* p;
	time(&timep);
	p = gmtime(&timep);
	int start = p->tm_sec;
	MOUSEMSG m;                
	while (TRUE)               
	{
		drawMap();            
		if (gameOver())       
		{
			int end = p->tm_sec;
			int danqian = end - start;
			sprintf(arr, "%d", danqian);
			MessageBoxA(hwnd, "The end of the game takes time" + arr[0], "Jigsaw puzzle", MB_OK);
			if (MessageBoxA(hwnd, "Success, whether to continue", "Jigsaw puzzle", MB_YESNO) == IDYES)
			{
				main();
			}
			exit(0);          
		}
		m = GetMouseMsg();    
		int w_i = searchWhite_i();    
		int w_j = searchWhite_j();	   
		int m_i;
		int m_j;
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			m_i = m.y / 200;           
			m_j = m.x / 200;			   
			if (w_i == m_i && w_j + 1 == m_j)  
			{
				map[w_i][w_j] = map[m_i][m_j];
				map[m_i][m_j] = 15;
			}
			if (w_i == m_i && w_j - 1 == m_j)  
			{
				map[w_i][w_j] = map[m_i][m_j];
				map[m_i][m_j] = 15;
			}
			if (w_i + 1 == m_i && w_j == m_j)  
			{
				map[w_i][w_j] = map[m_i][m_j];
				map[m_i][m_j] = 15;
			}
			if (w_i - 1 == m_i && w_j == m_j)  
			{
				map[w_i][w_j] = map[m_i][m_j];
				map[m_i][m_j] = 15;
			}
			break;
		}
	}
}

bool Lookup(int n)
{
	for (int i = 0; i < Num; i++)
	{
		if (temp[i] == n)
		{
			for (int m = i; m < Num - 1; m++)
			{
				temp[m] = temp[m + 1];
			}
			return 1;
		}
	}
	return 0;
}
void random()
{
	int Count = 0;
	int Mumber = 0;         
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Mumber++;
			Count = temp[rand() % Num];  
			if (Lookup(Count))
			{
				map[i][j] = Count;
				Num--;                
			}
			if (Mumber == 15)           
			{
				break;
			}
		}
	}
	map[3][3] = 15;                   
}


int mouseInButton(struct button* pB, MOUSEMSG m)
{
	if (pB->x <= m.x && m.x <= pB->x + pB->width && pB->y <= m.y && m.y <= pB->height + pB->y)
	{
		pB->color = RED;
		return 1;
	}
	pB->color = YELLOW;
	return 0;
}

int clickButton(struct button* pB, MOUSEMSG m)
{
	if (mouseInButton(pB, m) && m.uMsg == WM_LBUTTONDOWN)
	{
		return 1;
	}
	return 0;
}


int GUI()
{
	hwnd = initgraph(800, 600); 
	int iNum = 1;
	char cNum = 'A';
	IMAGE mm;
	loadimage(&mm, "main.png", 800, 800);
	
	struct button* play = createButton(300, 150, 180, 50, YELLOW, "Start");
	struct button* pause = createButton(300, 205, 180, 50, YELLOW, "Pause");
	struct button* end = createButton(300, 260, 180, 50, YELLOW, "End");
	while (1)
	{
		
		putimage(0, 0, &mm);
		drawButton(play);
		drawButton(pause);
		drawButton(end);
		MOUSEMSG m = GetMouseMsg();
		if (clickButton(play, m))
		{
			return 1;
		}

		if (clickButton(pause, m))
		{
			return 2;
		}

		if (clickButton(end, m))
		{
			closegraph();

			exit(1);
		}

	
	}
	closegraph();
	return 0;
}




int main(void)
{

	int count = GUI();
	switch (count)
	{
	case 1:
		NUMBER++;                          
		if (NUMBER == 8)                   
		{
			MessageBox(hwnd, "Congratulations on your clearance!", "Jigsaw puzzle", MB_OK);
			exit(0);
		}
		srand((unsigned int)time(NULL));

		for (int i = 0; i < 15; i++)   
		{
			temp[i] = i;
		}
		random();
		hwnd = initgraph(800, 800);      
		cleardevice();                 
		mciSendString("open bgm1.mp3", 0, 0, 0);  
		mciSendString("play bgm1.mp3", 0, 0, 0);   
		loadResoure();     
		playGame();        
		closegraph();      
		break;
	case 2:
		break;
	}
	return 0;
}
