#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <conio.h>

using namespace std;

void cls(HANDLE &hConsole)
{
	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	// Get the number of character cells in the current buffer. 

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.

	if (!FillConsoleOutputCharacter(hConsole, (TCHAR) ' ',
		dwConSize, coordScreen, &cCharsWritten))
		return;

	// Get the current text attribute.

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	// Set the buffer's attributes accordingly.

	if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten))
		return;

	// Put the cursor at its home coordinates.

	SetConsoleCursorPosition(hConsole, coordScreen);
}


// Print to std console buffer
void printstd(char** &c, int s, HANDLE &std, int g)
{
	SetConsoleTextAttribute(std, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE);
	int alive = 0;
	wchar_t buff[1];
	for (int i = 2; i < s + 2; i++)
	{
		for (int j = 2; j < s + 2; j++)
		{
			if (c[i][j] == 'X')
			{
				alive++;
				SetConsoleTextAttribute(std, BACKGROUND_GREEN | FOREGROUND_GREEN);
			}
			cout << c[i][j] << ' ';
			SetConsoleTextAttribute(std, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		}
		SetConsoleTextAttribute(std, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		cout << endl;
	}
	SetConsoleTextAttribute(std, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	cout << endl;
	cout << "Alive: " << alive << '\t' << "Generations: " << g << endl;
}

// Print to additional console buffer
void printnew(char** &c, int s, HANDLE &h, int g)
{
	SetConsoleTextAttribute(h, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE);
	int alive = 0;
	int r = 10;
	wchar_t buff[1];
	for (int i = 2; i < s + 2; i++)
	{
		for (int j = 2; j < s + 2; j++)
		{
			if (c[i][j] == 'X')
			{
				alive++;
				SetConsoleTextAttribute(h, BACKGROUND_GREEN | FOREGROUND_GREEN);
			}
			buff[0] = c[i][j];
			WriteConsole(h, buff, 1, 0, NULL);
			WriteConsole(h, " ", 1, 0, NULL);
			SetConsoleTextAttribute(h, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		}
		SetConsoleTextAttribute(h, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		WriteConsole(h, "\n", 1, 0, NULL);
	}
	SetConsoleTextAttribute(h, BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
	WriteConsole(h, "\n", 1, 0, NULL);
		wchar_t buff2[7] = { 'A', 'l', 'i', 'v', 'e', ':', ' ' };
	WriteConsole(h, buff2, 7, 0, NULL);
	int sb;
	if (alive < 10) sb = 1;
	else if (alive < 100) sb = 2;
	else if (alive < 1000) sb = 3;
	else sb = 4;
	char buffer[5];
	char *p;
	p = _itoa(alive, buffer, r);
	wchar_t buff3[4] = {buffer[0], buffer[1], buffer[2], buffer[3] };
	WriteConsole(h, buff3, sb, 0, NULL);
	wchar_t buff4[14] = { '\t', 'G', 'e', 'n', 'e', 'r', 'a', 't', 'i', 'o', 'n', 's', ':', ' ' };
	WriteConsole(h, buff4, 14, 0, NULL);
	if (g < 10) sb = 1;
	else if (g < 100) sb = 2;
	else sb = 3;
	char buffer2[4];
	p = _itoa(g, buffer2, r);
	wchar_t buff5[4] = { buffer2[0], buffer2[1], buffer2[2] };
	WriteConsole(h, buff5, sb, 0, NULL);
}

void equal(char** &c1, char** &c2, int s)
{
	for (int i = 0; i < s + 4; i++)
	{
		for (int j = 0; j < s + 4; j++)
		{
			c2[i][j] = c1[i][j];
		}
	}
}

// Check if cell should die or live, or be born
void check(char** &c1, char** &c2, int s, int a, int b)
{
	int count;
	for (int i = a - 1; i < a + 2; i++)
	{
		for (int j = b - 1; j < b + 2; j++)
		{
			count = 0;
			for (int q = i - 1; q < i + 2; q++)
			{
				for (int w = j - 1; w < j + 2; w++)
				{
					if (c2[q][w] == 'X')
					{
						count++;
						if ((q == i) && (w == j)) count--;
					}
				}
			}
			if (c2[i][j] == 'X')
			{
				if ((count < 2) || (count > 3)) c1[i][j] = '.';
			}

			if (c2[i][j] == '.')
			{
				if (count == 3) c1[i][j] = 'X';
			}
		}
	}
}

int main()
{
	// Thats why we need admin rights
	HWND console = GetConsoleWindow();

	MoveWindow(console, 0, 0, 850, 900, TRUE);
	//

	string path;
	Sleep(100);
	cout << "Enter the path to input file" << endl;
	cin >> path;
	const int size = 50;
	char** space;
	char** space2;
	space = new char*[size + 4];
	space2 = new char*[size + 4];

	fstream f;
	int delay;
	f.open(path, fstream::in);
	if (f.is_open())
	{
		cout << "File successfully opened" << endl;
		cout << "Enter steps delay (in ms, 0 for fastest)" << endl;
		cin >> delay;
		Sleep(1000);
		for (int i = 0; i < size + 4; i++)
		{
			space[i] = new char[size + 4];
			space2[i] = new char[size + 4];
			for (int j = 0; j < size + 4; j++)
			{
				f >> space[i][j];
			}
		}
		f.close();
	}
	else
	{
		cout << "Error while opening file" << endl;
		return -1;
	}

	COORD bufferSize = { 110, 70 };

	COORD dest = {0, 0};

	HANDLE hStdout, hNewScreenBuffer;

	// Setting up std console buffer
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	int c = 0;

	SetConsoleScreenBufferSize(hStdout, bufferSize);
	cls(hStdout);
	printstd(space, size, hStdout, c);

	// Creating and setting up additional console buffer
	hNewScreenBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ |
		GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL);
	SetConsoleScreenBufferSize(hNewScreenBuffer, bufferSize);
	cls(hNewScreenBuffer);
	printnew(space, size, hNewScreenBuffer, c);

	while (c < 999)
	{
		equal(space, space2, size);
		for (int i = 2; i < size + 2; i++)
		{
			for (int j = 2; j < size + 2; j++)
			{
				if (space2[i][j] == 'X')
				{
					check(space, space2, size, i, j);
				}
			}
		}
		// Showing one buffer, while another rendering in background
		if (c % 2 == 0)
		{
			// This line is also important but only works with admin rights, fuck C++
			SetConsoleCursorPosition(hStdout, dest);

			SetConsoleActiveScreenBuffer(hStdout);
			cls(hNewScreenBuffer);
			printnew(space, size, hNewScreenBuffer, c);
		}
		else
		{
			// This line is also important but only works with admin rights, fuck C++
			SetConsoleCursorPosition(hNewScreenBuffer, dest);

			SetConsoleActiveScreenBuffer(hNewScreenBuffer);
			cls(hStdout);
			printstd(space, size, hStdout, c);
		}
		Sleep(delay);
		c++;
	}
	
	system("pause");
    return 0;
}
