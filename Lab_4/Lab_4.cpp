#include "stdafx.h"
#include "Lab_4.h"
#include <windows.h>
#include <cstring>
#include <string>
#include "windowsx.h"
#include "math.h"
#include <iostream>
#include <conio.h>
#include <random>
#include <time.h>
#include <ctime>


using namespace std;

#define MAX_LOADSTRING 100
#define IMAGE_SIZE 32
#define BRUSH(color) SelectBrush(hdc, CreateSolidBrush(color))
#define IDT_TIMER 10003


// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING] = _T("Лабораторная работа 4, Климчук А. А.");					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING] = _T("win32app");			// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#pragma region MyHeaders
HBITMAP LoadCustomImage(char* file);
void DrawBitmap(HDC hdc, HBITMAP hBitmap, int x, int y, int width, int height);
static void DrawMatrix(HDC hdc);
static void DrawMatrixStarWars(HDC hdc);
static void FindIndexsOfClick(int& indexOfX, int& indexOfY);
static void ChooseRule(HWND hWnd);
static bool RuleForWhitePawn(bool searchCheck, bool protectSquares);
static bool RuleForBlackPawn(bool searchCheck, bool protectSquares);
static bool RuleForKnight(bool searchCheck, bool protectSquares);
static bool RuleForBishop(bool searchCheck, bool protectSquares);
static bool RuleForRook(bool searchCheck, bool protectSquares);
static bool RuleForKing();
static bool RuleForQueen(bool searchCheck, bool protectSquares);
static void ChangeColorOfFigure();
static bool TakeFigure();
static void ChangeOfMove(bool a); // смена хода
static bool Checkmate(HWND hWnd);
static void IndexOfKing(bool KingRun);
static void SearchCheck(bool move);
static void SearchIndexsOfSquaresForNoCheck(bool move);
static void CleanArrayForCheckAndSecondCheck();
static void CleanFigureForNoCheck();
static bool MoveFigureForNoCheck();
static bool IsItFigureForNoCheck();
static bool IsItSquareForNoCheck();
static void MoveFigures(HWND hWnd, int lParam);
static bool NoDefence();
static void ProtectSquares();
static void CleanProtectSquares();
static bool IsItProtectSquares();
#pragma endregion

#pragma region Pictures
HBITMAP hFrames[] = { /*0*/LoadCustomImage("black_rook_in_white.bmp"), /*1*/LoadCustomImage("black_knight_on_black.bmp"), /*2*/LoadCustomImage("black_bishop_on_white.bmp"),
/*3*/LoadCustomImage("black_queen_on_black.bmp"),/*4*/LoadCustomImage("black_queen_on_white.bmp"), /*5*/ LoadCustomImage("black_king_on_black.bmp"),/*6*/LoadCustomImage("blak_king_on_white.bmp"),
/*7*/LoadCustomImage("black_bishop_on_black.bmp"),/*8*/LoadCustomImage("black_knight_on_white.bmp"), /*9*/ LoadCustomImage("black_rook_on_black.bmp"),
/*10*/LoadCustomImage("black_pawn_on_black.bmp"),/*11*/LoadCustomImage("black_pawn_on_white.bmp"),

/*12*/LoadCustomImage("white_rook_on_white.bmp"), /*13*/ LoadCustomImage("white_knight_on_black.bmp"),/*14*/ LoadCustomImage("white_bishop_on_white.bmp"),
/*15*/LoadCustomImage("white_queen_on_black.bmp"),/*16*/LoadCustomImage("white_queen_on_white.bmp"), /*17*/ LoadCustomImage("white_king_on_black.bmp"), /*18*/LoadCustomImage("white_king_on_white.bmp"),
/*19*/LoadCustomImage("white_bishop_on_black.bmp"),/*20*/LoadCustomImage("white_knight_on_white.bmp"), /*21*/ LoadCustomImage("white_rook_on_black.bmp"),
/*22*/LoadCustomImage("white_pawn_on_black.bmp"),/*23*/LoadCustomImage("white_pawn_on_white.bmp"),

/*24*/LoadCustomImage("white_square.bmp"),/*25*/LoadCustomImage("black_square.bmp")
};

HBITMAP hFramesStarWars[] = { /*0*/LoadCustomImage("2black_rook_on_white.bmp"), /*1*/LoadCustomImage("2black_knight_on_black.bmp"), /*2*/LoadCustomImage("2black_bishop_on_white.bmp"),
/*3*/LoadCustomImage("2black_queen_on_black.bmp"),/*4*/LoadCustomImage("2black_queen_on_white.bmp"), /*5*/ LoadCustomImage("2black_king_on_black.bmp"),/*6*/LoadCustomImage("2black_king_on_white.bmp"),
/*7*/LoadCustomImage("2black_bishop_on_black.bmp"),/*8*/LoadCustomImage("2black_knight_on_white.bmp"), /*9*/ LoadCustomImage("2black_rook_on_black.bmp"),
/*10*/LoadCustomImage("2black_pawn_on_black.bmp"),/*11*/LoadCustomImage("2black_pawn_on_white.bmp"),

/*12*/LoadCustomImage("2white_rook_on_white.bmp"), /*13*/ LoadCustomImage("2white_knight_on_black.bmp"),/*14*/ LoadCustomImage("2white_bishop_on_white.bmp"),
/*15*/LoadCustomImage("2white_queen_on_black.bmp"),/*16*/LoadCustomImage("2white_queen_on_white.bmp"), /*17*/ LoadCustomImage("2white_king_on_black.bmp"), /*18*/LoadCustomImage("2white_king_on_white.bmp"),
/*19*/LoadCustomImage("2white_bishop_on_black.bmp"),/*20*/LoadCustomImage("2white_knight_on_white.bmp"), /*21*/ LoadCustomImage("2white_rook_on_black.bmp"),
/*22*/LoadCustomImage("2white_pawn_on_black.bmp"),/*23*/LoadCustomImage("2white_pawn_on_white.bmp"),

/*24*/LoadCustomImage("2white_square.bmp"),/*25*/LoadCustomImage("2black_square.bmp")
};
#pragma endregion

#pragma region StandartPart
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAB_4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB_4));

	// Цикл основного сообщения:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB_4));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_LAB_4);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

HWND hwndButton, hwndButton2, hwndStarWars, hwndStandartStyle;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		1300, 810,
		NULL, NULL, hInstance, NULL);

	hwndStandartStyle = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		NULL/*L"OK"*/,      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		810,         // x position 
		200,         // y position 
		250,        // Button width
		80,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	SendMessage(hwndStandartStyle, WM_SETTEXT, 0, (LPARAM)L"Standart");


	hwndStarWars = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		NULL/*L"OK"*/,      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		1065,         // x position 
		200,         // y position 
		250,        // Button width
		80,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	SendMessage(hwndStarWars, WM_SETTEXT, 0, (LPARAM)L"Star Wars");






	hwndButton = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		NULL/*L"OK"*/,      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		810,         // x position 
		1,         // y position 
		500,        // Button width
		100,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	//SendMessage(hwndButton, WM_SETTEXT, 0, (LPARAM)L"ИГРОК 1");
	hwndButton2 = CreateWindow(
		L"BUTTON",  // Predefined class; Unicode assumed 
		NULL/*L"OK"*/,      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		810,         // x position 
		685,         // y position 
		500,        // Button width
		100,        // Button height
		hWnd,     // Parent window
		NULL,       // No menu.
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	SendMessage(hwndButton, WM_SETTEXT, 0, (LPARAM)L" ");
	SendMessage(hwndButton2, WM_SETTEXT, 0, (LPARAM)L"ХОД БЕЛЫХ");
	//SendMessage(hwndButton2, WM_SETTEXT, 0, (LPARAM)L"ИГРОК 2");
	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}
#pragma endregion

#pragma region GlobalVars
static int arr[8][8] = {
		{-41, -20, -31, -50, -61, -30, -21, -40},
		{-10, -11,-10, -11,-10, -11,-10, -11},
		{1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1},
		{11,10,11,10,11,10,11,10},
		{40,21,30,51,60,31,20,41}
};
static int startPosition[8][8] = {
		{-41, -20, -31, -50, -61, -30, -21, -40},
		{-10, -11,-10, -11,-10, -11,-10, -11},
		{1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1},
		{1, 0, 1, 0, 1, 0, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 1},
		{11,10,11,10,11,10,11,10},
		{40,21,30,51,60,31,20,41}
};

static int indexOfX = -1, indexOfY = -1, // индексы той фигуры, которой ходим
			indexXForMove = -1, indexYForMove = -1; // индексы фигурой куда ходим

static bool moveOfWhite = true, moveOfBlack = false,
			checkWhite = false, // делаем шах БЕЛЫМИ
			checkBlack = false, // ставим шах ЧЁРНЫМИ
			checkmate = false; // ставим мат

static int figureForNoCheck[50] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, // индексы фигур, которыми мы можем прикрыться
			saveIndexOfCheck[50] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			saveIndexOfSecondCheck[50] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
			squaresUnderProtect[200];

static int numbersForSquaresUnderProtect = -1;

static int  numberOfElementInArray = 1, // ему будет присваеваться значение индекса в массиве saveIndexOfCheck, а значение = 1 , потому что первые два индекса хранят положение фигуры, которую можно взять
			numberOfElementInSecondArray = 1, // ему будет присваеваться значение индекса в массиве saveIndexOfSecondCheck, а значение = 1 , потому что первые два индекса хранят положение фигуры, которую можно взять
			indexYOfKing = -1, // чтобы знать индекс короля для шаха
			indexXOfKing = -1, // чтобы знать индекс короля для шаха
			indexForArrayForNoCheck = -1; // индекс для массива какими фигурами защищаемся

static bool whiteRunFromCheck = false, blackRunFromCheck = false; // нужно ли бежать от шаха королю или нет

static bool standart = true, starWars = false; // для стилей шахматной доски

static bool whiteKingNoMove = true, blackKingNoMove = true, castleWhite = false , castleBlack = false;

#pragma endregion

static bool NoDefence()
{
	bool flag = false;
	for (int i = 0; i < 50; i+= 2)
	{
		if (!flag && (figureForNoCheck[i] == indexYOfKing || figureForNoCheck[i] == -1) && (figureForNoCheck[i + 1] == indexXOfKing || figureForNoCheck[i + 1] == -1)) flag = false;
		else flag = true;
	}
	if (!flag) return true;
	else return false;
}

static bool Checkmate(HWND hWnd)
{
	//bool tmpW = moveOfWhite, tmpB = moveOfBlack;
	swap(moveOfWhite, moveOfBlack);
	IndexOfKing(false);
	checkmate = true;
	int arrm[16] = { indexYOfKing - 1, indexXOfKing,
					indexYOfKing - 1, indexXOfKing + 1,
					indexYOfKing, indexXOfKing + 1,
					indexYOfKing + 1, indexXOfKing + 1,
					indexYOfKing + 1, indexXOfKing,
					indexYOfKing + 1, indexXOfKing - 1,
					indexYOfKing, indexXOfKing - 1,
					indexYOfKing - 1, indexXOfKing - 1
	};
	if (!NoDefence())
	{
		checkmate = false;
		swap(moveOfWhite, moveOfBlack);
		return false;
	}
	else
	{
		for (int i(0); i < 16; i += 2)
		{
			indexYForMove = arrm[i];
			indexXForMove = arrm[i + 1];
			if (indexYForMove >= 0 && indexYForMove < 8 && indexXForMove >= 0 && indexXForMove < 7 && !IsItSquareForNoCheck() && 
				((!moveOfWhite && arr[indexYForMove][indexXForMove] < 2) || (!moveOfBlack && arr[indexYForMove][indexXForMove] >= 0))
				&& !IsItProtectSquares())
			{
				checkmate = false;
				swap(moveOfWhite, moveOfBlack);
				return false;
			}
		}
		if (checkmate && moveOfWhite)
		{
			HWND hwndButton = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				NULL/*L"OK"*/,      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
				1000,         // x position 
				400,         // y position 
				300,        // Button width
				100,        // Button height
				hWnd,     // Parent window
				NULL,       // No menu.
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);
			SendMessage(hwndButton, WM_SETTEXT, 0, (LPARAM)L"Шах и мат, белые выиграли");
			swap(moveOfWhite, moveOfBlack);
			return true;
		}
		else if (checkmate && moveOfBlack)
		{
			HWND hwndButton = CreateWindow(
				L"BUTTON",  // Predefined class; Unicode assumed 
				NULL/*L"OK"*/,      // Button text 
				WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
				1000,         // x position 
				400,         // y position 
				300,        // Button width
				100,        // Button height
				hWnd,     // Parent window
				NULL,       // No menu.
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
				NULL);
			SendMessage(hwndButton, WM_SETTEXT, 0, (LPARAM)L"Шах и мат, чёрные выиграли");
			swap(moveOfWhite, moveOfBlack);
			return true;

		}
	}
	
	//if (moveOfWhite && checkBlack) tmp = true;
	//else if (moveOfBlack) tmp = false;
	
	//IndexOfKing(false);


}

static void CleanProtectSquares()
{
	for (int i = 0; i < 200; i++)
	{
		squaresUnderProtect[i] = -1;
	}
	numbersForSquaresUnderProtect = -1;

}

static void ProtectSquares()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			indexOfY = i;
			indexOfX = j;
			if (!moveOfWhite && arr[indexOfY][indexOfX] > 2)
			{
				double result = floor(abs(arr[i][j])) / 10;
				int choice = result;
				switch (choice)
				{
				case 1:
					RuleForWhitePawn(false, true);
					break;
				case 2:
					RuleForKnight(false, true);
					break;
				case 3:
					RuleForBishop(false, true);
					break;
				case 4:
					RuleForRook(false, true);
					break;
				case 5:
					RuleForQueen(false, true);
					break;
				}
			}
			else if (!moveOfBlack && arr[indexOfY][indexOfX] < 0)
			{
				double result = floor(abs(arr[i][j])) / 10;
				int choice = result;
				switch (choice)
				{
				case 1:
					RuleForBlackPawn(false, true);
					break;
				case 2:
					RuleForKnight(false, true);
					break;
				case 3:
					RuleForBishop(false, true);
					break;
				case 4:
					RuleForRook(false, true);
					break;
				case 5:
					RuleForQueen(false, true);
					break;
				}
			}
		}
	}
}

static bool IsItProtectSquares()
{
	for (int i(0); i < 200; i += 2)
	{
		if (indexYForMove == squaresUnderProtect[i] && indexXForMove == squaresUnderProtect[i + 1]) return true;
	}
	return false;
}

static void MoveFigures(HWND hWnd, int lParam)
{
	static bool move = false;
	if (!move) // хода нет, жмём на фигуру
	{
		CleanProtectSquares();
		indexOfX = GET_X_LPARAM(lParam);
		indexOfY = GET_Y_LPARAM(lParam);
		FindIndexsOfClick(indexOfX, indexOfY); // при входе параметры это координаты, при выходе - индексы нажатой картинки

		if (indexOfX != -1 && indexOfY != -1 && moveOfWhite && arr[indexOfY][indexOfX] > 2) move = true; // если ход белых и фигура, на которую нажали точно белая, а не клетка и не черная фигура, ходим
		if (indexOfX != -1 && indexOfY != -1 && moveOfBlack && arr[indexOfY][indexOfX] < -2) move = true; // и наоборот
		move = true;
		if ((moveOfWhite && whiteRunFromCheck) || (moveOfBlack && blackRunFromCheck)) // если дан двойной шах и можем ходить только королём
		{
			if (moveOfWhite && (arr[indexOfY][indexOfX] == 60 || arr[indexOfY][indexOfX] == 61)) move = true;
			else if (moveOfBlack && (arr[indexOfY][indexOfX] == -60 || arr[indexOfY][indexOfX] == -61)) move = true;
			else move = false;
		}

		if (move && (checkWhite || checkBlack)) // если взялись за нужную фигуру в свой ход и был установлен шах
		{
			if (IsItFigureForNoCheck()) // фигура, которой можно закрыть шах?
			{
				move = true;
			}
			else move = false;
		}
	}
	else
	{
		indexXForMove = GET_X_LPARAM(lParam);
		indexYForMove = GET_Y_LPARAM(lParam);
		FindIndexsOfClick(indexXForMove, indexYForMove); // на входе - куда хотим переместить фигуру, на выходе - индексы для перемещения
		IndexOfKing(false); // определяем перед изменением чей ход индексы положения короля, а вдруг мы ему шах дали

		if (checkWhite || checkBlack) // дан шах, смотрим, куда ходить
		{
			if ((arr[indexOfY][indexOfX] == 60 || arr[indexOfY][indexOfX] == 61) || (arr[indexOfY][indexOfX] == -60 || arr[indexOfY][indexOfX] == -61) && !IsItSquareForNoCheck())
			{
				move = true;
				CleanArrayForCheckAndSecondCheck();
				CleanFigureForNoCheck();
				checkWhite = checkBlack = false;
			}
			else if (IsItSquareForNoCheck()) // можно сюда ходить
			{
				if (whiteRunFromCheck || blackRunFromCheck) // если ходим только королём и ушли от шаха
				{
					whiteRunFromCheck = blackRunFromCheck = false;
				}
				CleanArrayForCheckAndSecondCheck();
				CleanFigureForNoCheck();
				checkWhite = checkBlack = false;
			}
			else move = false; //это мы делаем, если нам дан шах, а мы походили не туда
		}

		if (move && !checkmate) // сработает, если нам не дан шах или если шах дан и мы нажали на правильно поле, чтобы от него закрыться
		{
			bool tmpMove;
			if (moveOfWhite) tmpMove = true;
			else if (moveOfBlack) tmpMove = false;

			if ((moveOfWhite && arr[indexYForMove][indexXForMove] <= 1) || (moveOfBlack && arr[indexYForMove][indexXForMove] >= 0)) // не ходим на себе или на своих
			{
				ChooseRule(hWnd); // если правило сработало, переместили фигуру, и поменяли ход и наоборот

				//int tmp = indexOfY, tmp2 = indexOfX;
				ProtectSquares();
				//indexOfY = tmp;
				//indexOfX = tmp2;
				SearchCheck(tmpMove);
				SearchIndexsOfSquaresForNoCheck(tmpMove);
			}
		}
		if (moveOfWhite)
		{
			SendMessage(hwndButton, WM_SETTEXT, 0, (LPARAM)L" ");
			SendMessage(hwndButton2, WM_SETTEXT, 0, (LPARAM)L"ХОД БЕЛЫХ");
		}
		else if (moveOfBlack)
		{
			SendMessage(hwndButton, WM_SETTEXT, 0, (LPARAM)L"ХОД ЧЁРНЫХ");
			SendMessage(hwndButton2, WM_SETTEXT, 0, (LPARAM)L" ");
		}
		if (checkWhite || checkBlack)
		{
			Checkmate(hWnd);
		}
		move = false;
	}
}

static bool IsItFigureForNoCheck()
{
	for (int i(0); i < 50; i += 2)
	{
		if (indexOfY == figureForNoCheck[i] && indexOfX == figureForNoCheck[i + 1]) return true;
	}
	return false;
}

static bool IsItSquareForNoCheck()
{
	
	for (int i(0); i < 50; i += 2)
	{
		if (indexYForMove == saveIndexOfCheck[i] && indexXForMove == saveIndexOfCheck[i + 1]) return true;
	}
	return false;
}

static void ChangeOfMove(bool move) 
{
	if (move)
	{
		moveOfWhite = true;
		moveOfBlack = false;
	}
	else
	{
		moveOfWhite = false;
		moveOfBlack = true;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{
	case WM_COMMAND:
		if (lParam == (LPARAM)hwndStandartStyle)
		{
			//обработка нажатия кнопки
			standart = true;
			starWars = false;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (lParam == (LPARAM)hwndStarWars)
		{
			standart = false;
			starWars = true;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONDOWN:

		MoveFigures(hWnd, lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		BRUSH(0xFFFFFF); // уьрать
		if (standart)
		{
			DrawMatrix(hdc);
		}
		else if (starWars)
		{
			DrawMatrixStarWars(hdc);
		}	
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		return 0;
	}
}

#pragma region ForImages
HBITMAP LoadCustomImage(char *file)
{
	wchar_t wtext[1024];
	mbstowcs(wtext, file, strlen(file) + 1);
	return (HBITMAP)LoadImage(NULL, wtext, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}


void DrawBitmap(HDC hdc, HBITMAP hBitmap, int x, int y, int width, int height)
{
	HDC hdcMem;
	BITMAP bitmap;
	HGDIOBJ oldBitmap;
	hdcMem = CreateCompatibleDC(hdc);    //создаем в памяти контекст устройства memBit
	oldBitmap = SelectObject(hdcMem, hBitmap);     //cтавим контексту в соответствие битовый образ
	GetObject(hBitmap, sizeof(bitmap), &bitmap);   //получаем информацию о загруженном объектк
	BitBlt(hdc, x, y, width, height, hdcMem, 0, 0, SRCCOPY);   //вывод битового изображения
	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
}
#pragma endregion

#pragma region RulesForWhiteAndBlack


static void ChooseRule(HWND hWnd)
{
	double result = floor(abs(arr[indexOfY][indexOfX])) / 10;
	int choice = result;
	switch (choice)
	{
	case 1:
		if (moveOfWhite && RuleForWhitePawn(false, false))
		{
			//if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
			//CleanMassivForCloseSquares();
			if (!TakeFigure()) {
				ChangeColorOfFigure();
			}
			ChangeOfMove(false);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (moveOfWhite) ChangeOfMove(true); // если ход белых, но фигура походила неверно, то ход остаётся белым
		else if (moveOfBlack && RuleForBlackPawn(false, false))
		{
			//if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
			if (!TakeFigure()) {
				ChangeColorOfFigure();
			}
			ChangeOfMove(true);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (moveOfBlack) ChangeOfMove(false); // если ход черных, но фигура походила неверно, то ход остаётся черным
		
		break;
	case 2:
		if (RuleForKnight(false, false)) 
		{
		//	if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
			if (!TakeFigure()) {
				ChangeColorOfFigure();
			}

			if (moveOfWhite) ChangeOfMove(false); // если ход белых, но фигура походила верно, то ход переходит черным
			else if (moveOfBlack) ChangeOfMove(true); // если ход черных, но фигура походила верно, то ход переходит белым

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (moveOfWhite) ChangeOfMove(true); // если ход белых, но фигура походила неверно, то ход остаётся белым
		else if (moveOfBlack) ChangeOfMove(false); // если ход черных, но фигура походила неверно, то ход остаётся черным

		break;
	case 3:

		if (RuleForBishop(false, false))
		{
			//if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
			if (!TakeFigure()) {
				ChangeColorOfFigure();
			}

			if (moveOfWhite) ChangeOfMove(false); // если ход белых, но фигура походила верно, то ход переходит черным
			else if (moveOfBlack) ChangeOfMove(true); // если ход черных, но фигура походила верно, то ход переходит белым

			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (moveOfWhite) ChangeOfMove(true); // если ход белых, но фигура походила неверно, то ход остаётся белым
		else if (moveOfBlack) ChangeOfMove(false); // если ход черных, но фигура походила неверно, то ход остаётся черным

		
		break;
	case 4:
		if (RuleForRook(false, false))
		{
			//if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
			if (!TakeFigure()) {
				ChangeColorOfFigure();
			}

			if (moveOfWhite) ChangeOfMove(false); // если ход белых, но фигура походила верно, то ход переходит черным
			else if (moveOfBlack) ChangeOfMove(true); // если ход черных, но фигура походила верно, то ход переходит белым
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (moveOfWhite) ChangeOfMove(true); // если ход белых, но фигура походила неверно, то ход остаётся белым
		else if (moveOfBlack) ChangeOfMove(false); // если ход черных, но фигура походила неверно, то ход остаётся черным


		break;
	case 5:
		//queenIsGo = true;
		if (RuleForQueen(false, false))
		{
			//if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
			if (!TakeFigure()) {
				ChangeColorOfFigure();
			}

			if (moveOfWhite) ChangeOfMove(false); // если ход белых, но фигура походила верно, то ход переходит черным
			else if (moveOfBlack) ChangeOfMove(true); // если ход черных, но фигура походила верно, то ход переходит белым
			InvalidateRect(hWnd, NULL, TRUE);
		}
		//queenIsGo = false;
		else if (moveOfWhite) ChangeOfMove(true); // если ход белых, но фигура походила неверно, то ход остаётся белым
		else if (moveOfBlack) ChangeOfMove(false); // если ход черных, но фигура походила неверно, то ход остаётся черным

		
		break;
	case 6:
		if (RuleForKing())
		{
			if (!TakeFigure()) {
				ChangeColorOfFigure();
			}

			if (moveOfWhite) ChangeOfMove(false); // если ход белых, но фигура походила верно, то ход переходит черным
			else if (moveOfBlack) ChangeOfMove(true); // если ход черных, но фигура походила верно, то ход переходит белым
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else if (moveOfWhite) ChangeOfMove(true); // если ход белых, но фигура походила неверно, то ход остаётся белым
		else if (moveOfBlack) ChangeOfMove(false); // если ход черных, но фигура походила неверно, то ход остаётся черным
		InvalidateRect(hWnd, NULL, TRUE);

		break;
	}

	
}

static bool TakeFigure() // доработать взятие черными и белыми фигурами на белых и черных соответственно
{
		if ((arr[indexOfY][indexOfX] > 0 && arr[indexYForMove][indexXForMove] < 0) || (arr[indexOfY][indexOfX] < 0 && arr[indexYForMove][indexXForMove] > 2)) // фигура на белом поле бьет фигуру на чёрном поле
		{
			if (abs(arr[indexOfY][indexOfX]) % 2 == 1 && abs(arr[indexYForMove][indexXForMove]) % 2 == 0) // берём фигуру с белого поля берём на чёрном
			{
				if (arr[indexOfY][indexOfX] > 2)
				{
					arr[indexYForMove][indexXForMove] = arr[indexOfY][indexOfX] - 1;
					arr[indexOfY][indexOfX] = 1;
				}
				else
				{
					arr[indexYForMove][indexXForMove] = arr[indexOfY][indexOfX] + 1;
					arr[indexOfY][indexOfX] = 1;
				}
				return true;
			}

			if (abs(arr[indexOfY][indexOfX]) % 2 == 0 && abs(arr[indexYForMove][indexXForMove]) % 2 == 1) // берём фигуру с чёрного поля берём на белом
			{

				if (arr[indexOfY][indexOfX] > 2)
				{
					arr[indexYForMove][indexXForMove] = arr[indexOfY][indexOfX] + 1;
					arr[indexOfY][indexOfX] = 0;
				}
				else
				{
					arr[indexYForMove][indexXForMove] = arr[indexOfY][indexOfX] - 1;
					arr[indexOfY][indexOfX] = 0;
				}
				return true;
			}
			if ((abs(arr[indexOfY][indexOfX]) % 2 == 1 && abs(arr[indexYForMove][indexXForMove]) % 2 == 1) || (abs(arr[indexOfY][indexOfX]) % 2 == 0 && abs(arr[indexYForMove][indexXForMove]) % 2 == 0)) // берём фигуру с ч. поля на ч. поле и с б. на б.
			{
				if (abs(arr[indexOfY][indexOfX]) % 2 == 1)
				{
					arr[indexYForMove][indexXForMove] = arr[indexOfY][indexOfX];
					arr[indexOfY][indexOfX] = 1;
				}
				else
				{
					arr[indexYForMove][indexXForMove] = arr[indexOfY][indexOfX];
					arr[indexOfY][indexOfX] = 0;
				}
				return true;
			}
		}
		return false;
}

static void ChangeColorOfFigure() {

	if (moveOfWhite && abs(arr[indexOfY][indexOfX]) % 2 == 1  && abs(arr[indexYForMove][indexXForMove]) % 2 == 0) // фигура на белом поле, а переход на чёрное поле
	{
		arr[indexOfY][indexOfX] += -1;
		arr[indexYForMove][indexXForMove] += 1;
		swap(arr[indexOfY][indexOfX], arr[indexYForMove][indexXForMove]);
		return;
	}
	if (moveOfBlack && abs(arr[indexOfY][indexOfX]) % 2 == 1 && abs(arr[indexYForMove][indexXForMove]) % 2 == 0) // фигура на белом поле, а переход на чёрное поле
	{
		arr[indexOfY][indexOfX] += 1;
		arr[indexYForMove][indexXForMove] += 1;
		swap(arr[indexOfY][indexOfX], arr[indexYForMove][indexXForMove]);
		return;
	}
	if (moveOfWhite && abs(arr[indexOfY][indexOfX]) % 2 == 0 && abs(arr[indexYForMove][indexXForMove]) % 2 == 1) // фигура на чёрном поле, а переход на белое поле
	{
		arr[indexOfY][indexOfX] += 1;
		arr[indexYForMove][indexXForMove] += -1;
		swap(arr[indexOfY][indexOfX], arr[indexYForMove][indexXForMove]);
		return;
	}
	if (moveOfBlack && abs(arr[indexOfY][indexOfX]) % 2 == 0 && abs(arr[indexYForMove][indexXForMove]) % 2 == 1) // фигура на белом поле, а переход на чёрное поле
	{
		arr[indexOfY][indexOfX] += -1;
		arr[indexYForMove][indexXForMove] += -1;
		swap(arr[indexOfY][indexOfX], arr[indexYForMove][indexXForMove]);
		return;
	}
	swap(arr[indexOfY][indexOfX], arr[indexYForMove][indexXForMove]);
}

static bool RuleForWhitePawn(bool searchCheck, bool protectSquares) // на этом моменте мы точно знаем, что ход у белых
{
	if (protectSquares)
	{
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY - 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX + 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY - 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX - 1;
		return true;
	}

	if (indexOfY == 6 && indexOfY - indexYForMove <= 2 && indexOfX == indexXForMove && indexOfY != indexYForMove) // первый ход пешки, проверка на 1 и 2 хода ходим и по той же линии
	{
		for (int i(1); i <= indexOfY - indexYForMove; i++) // если перед пешкой стоит черная фигура не можем ходить
		{
			if (arr[indexOfY - i][indexOfX] < 0) return false;
		}
		return true;
	}

	if (indexYForMove >= 0 && indexYForMove < indexOfY && indexOfX == indexXForMove && indexOfY - indexYForMove == 1) // если мы ходим просто вперёд
	{
		if (arr[indexYForMove][indexXForMove] < 0) return false; // если спереди есть пешка
		else return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//	Сохраняем индексы для закрытия от шаха
	if (searchCheck) {

		if (indexOfY - indexYForMove == 1 && (indexXForMove == indexOfX - 1 || indexXForMove == indexOfX + 1) && (arr[indexYForMove][indexXForMove] == -61 || arr[indexYForMove][indexXForMove] == -60))
		{
			if (!checkWhite) // если мы ещё не дали шах
			{
				checkWhite = true;
				saveIndexOfCheck[0] = indexOfY;
				saveIndexOfCheck[1] = indexOfX;
			}
			else if (checkWhite)
			{
				saveIndexOfSecondCheck[0] = indexOfY;
				saveIndexOfSecondCheck[1] = indexOfX;
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////

	if (indexOfY - indexYForMove == 1 && (indexXForMove == indexOfX - 1 || indexXForMove == indexOfX + 1) && arr[indexYForMove][indexXForMove] < 0) return true; // если поле куда мы переходим стоит вражеская фигура

	return false;
	//return true;
}

static bool RuleForBlackPawn(bool searchCheck, bool protectSquares)
{

	if (protectSquares)
	{
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY + 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX + 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY + 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX - 1;
		return true;
	}

	if (indexOfY == 1 && indexYForMove - indexOfY <= 2 && indexOfX == indexXForMove && indexOfY != indexYForMove) // первый ход пешки, проверка на 1 и 2 хода ходим и по той же линии
	{
		for (int i(1); i <= indexYForMove - indexOfY; i++) // если перед пешкой стоит черная фигура не можем ходить
		{
			if (arr[indexOfY + i][indexOfX] > 2) return false;
		}
		return true;
	}
	if (indexYForMove <= 7 && indexYForMove > indexOfY && indexOfX == indexXForMove && indexYForMove - indexOfY == 1) // если мы ходим просто вперёд
	{
		if (arr[indexYForMove][indexXForMove] > 2) return false; // если спереди есть пешка
		else return true;
	} 

	///////////////////////////////////////////////////////////////////////////////////////////////
	//	Сохраняем индексы для закрытия от шаха
	if (searchCheck)
	{
		if (indexYForMove - indexOfY == 1 && (indexXForMove == indexOfX - 1 || indexXForMove == indexOfX + 1) && (arr[indexYForMove][indexXForMove] == 61 || arr[indexYForMove][indexXForMove] == 60))
		{
			if (!checkBlack) // если мы ещё не дали шах
			{
				checkBlack = true;
				saveIndexOfCheck[0] = indexOfY;
				saveIndexOfCheck[1] = indexOfX;
			}
			else if (checkBlack)
			{
				saveIndexOfSecondCheck[0] = indexOfY;
				saveIndexOfSecondCheck[1] = indexOfX;
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////

	if (indexYForMove - indexOfY == 1 && (indexXForMove == indexOfX - 1 || indexXForMove == indexOfX + 1) && arr[indexYForMove][indexXForMove] > 2)  return true; // если поле куда мы переходим стоит вражеская фигура
		
	return false;
}

static bool RuleForKnight(bool searchCheck, bool protectSquares)
{

	if (protectSquares)
	{
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY + 2;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX - 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY + 2;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX + 1;

		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY - 2;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX - 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY - 2;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX + 1;

		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY - 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX + 2;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY - 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX + 2;

		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY - 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX - 2;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY - 1;
		squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX - 2;

		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	//	Сохраняем индексы для закрытия от шаха
	if (searchCheck)
	{
		if ((indexYForMove == indexOfY + 2 && (indexXForMove == indexOfX - 1 || indexXForMove == indexOfX + 1)) || (indexYForMove == indexOfY - 2 && (indexXForMove == indexOfX + 1 || indexXForMove == indexOfX - 1)))
		{
			if (!checkWhite && !moveOfWhite && (arr[indexYForMove][indexXForMove] == -61 || arr[indexYForMove][indexXForMove] == -60)) // дали белым конём шах и это первый шах
			{
				checkWhite = true;
				saveIndexOfCheck[0] = indexOfY;
				saveIndexOfCheck[1] = indexOfX;
			}
			else if (checkWhite && !moveOfWhite && (arr[indexYForMove][indexXForMove] == -61 || arr[indexYForMove][indexXForMove] == -60)) // дали белым конём шах
			{
				saveIndexOfSecondCheck[0] = indexOfY;
				saveIndexOfSecondCheck[1] = indexOfX;
			}
			else if (!checkBlack && !moveOfBlack && (arr[indexYForMove][indexXForMove] == 61 || arr[indexYForMove][indexXForMove] == 60))
			{
				checkBlack = true;
				saveIndexOfCheck[0] = indexOfY;
				saveIndexOfCheck[1] = indexOfX;
			}
			else if (checkBlack && !moveOfBlack && (arr[indexYForMove][indexXForMove] == 61 || arr[indexYForMove][indexXForMove] == 60))
			{
				saveIndexOfSecondCheck[0] = indexOfY;
				saveIndexOfSecondCheck[1] = indexOfX;
			}
		}

		if ((indexXForMove == indexOfX + 2 && (indexYForMove == indexOfY - 1 || indexYForMove == indexOfY + 1)) || (indexXForMove == indexOfX - 2 && (indexYForMove == indexOfY + 1 || indexYForMove == indexOfY - 1)))
		{
			if (!checkWhite && !moveOfWhite && (arr[indexYForMove][indexXForMove] == -61 || arr[indexYForMove][indexXForMove] == -60)) // дали белым конём шах и это первый шах
			{
				checkWhite = true;
				saveIndexOfCheck[0] = indexOfY;
				saveIndexOfCheck[1] = indexOfX;
			}
			else if (checkWhite && !moveOfWhite && (arr[indexYForMove][indexXForMove] == -61 || arr[indexYForMove][indexXForMove] == -60)) // дали белым конём шах
			{
				saveIndexOfSecondCheck[0] = indexOfY;
				saveIndexOfSecondCheck[1] = indexOfX;
			}
			else if (!checkBlack && !moveOfBlack && (arr[indexYForMove][indexXForMove] == 61 || arr[indexYForMove][indexXForMove] == 60))
			{
				checkBlack = true;
				saveIndexOfCheck[0] = indexOfY;
				saveIndexOfCheck[1] = indexOfX;
			}
			else if (checkBlack && !moveOfBlack && (arr[indexYForMove][indexXForMove] == 61 || arr[indexYForMove][indexXForMove] == 60))
			{
				saveIndexOfSecondCheck[0] = indexOfY;
				saveIndexOfSecondCheck[1] = indexOfX;
			}
		}


	}
	///////////////////////////////////////////////////////////////////////////////////////////////

	if ((indexYForMove == indexOfY + 2 && (indexXForMove == indexOfX - 1 || indexXForMove == indexOfX + 1)) || (indexYForMove == indexOfY - 2 && (indexXForMove == indexOfX + 1 || indexXForMove == indexOfX - 1)))
	{ // здесь правило, когда конь ходит больше вверх или вниз, чем вправо или влево
		return true;
	}

	if ((indexXForMove == indexOfX + 2 && (indexYForMove == indexOfY - 1 || indexYForMove == indexOfY + 1)) || (indexXForMove == indexOfX - 2 && (indexYForMove == indexOfY + 1 || indexYForMove == indexOfY - 1))) 
	{	// здесь правило, когда конь ходит больше вправо или влево, чем вверх или вниз
		return true;
	}

	return false;
}

static bool RuleForBishop(bool searchCheck, bool protectSquares)
{
	
	if (protectSquares)
	{

		for (int i = indexOfY - 1, j = indexOfX + 1; i >= 0 && i < 8 && j >= 0 && j < 8 && (arr[i][j] == 0 || arr[i][j] == 1); i--, j++)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = j;
			if (i-1 >= 0 && i-1 < 8 && j+1 >= 0 && j+1 < 8 && arr[i-1][j+1] != 0 && arr[i-1][j+1] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i-1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = j+1;
			}
		}
		for (int i = indexOfY - 1, j = indexOfX - 1; i >= 0 && i < 8 && j >= 0 && j < 8 && (arr[i][j] == 0 || arr[i][j] == 1); i--, j--)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = j;
			if (i - 1 >= 0 && i - 1 < 8 && j - 1 >= 0 && j - 1 < 8 && arr[i - 1][j - 1] != 0 && arr[i - 1][j - 1] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i - 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = j - 1;
			}
		}
		for (int i = indexOfY + 1, j = indexOfX + 1; i >= 0 && i < 8 && j >= 0 && j < 8 && (arr[i][j] == 0 || arr[i][j] == 1); i++, j++)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = j;
			if (i + 1 >= 0 && i + 1 < 8 && j + 1 >= 0 && j + 1 < 8 && arr[i + 1][j + 1] != 0 && arr[i + 1][j + 1] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i + 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = j + 1;
			}
		}
		for (int i = indexOfY + 1, j = indexOfX - 1; i >= 0 && i < 8 && j >= 0 && j < 8 && (arr[i][j] == 0 || arr[i][j] == 1); i++, j--)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = j;
			if (i + 1 >= 0 && i + 1 < 8 && j - 1 >= 0 && j - 1 < 8 && arr[i + 1][j - 1] != 0 && arr[i + 1][j - 1] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i + 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = j - 1;
			}
		}
		return true;
	}

	if (indexOfY > indexYForMove && indexOfX < indexXForMove) // ходим вверх направо
	{
		for (int i = indexOfY - 1, j = indexOfX + 1; i >= 0 && i < 8 && j >= 0 && j < 8; i--, j++)   // начинаем проход не с самой фигуру на клетку поодаль
		{

			if (i != indexYForMove || j != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // К этому моменту мы уже отдали ход на противоложную сторону, поэтому !moveOfWhite стоит
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][j] > 2 || arr[i][j] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					return false;
				}

			}
			else 
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
		
	}
	else if (indexOfY > indexYForMove && indexOfX > indexXForMove)// ходим вверх налево  
	{
		for (int i = indexOfY - 1, j = indexOfX - 1; i >= 0 && i < 8 && j >= 0 && j < 8; i--, j--) // начинаем проход не с самой фигуру на клетку поодаль
		{

			if (i != indexYForMove || j != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][j] > 2 || arr[i][j] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{

					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					return false;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}
		}
		CleanArrayForCheckAndSecondCheck();	
		
	}
	else if (indexOfX < indexXForMove && indexOfY < indexYForMove) // ходим вниз направо 
	{
		for (int i = indexOfY + 1, j = indexOfX + 1; i >= 0 && i < 8 && j >= 0 && j < 8; i++, j++)  // начинаем проход не с самой фигуру на клетку поодаль
		{

			if (i != indexYForMove || j != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][j] > 2 || arr[i][j] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					return false;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}
		}
		CleanArrayForCheckAndSecondCheck();
	}
	else if (indexOfX > indexXForMove && indexOfY < indexYForMove) // ходим вниз налево  
	{
		for (int i = indexOfY + 1, j = indexOfX - 1; i >= 0 && i < 8 && j >= 0 && j < 8; i++, j--) // начинаем проход не с самой фигуру на клетку поодаль
		{

			if (i != indexYForMove || j != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][j] > 2 || arr[i][j] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					return false;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	}
		
	return false;
}

static bool RuleForRook(bool searchCheck, bool protectSquares)
{

	if (protectSquares)
	{

		for (int i = indexOfY - 1; i >= 0 && (arr[i][indexOfX] == 0 || arr[i][indexOfX] == 1); i--)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX;
			if (i - 1 >= 0 && arr[i - 1][indexOfX] != 0 && arr[i - 1][indexOfX] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i - 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX;
			}
		}
		for (int i = indexOfY + 1; i < 8 && (arr[i][indexOfX] == 0 || arr[i][indexOfX] == 1); i++)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX; 
			if (i + 1 < 8  && arr[i + 1][indexOfX] != 0 && arr[i + 1][indexOfX] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i + 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX;
			}
		}
		for (int i = indexOfX + 1 ; i < 8 && (arr[indexOfY][i] == 0 || arr[indexOfY][i] == 1); i++)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			if (i + 1 < 8 && arr[indexOfY][i] != 0 && arr[indexOfY][i] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i + 1;
			}
		}
		for (int i = indexOfX - 1; i >= 0 && (arr[indexOfY][i] == 0 || arr[indexOfY][i] == 1); i--)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			if (i - 1 >= 0 && arr[indexOfY][i] != 0 && arr[indexOfY][i] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i - 1;
			}
		}
		return true;
	}


	if (indexOfY > indexYForMove && indexOfX == indexXForMove) // вверх
	{
		for (int i = indexOfY - 1; i >= 0; i--)
		{

			if (i != indexYForMove || indexOfX != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][indexOfX] > 2 || arr[i][indexOfX] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					return false;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	}
	else if (indexOfY < indexYForMove && indexOfX == indexXForMove) // вверх
	{	
		for (int i = indexOfY + 1; i < 8; i++)
		{

			if (i != indexYForMove || indexOfX != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][indexOfX] > 2 || arr[i][indexOfX] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					return false;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	}
	else if (indexOfX < indexXForMove && indexOfY == indexYForMove) // вправо
	{
		for (int i = indexOfX + 1; i < 8; i++)
		{

			if (indexOfY != indexYForMove || i != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = indexOfY;
						saveIndexOfCheck[++numberOfElementInArray] = i;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfY;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = indexOfY;
						saveIndexOfCheck[++numberOfElementInArray] = i;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfY;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[indexOfY][i] > 2 || arr[indexOfY][i] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					return false;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	}
	else if (indexOfX > indexXForMove && indexOfY == indexYForMove) // влево
	{
		for (int i = indexOfX - 1; i >= 0; i--)
		{

			if (indexOfY != indexYForMove || i != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = indexOfY;
						saveIndexOfCheck[++numberOfElementInArray] = i;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfY;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = indexOfY;
						saveIndexOfCheck[++numberOfElementInArray] = i;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfY;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[indexOfY][i] > 2 || arr[indexOfY][i] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					return false;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	}
	return false;
}
	
static bool RuleForQueen(bool searchCheck, bool protectSquares)
{
	if (protectSquares)
	{
		for (int i = indexOfY - 1, j = indexOfX + 1; i >= 0 && i < 8 && j >= 0 && j < 8 && (arr[i][j] == 0 || arr[i][j] == 1); i--, j++)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = j;
			if (i - 1 >= 0 && i - 1 < 8 && j + 1 >= 0 && j + 1 < 8 && arr[i - 1][j + 1] != 0 && arr[i - 1][j + 1] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i - 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = j + 1;
			}
		}
		for (int i = indexOfY - 1, j = indexOfX - 1; i >= 0 && i < 8 && j >= 0 && j < 8 && (arr[i][j] == 0 || arr[i][j] == 1); i--, j--)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = j;
			if (i - 1 >= 0 && i - 1 < 8 && j - 1 >= 0 && j - 1 < 8 && arr[i - 1][j - 1] != 0 && arr[i - 1][j - 1] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i - 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = j - 1;
			}
		}
		for (int i = indexOfY + 1, j = indexOfX + 1; i >= 0 && i < 8 && j >= 0 && j < 8 && (arr[i][j] == 0 || arr[i][j] == 1); i++, j++)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = j;
			if (i + 1 >= 0 && i + 1 < 8 && j + 1 >= 0 && j + 1 < 8 && arr[i + 1][j + 1] != 0 && arr[i + 1][j + 1] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i + 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = j + 1;
			}
		}
		for (int i = indexOfY + 1, j = indexOfX - 1; i >= 0 && i < 8 && j >= 0 && j < 8 && (arr[i][j] == 0 || arr[i][j] == 1); i++, j--)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = j;
			if (i + 1 >= 0 && i + 1 < 8 && j - 1 >= 0 && j - 1 < 8 && arr[i + 1][j - 1] != 0 && arr[i + 1][j - 1] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i + 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = j - 1;
			}
		}
		for (int i = indexOfY - 1; i >= 0 && (arr[i][indexOfX] == 0 || arr[i][indexOfX] == 1); i--)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX;
			if (i - 1 >= 0 && arr[i - 1][indexOfX] != 0 && arr[i - 1][indexOfX] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i - 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX;
			}
		}
		for (int i = indexOfY + 1; i < 8 && (arr[i][indexOfX] == 0 || arr[i][indexOfX] == 1); i++)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX;
			if (i + 1 < 8 && arr[i + 1][indexOfX] != 0 && arr[i + 1][indexOfX] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i + 1;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfX;
			}
		}
		for (int i = indexOfX + 1; i < 8 && (arr[indexOfY][i] == 0 || arr[indexOfY][i] == 1); i++)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			if (i + 1 < 8 && arr[indexOfY][i] != 0 && arr[indexOfY][i] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i + 1;
			}
		}
		for (int i = indexOfX - 1; i >= 0 && (arr[indexOfY][i] == 0 || arr[indexOfY][i] == 1); i--)
		{
			squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY;
			squaresUnderProtect[++numbersForSquaresUnderProtect] = i;
			if (i - 1 >= 0 && arr[indexOfY][i] != 0 && arr[indexOfY][i] != 1)
			{
				squaresUnderProtect[++numbersForSquaresUnderProtect] = indexOfY;
				squaresUnderProtect[++numbersForSquaresUnderProtect] = i - 1;
			}
		}
		return true;
	}


	if (indexOfY > indexYForMove && indexOfX < indexXForMove) // ходим вверх направо
	{
		for (int i = indexOfY - 1, j = indexOfX + 1; i >= 0 && i < 8 && j >= 0 && j < 8; i--, j++)   // начинаем проход не с самой фигуру на клетку поодаль
		{


			if (i != indexYForMove || j != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][j] > 2 || arr[i][j] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	} 
	else if (indexOfY > indexYForMove&& indexOfX > indexXForMove)// ходим вверх налево  
	{
		for (int i = indexOfY - 1, j = indexOfX - 1; i >= 0 && i < 8 && j >= 0 && j < 8; i--, j--) // начинаем проход не с самой фигуру на клетку поодаль
		{

			if (i != indexYForMove || j != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][j] > 2 || arr[i][j] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	}
	else if (indexOfX < indexXForMove && indexOfY < indexYForMove) // ходим вниз направо 
	{
		for (int i = indexOfY + 1, j = indexOfX + 1; i >= 0 && i < 8 && j >= 0 && j < 8; i++, j++)  // начинаем проход не с самой фигуру на клетку поодаль
		{
			
			if (i != indexYForMove || j != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][j] > 2 || arr[i][j] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}
		}
		CleanArrayForCheckAndSecondCheck();

	}
	else if (indexOfX > indexXForMove&& indexOfY < indexYForMove) // ходим вниз налево  
	{
		for (int i = indexOfY + 1, j = indexOfX - 1; i >= 0 && i < 8 && j >= 0 && j < 8; i++, j--) // начинаем проход не с самой фигуру на клетку поодаль
		{
			if (i != indexYForMove || j != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = j;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = j;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][j] > 2 || arr[i][j] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}
		}
		CleanArrayForCheckAndSecondCheck();
	}


	if (indexOfY > indexYForMove && indexOfX == indexXForMove) // вверх
	{
		for (int i = indexOfY - 1; i >= 0; i--)
		{
			if (i != indexYForMove || indexOfX != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][indexOfX] > 2 || arr[i][indexOfX] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	}
	else if (indexOfY < indexYForMove && indexOfX == indexXForMove) // вверх
	{
		for (int i = indexOfY + 1; i < 8; i++)
		{

			if (i != indexYForMove || indexOfX != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = i;
						saveIndexOfCheck[++numberOfElementInArray] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[i][indexOfX] > 2 || arr[i][indexOfX] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	}
	else if (indexOfX < indexXForMove && indexOfY == indexYForMove) // вправо
	{
		for (int i = indexOfX + 1; i < 8; i++)
		{

			if (indexOfY != indexYForMove || i != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = indexOfY;
						saveIndexOfCheck[++numberOfElementInArray] = i;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfY;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = indexOfY;
						saveIndexOfCheck[++numberOfElementInArray] = i;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfY;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[indexOfY][i] > 2 || arr[indexOfY][i] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	}
	else if (indexOfX > indexXForMove && indexOfY == indexYForMove) // влево
	{
		for (int i = indexOfX - 1; i >= 0; i--)
		{

			if (indexOfY != indexYForMove || i != indexXForMove) // "пункт назначения достигнут", можно туда ходить
			{

				///////////////////////////////////////////////////////////////////////////////////////////////
				//	Сохраняем индексы для закрытия от шаха, не ставим при этом checkWhite and checkBlack и не очищаем здесь
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite) // не дали ещё шах белыми и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = indexOfY;
						saveIndexOfCheck[++numberOfElementInArray] = i;
					}
					else if (!moveOfWhite && checkWhite) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfY;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
					}
					else if (!moveOfBlack && !checkBlack) // не дали ещё шах чёрными и заносим всё в первый массив
					{
						saveIndexOfCheck[++numberOfElementInArray] = indexOfY;
						saveIndexOfCheck[++numberOfElementInArray] = i;
					}
					else if (!moveOfBlack && checkBlack) // шах уже был дан какой-то фигурой, значит это вторая фигура, которая даёт шах
					{
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = indexOfY;
						saveIndexOfSecondCheck[++numberOfElementInSecondArray] = i;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////

				if (arr[indexOfY][i] > 2 || arr[indexOfY][i] < 0) // если ход слона или ладьи до пункта назначения перегорожен, то не ходим
				{
					///////////////////////////////////////////////////////////////////////////////////////////////
					//	Очищаем массивы. так как король не был достигнут
					if (searchCheck) CleanArrayForCheckAndSecondCheck();
					///////////////////////////////////////////////////////////////////////////////////////////////
					break;
				}

			}
			else
			{
				///////////////////////////////////////////////////////////////////////////////////////////////
				// Устанавливаем в первые два элемента массива фигуру, которую дала шаха, чтобы побить её и избавиться от шаха, ТАКЖЕ УСТАНАВЛИВАЕМ checkWhite and checkBlack
				if (searchCheck)
				{
					if (!moveOfWhite && !checkWhite)
					{
						checkWhite = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfWhite && checkWhite)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && !checkBlack)
					{
						checkBlack = true;
						saveIndexOfCheck[0] = indexOfY;
						saveIndexOfCheck[1] = indexOfX;
					}
					else if (!moveOfBlack && checkBlack)
					{
						saveIndexOfSecondCheck[0] = indexOfY;
						saveIndexOfSecondCheck[1] = indexOfX;
					}
				}
				///////////////////////////////////////////////////////////////////////////////////////////////
				return true;
			}

		}
		CleanArrayForCheckAndSecondCheck();
	}

	return false;
}

static bool RuleForKing()
{
	if (abs(indexOfY - indexYForMove) <= 1 && abs(indexOfX - indexXForMove) <= 1)
	{
		if (arr[indexYForMove][indexXForMove] < 2 && moveOfWhite) return true;
		if (arr[indexYForMove][indexXForMove] > -1 && moveOfBlack) return true;
	}
	
	if ((!castleBlack || !castleWhite) && (arr[indexOfY][indexOfX] == 60 && whiteKingNoMove) || (arr[indexOfY][indexOfX] == -61 && blackKingNoMove))
	{
		if (indexOfY == indexYForMove && indexOfX == indexXForMove - 2 && (arr[indexOfY][indexOfX + 1] == 0 || arr[indexOfY][indexOfX + 1] == 1) && (arr[indexOfY][indexOfX + 2] == 0 || arr[indexOfY][indexOfX + 2] == 1))
		{
			if (abs(arr[indexOfY][indexOfX + 3]) == 40 || abs(arr[indexOfY][indexOfX + 3]) == 41)
			{
				if (moveOfWhite)
				{
					castleWhite = true;
					arr[7][4] = 0;
					arr[7][5] = 41;
					arr[7][6] = 60;
					arr[7][7] = 1;
					ChangeOfMove(false);
				}
				else if (moveOfBlack)
				{
					castleBlack = true;
					arr[0][4] = 1;
					arr[0][5] = -40;
					arr[0][6] = -61;
					arr[0][7] = 0;
					ChangeOfMove(true);
				}
				
				return false;
			}
		}
		else if (indexOfY == indexYForMove && indexOfX == indexXForMove + 2 && (arr[indexOfY][indexOfX - 1] == 0 || arr[indexOfY][indexOfX - 1] == 1) && (arr[indexOfY][indexOfX - 2] == 0 || arr[indexOfY][indexOfX - 2] == 1) && (arr[indexOfY][indexOfX - 3] == 0 || arr[indexOfY][indexOfX - 3] == 1))
		{
			if (abs(arr[indexOfY][indexOfX - 4]) == 40 || abs(arr[indexOfY][indexOfX - 4]) == 41)
			{
				
				if (moveOfWhite)
				{
					castleWhite = true;
					arr[7][0] = 0;
					arr[7][2] = 60;
					arr[7][3] = 41;
					arr[7][4] = 0;
					ChangeOfMove(false);
				}
				else if (moveOfBlack)
				{
					castleBlack = true;
					arr[0][0] = 1;
					arr[0][2] = -61;
					arr[0][3] = -40;
					arr[0][4] = 1;
					ChangeOfMove(true);
				}
				return false;
			}
		}
	}
	
	return false;
}

#pragma endregion 

static void IndexOfKing(bool KingRun) 
{
	for (int i = 0; i < 8; i++) // индексы нахождения чёрного или белого короля
	{
		for (int j = 0; j < 8; j++)
		{
			if (!KingRun && moveOfWhite && (arr[i][j] == -61 || arr[i][j] == -60)) // можно ходить не только королём
			{
				indexYOfKing = i;
				indexXOfKing = j;
			}
			else if (!KingRun && moveOfBlack && (arr[i][j] == 61 || arr[i][j] == 60)) // можно ходить не только королём
			{
				indexYOfKing = i;
				indexXOfKing = j;
			}
			else if (KingRun && moveOfWhite && (arr[i][j] == 61 || arr[i][j] == 60)) // можно убегать только белым королём
			{
				indexYOfKing = i;
				indexXOfKing = j;
			}
			else if (KingRun && moveOfBlack && (arr[i][j] == -61 || arr[i][j] == -60)) // можно убегать только чёрным королём
			{
				indexYOfKing = i;
				indexXOfKing = j;
			}
			
		}
	}
}

static void SearchCheck(bool move) // если move = true то значит ход белых , если наоборот то чёрных
{
	indexYForMove = indexYOfKing;
	indexXForMove = indexXOfKing;
	for (int i = 0; i < 8; i++) // устанавливаем шах и индексы нужных для закрытия полей
	{
		for (int j = 0; j < 8; j++)
		{
			indexOfY = i;
			indexOfX = j;

			if (move && arr[i][j] > 2)
			{
				double result = floor(abs(arr[i][j])) / 10;
				int choice = result;
				switch (choice)
				{
				case 1:
					RuleForWhitePawn(true, false);
					break;
				case 2:
					RuleForKnight(true, false);
					break;
				case 3:
					RuleForBishop(true, false);
					break;
				case 4:
					RuleForRook(true, false);
					break;
				case 5:
					RuleForQueen(true, false);
					break;
				}

			}

			if (!move && arr[i][j] < 0)
			{
				double result = floor(abs(arr[i][j])) / 10;
				int choice = result;
				switch (choice)
				{
				case 1:
					RuleForBlackPawn(true, false);
					break;
				case 2:
					RuleForKnight(true, false);
					break;
				case 3:
					RuleForBishop(true, false);
					break;
				case 4:
					RuleForRook(true, false);
					break;
				case 5:
					RuleForQueen(true, false);
					break;
				}
			}
		}
	}


}

static void SearchIndexsOfSquaresForNoCheck(bool move)
{
	if (saveIndexOfCheck[0] != -1 && saveIndexOfSecondCheck[0] != -1) // когда двойной шах ходим только королём
	{
		if (move) blackRunFromCheck = true;
		else if (!move) whiteRunFromCheck = true;
	}
	else if (saveIndexOfCheck[0] != -1 && saveIndexOfSecondCheck[0] == -1) // когда обычный (не двойной) шах
	{

		int lenght = 0;/*= (numberOfElementInArray - 1) / 2 */; // количество полей. которые нужно закрыть 
		for (int i = 0; i < 50; i++) 
		{
			if (saveIndexOfCheck[i] != -1)
			{
				lenght++;
			}
			else break;
		}

		for (int z = 0; z < lenght - 1; z+=2)
		{
			indexYForMove = saveIndexOfCheck[z];
			indexXForMove = saveIndexOfCheck[z + 1];

			for (int i = 0; i < 8; i++) // устанавливаем шах
			{
				for (int j = 0; j < 8; j++)
				{

					indexOfY = i;
					indexOfX = j;

					if (move && arr[i][j] < 0) // типо ход белых, но защищаемся чёрными
					{
						double result = floor(abs(arr[i][j])) / 10;
						int choice = result;
						switch (choice)
						{
						case 1:
							if (RuleForBlackPawn(false, false))
							{
								figureForNoCheck[++indexForArrayForNoCheck] = i;
								figureForNoCheck[++indexForArrayForNoCheck] = j;
							}
							//if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
							break;
						case 2:

							if (RuleForKnight(false, false))
							{
								figureForNoCheck[++indexForArrayForNoCheck] = i;
								figureForNoCheck[++indexForArrayForNoCheck] = j;
							}
							///if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
							break;
						case 3:
							if (RuleForBishop(false, false))
							{
								figureForNoCheck[++indexForArrayForNoCheck] = i;
								figureForNoCheck[++indexForArrayForNoCheck] = j;
							}
							//if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
							break;
						case 4:

							if (RuleForRook(false, false))
							{
								figureForNoCheck[++indexForArrayForNoCheck] = i;
								figureForNoCheck[++indexForArrayForNoCheck] = j;
							}
							//if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
							break;
						case 5:
							//queenIsGo = true;
							if (RuleForQueen(false, false))
							{
								figureForNoCheck[++indexForArrayForNoCheck] = i;
								figureForNoCheck[++indexForArrayForNoCheck] = j;
							}
							//if ((moveOfWhite && !checkBlack) || (moveOfBlack && !checkWhite)) CleanMassivForCloseSquares();
							//queenIsGo = false;
							break;

						case 6:
							figureForNoCheck[++indexForArrayForNoCheck] = i;
							figureForNoCheck[++indexForArrayForNoCheck] = j;
							break;
						}
					}

					if (!move && arr[i][j] > 2) // формально ход чёрных, но закрываться надо белыми
					{
						double result = floor(abs(arr[i][j])) / 10;
						int choice = result;
						switch (choice)
						{
						case 1:
							if (RuleForWhitePawn(false, false))
							{
								figureForNoCheck[++indexForArrayForNoCheck] = i;
								figureForNoCheck[++indexForArrayForNoCheck] = j;
							}
							break;
						case 2:

							if (RuleForKnight(false, false))
							{
								figureForNoCheck[++indexForArrayForNoCheck] = i;
								figureForNoCheck[++indexForArrayForNoCheck] = j;
							}
							break;
						case 3:
							if (RuleForBishop(false, false))
							{
								figureForNoCheck[++indexForArrayForNoCheck] = i;
								figureForNoCheck[++indexForArrayForNoCheck] = j;
							}
							break;
						case 4:

							if (RuleForRook(false, false))
							{
								figureForNoCheck[++indexForArrayForNoCheck] = i;
								figureForNoCheck[++indexForArrayForNoCheck] = j;
							}
							break;
						case 5:
							if (RuleForQueen(false, false))
							{
								figureForNoCheck[++indexForArrayForNoCheck] = i;
								figureForNoCheck[++indexForArrayForNoCheck] = j;
							}
							
							break;
						case 6:
							figureForNoCheck[++indexForArrayForNoCheck] = i;
							figureForNoCheck[++indexForArrayForNoCheck] = j;
							break;
						}
					}
				}
			}
		}
	}
}

static bool MoveFigureForNoCheck()
{
	for (int i = 0; i < 50; i+=2)
	{
		if (indexOfY == figureForNoCheck[i] && indexOfX == figureForNoCheck[i + 1]) return true;
	}
	return false;
}

#pragma region Clean

static void CleanArrayForCheckAndSecondCheck()
{
	if (/*!(checkWhite || checkBlack) && */saveIndexOfCheck[0] != -1 && saveIndexOfSecondCheck[2] != -1) // неправильно был установлен второй шах
	{
		for (int i(0); i < 50; i++)
		{
			saveIndexOfSecondCheck[i] = -1;
		}
	}
	else if (!(checkWhite || checkBlack) && saveIndexOfCheck[2] != -1 && saveIndexOfSecondCheck[0] == -1)
	{
		for (int i(0); i < 50; i++)
		{
			saveIndexOfCheck[i] = -1;
		}
	}
	numberOfElementInArray = numberOfElementInSecondArray = 1;
}

static void CleanFigureForNoCheck()
{
	for (int i = 0; i < 50; i++) figureForNoCheck[i] = -1;
}
#pragma endregion


#pragma region WhereFigure?
static void FindIndexsOfClick(int& indexOfX, int& indexOfY)
{
	int xPosition = indexOfX, yPosition = indexOfY;
	if (xPosition > 800 || yPosition > 800) 
	{
		indexOfX = indexOfY = -1;
		return;
	}
	indexOfX = indexOfY = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (xPosition >= i * 100 && xPosition <= i * 100 + 100 && yPosition >= j * 100 && yPosition <= j * 100 + 100) {
				indexOfX = i;
				indexOfY = j;
				break;
			}
		}
	}
	
}

#pragma endregion

#pragma region ChessBoard
static void DrawMatrix(HDC hdc)
{
	int xPosition = 100, yPosition = 100;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			int currentXPos = xPosition * j;
			int currentYPos = yPosition * i;
			switch (arr[i][j])
			{
			case 0:		DrawBitmap(hdc, hFrames[25], currentXPos, currentYPos, 100, 100); break;
			case 1:		DrawBitmap(hdc, hFrames[24], currentXPos, currentYPos, 100, 100); break;
			case 10:	DrawBitmap(hdc, hFrames[22], currentXPos, currentYPos, 100, 100); break;
			case -10:	DrawBitmap(hdc, hFrames[10], currentXPos, currentYPos, 100, 100); break;
			case 11:	DrawBitmap(hdc, hFrames[23], currentXPos, currentYPos, 100, 100); break;
			case -11:	DrawBitmap(hdc, hFrames[11], currentXPos, currentYPos, 100, 100); break;
			case 20:	DrawBitmap(hdc, hFrames[13], currentXPos, currentYPos, 100, 100); break;
			case -20:	DrawBitmap(hdc, hFrames[1], currentXPos, currentYPos, 100, 100); break;
			case 21:	DrawBitmap(hdc, hFrames[20], currentXPos, currentYPos, 100, 100); break;
			case -21:	DrawBitmap(hdc, hFrames[8], currentXPos, currentYPos, 100, 100); break;
			case 30:	DrawBitmap(hdc, hFrames[19], currentXPos, currentYPos, 100, 100); break;
			case -30:	DrawBitmap(hdc, hFrames[7], currentXPos, currentYPos, 100, 100); break;
			case 31:	DrawBitmap(hdc, hFrames[14], currentXPos, currentYPos, 100, 100); break;
			case -31:	DrawBitmap(hdc, hFrames[2], currentXPos, currentYPos, 100, 100); break;
			case 40:	DrawBitmap(hdc, hFrames[21], currentXPos, currentYPos, 100, 100); break;
			case -40:	DrawBitmap(hdc, hFrames[9], currentXPos, currentYPos, 100, 100); break;
			case 41:	DrawBitmap(hdc, hFrames[12], currentXPos, currentYPos, 100, 100); break;
			case -41:	DrawBitmap(hdc, hFrames[0], currentXPos, currentYPos, 100, 100); break;
			case 50:	DrawBitmap(hdc, hFrames[15], currentXPos, currentYPos, 100, 100); break;
			case -50:	DrawBitmap(hdc, hFrames[3], currentXPos, currentYPos, 100, 100); break;
			case 51:	DrawBitmap(hdc, hFrames[16], currentXPos, currentYPos, 100, 100); break;
			case -51:	DrawBitmap(hdc, hFrames[4], currentXPos, currentYPos, 100, 100); break;
			case 60:	DrawBitmap(hdc, hFrames[17], currentXPos, currentYPos, 100, 100); break;
			case -60:	DrawBitmap(hdc, hFrames[5], currentXPos, currentYPos, 100, 100); break;
			case 61:	DrawBitmap(hdc, hFrames[18], currentXPos, currentYPos, 100, 100); break;
			case -61:	DrawBitmap(hdc, hFrames[6], currentXPos, currentYPos, 100, 100); break;
			default:	break;
			}
		}
}

static void DrawMatrixStarWars(HDC hdc)
{
	int xPosition = 100, yPosition = 100;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			int currentXPos = xPosition * j;
			int currentYPos = yPosition * i;
			switch (arr[i][j])
			{
			case 0:		DrawBitmap(hdc, hFramesStarWars[25], currentXPos, currentYPos, 100, 100); break;
			case 1:		DrawBitmap(hdc, hFramesStarWars[24], currentXPos, currentYPos, 100, 100); break;
			case 10:	DrawBitmap(hdc, hFramesStarWars[22], currentXPos, currentYPos, 100, 100); break;
			case -10:	DrawBitmap(hdc, hFramesStarWars[10], currentXPos, currentYPos, 100, 100); break;
			case 11:	DrawBitmap(hdc, hFramesStarWars[23], currentXPos, currentYPos, 100, 100); break;
			case -11:	DrawBitmap(hdc, hFramesStarWars[11], currentXPos, currentYPos, 100, 100); break;
			case 20:	DrawBitmap(hdc, hFramesStarWars[13], currentXPos, currentYPos, 100, 100); break;
			case -20:	DrawBitmap(hdc, hFramesStarWars[1], currentXPos, currentYPos, 100, 100); break;
			case 21:	DrawBitmap(hdc, hFramesStarWars[20], currentXPos, currentYPos, 100, 100); break;
			case -21:	DrawBitmap(hdc, hFramesStarWars[8], currentXPos, currentYPos, 100, 100); break;
			case 30:	DrawBitmap(hdc, hFramesStarWars[19], currentXPos, currentYPos, 100, 100); break;
			case -30:	DrawBitmap(hdc, hFramesStarWars[7], currentXPos, currentYPos, 100, 100); break;
			case 31:	DrawBitmap(hdc, hFramesStarWars[14], currentXPos, currentYPos, 100, 100); break;
			case -31:	DrawBitmap(hdc, hFramesStarWars[2], currentXPos, currentYPos, 100, 100); break;
			case 40:	DrawBitmap(hdc, hFramesStarWars[21], currentXPos, currentYPos, 100, 100); break;
			case -40:	DrawBitmap(hdc, hFramesStarWars[9], currentXPos, currentYPos, 100, 100); break;
			case 41:	DrawBitmap(hdc, hFramesStarWars[12], currentXPos, currentYPos, 100, 100); break;
			case -41:	DrawBitmap(hdc, hFramesStarWars[0], currentXPos, currentYPos, 100, 100); break;
			case 50:	DrawBitmap(hdc, hFramesStarWars[15], currentXPos, currentYPos, 100, 100); break;
			case -50:	DrawBitmap(hdc, hFramesStarWars[3], currentXPos, currentYPos, 100, 100); break;
			case 51:	DrawBitmap(hdc, hFramesStarWars[16], currentXPos, currentYPos, 100, 100); break;
			case -51:	DrawBitmap(hdc, hFramesStarWars[4], currentXPos, currentYPos, 100, 100); break;
			case 60:	DrawBitmap(hdc, hFramesStarWars[17], currentXPos, currentYPos, 100, 100); break;
			case -60:	DrawBitmap(hdc, hFramesStarWars[5], currentXPos, currentYPos, 100, 100); break;
			case 61:	DrawBitmap(hdc, hFramesStarWars[18], currentXPos, currentYPos, 100, 100); break;
			case -61:	DrawBitmap(hdc, hFramesStarWars[6], currentXPos, currentYPos, 100, 100); break;
			default:	break;
			}
		}
}

 #pragma endregion