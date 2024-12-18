//FILENAME: BOBanker.cpp 
//AUTHOR: JJ Giesey
//EMAIL: jjgiesey@milligan.edu
//PURPOSE: Provides banking and other functions for B&O Board Game
//VERSION: 3.11 Current windows production version
//DATE: 11/20/24
//Revisions
//3.10 Revisions:
//		Fixed "Pittsburgh"
//		Added warning for already claimed coal
//		Updated About box
//3.11 Revisions
//Fixed so devalued companies would drop to bottom of trunorder stack
#include "stdafx.h"
#include "BOBanker.h"
#include "Players.h"
#include "Cities.h"
#include "Companies.h"
#include "TechLevels.h"
#include "Trains.h"
#include "Game.h"



#define MAX_LOADSTRING 100

#define BUTTON_IDENTIFIER 1

void setupPlayers();
//Precondition: None
//Postcondition: After running all the players will be loaded into the class players
// with initial values and turn orders set


void setPlayerNetworth();
//Precondition: players object has current cash and shares. Companies has current valuations
//Postcondition: players networth is calculated.


bool attemptBuy(int player, int company, int shares, int orphans, HWND hwnd);
//Precondition: player < numberPlayers
//Postcondition: returns 1 if successfull and handles all the buying transactions

bool attemptSell(int player, int company, int shares);
// This function will return a 1 when the player has sucessfully sold a stock for their turn.

bool attemptOrphan(int player, int company, int value, HWND hwnd);
// This function will return a 1 when a player has successfully had a company buy back orphaned stock.

int reduceForOrphans();
//Preconditions:To be run at end of market round
//Postconditions: Valuation of companies that have orphan shares
//				is reduced by one level.

int setCompanyPresident(int company);
// Preconditions: company is a vaild company. The shares ownd by players is passed through playerShares and the current president it passed
// through companyPresident
// Postcondidtion: The number of shares owned by each player is compared and if a new player has more shares their number is returned through setCompanyPresident.
// If not the old president is passed.

int updateInfo();
//Precondition: None
//Postcondition: Current values for player cash and networth and company cash are displayed in
//				edit control boxes 

int updateBuild(int company);
//Precondition: The index of the current company is passed in company
//Postcondition: The build window is refreshed for the current company.

int updateProfits(int company);

int incrementTurn(bool success);
//Precondition: The routine is passed whether or not the operation was successful
// in success.
//Postcondition: The function will change turn to the next index to use
// in the order array to select the curren company/player.


void setCompanyDividens(int currentCompany, int option);
//Preconditions: For the current company given in currentCompany to pay dividens to shareholders and itself. Uses companyNetprofit, companySharesowned, and playerShares.
//Postcondition: Profits distributed to companyCash and playerCash

void setCompanyValuation(int company, int netProfitChange, char companyDividenPlan);
//Preconditions: For the company given by CurrentCompany that had a net profit change given by netProfitChange (decrease = -1, increase = 1
// and same = 0) and the decision to withhold (w) or distribute (d) dividens given in companyDividenPlan. Function also uses companyValuations
//Postcondition: Valuation of company is decrease if it had a decrease in net profit. It increase if net profit increased and dividens were paied. 
//Otherwise valuation stays the same.

char readValue(ifstream& inputStream, char cArray[]);
//Precondtions: Open stream in in inputStrean and character array is returned in cArray
//Postcondition: Will return last character read.

char findEOL(ifstream& inputStream);
//Precondition: Stream to be read for end of line is in inputStream
//				It is expected that all relevant values have been read
//Postcondition: Stream cleared. Last character returned. Should be /n if successful

int checkOrphans(int player);
int cityModInit(int company, HWND mxDlg);
int cityModOK(int company, HWND mXDlg);
int trainModInit(int company, HWND mTDlg);
int trainModOK(int company, HWND mTDlg);
int saveState();

// Data processing 

int fetchFileData(wchar_t pfileName[]);

int getManualData();

int TextToMonitor(wchar_t theText[]);

int pushFileData(wchar_t pfileName[]);
int pushAnalytics();
int openAnalytics();
int openMonitor();
int pushMonitor(wchar_t theText[]);
int endGame();



// Global Handles and such
HWND marketDlg;
HWND buildDlg;
HWND infoDlg;
HWND mainDlg;
HINSTANCE hInst;                                // current instance
HBRUSH hbrBkgrnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Objects
Trains trains(0);
TechLevels techLevels;
Cities cities(0);

Players players;// (0);
Players playersOld[10];
Players playersNew;

Companies companies;
Companies cBuild;
Companies companiesOld[10];
Companies companiesNew;

Game game;
Game gameOld[10];
Game gameNew;

// Global Variables:


int numberPlayers = 3;

wchar_t wfileName[50];

int playerTurnorder[maxPlayers];
int playerTurnorderOld[10][maxPlayers];
int playerTurnorderNew[maxPlayers];
int companyTurnorder[maxCompanies+1]; 
int companyTurnorderOld[10][maxCompanies+1]; 
int companyTurnorderNew[maxCompanies+1];
int modCompany;
int requestValuation;
//int numberPasses = 0;
int profitList[20];
int oldTrackCost;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PlayersP(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Filename(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Market(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Info(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    BuyShares(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SellShares(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Orphans(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    InitValuation(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Build(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ModPlay(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ModComp(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ModCity(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ModTrain(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BOBANKER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOBANKER));
    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOBANKER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BOBANKER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_MAXIMIZE);
   UpdateWindow(hWnd);

   return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	TCHAR greeting[] = _T("Hello World!");
//	DWORD Error;
    switch (message)
    {
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:

		if (wmId == IDM_ABOUT)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		}
		else if (wmId == ID_FILE_FILENAME)
		{

//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		}
		else if (wmId == IDM_START_NEWGAME)
		{
			game.setRound(1);
			game.setTurn(0);
			companies.setTechLevel(1);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_FILENAME), hWnd, Filename);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_PLAYERBOX), hWnd, PlayersP);
			openAnalytics();
			openMonitor();
			saveState();
			CreateDialog(hInst, MAKEINTRESOURCE(IDD_INFOBOX), hWnd, Info);
		}
		else if (wmId == IDM_START_SAVEDGAME)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_FILENAME), hWnd, Filename);
			ifstream inDataStream;
			inDataStream.open(wfileName);
			if (inDataStream.fail())
			{
				MessageBox(hWnd, L"File name not valid. Try again.", L"File Error", MB_OK);
				return(1);
			}
			fetchFileData(wfileName);
			companies.setTechLevel(0);
			players.getTurnorders(playerTurnorder, numberPlayers);
			companies.getTurnorders(companyTurnorder);
			CreateDialog(hInst, MAKEINTRESOURCE(IDD_INFOBOX), hWnd, Info);
		}
		else if (wmId == IDM_MODIFY_PLAYERS)
		{
			CreateDialog(hInst, MAKEINTRESOURCE(IDD_MOD_PLAY), hWnd, ModPlay);
		}
		else if (wmId == IDM_MODIFY_COMPANIES)
		{
			CreateDialog(hInst, MAKEINTRESOURCE(IDD_MOD_COMP), hWnd, ModComp);
		}
		else if ((wmId >= ID_CITIES_BO)&&(wmId < ID_CITIES_BO+maxCompanies))
		{
			modCompany = wmId - ID_CITIES_BO;
			CreateDialog(hInst, MAKEINTRESOURCE(IDD_MOD_CITY), hWnd, ModCity);
			break;
		}
		else if ((wmId >= ID_TRAINS_BO) && (wmId < ID_TRAINS_BO + maxCompanies))
		{
			modCompany = wmId - ID_TRAINS_BO;
			CreateDialog(hInst, MAKEINTRESOURCE(IDD_MOD_TRAIN), hWnd, ModTrain);
			break;
		}
		else if (wmId == ID_UNDO_UNDO)
		{
			wchar_t name[30];
			wchar_t buildList[100];
			if (gameOld[1].getRound() == 0)
			{
				MessageBox(hWnd, L"Data not stored.", L"Error", MB_OK);
				break;
			}
			else if (gameOld[1].getRound() == 1)
			{
				wcscpy_s(name, playersOld[1].name[playerTurnorder[gameOld[1].getTurn()]]);
			}
			else
			{
				wcscpy_s(name, companiesOld[1].name[companyTurnorder[gameOld[1].getTurn()]]);

			}
			wcscpy_s(buildList, L"Undo turn of ");
			wcscat_s(buildList, name);
			if (MessageBox(hWnd,buildList, L"Undo", MB_OKCANCEL) != 2)
			{
				companiesNew = companies;
				playersNew = players;
				gameNew = game;
				for (int j = 0; j < maxPlayers; j++)
				{
					playerTurnorderNew[j] = playerTurnorder[j];
				}
				for (int j = 0; j < maxCompanies; j++)
				{
					companyTurnorderNew[j] = companyTurnorder[j];
				}
				companies = companiesOld[1];
				players = playersOld[1];
				game = gameOld[1];
				for (int j = 0; j < maxPlayers; j++)
				{
					playerTurnorder[j] = playerTurnorderOld[1][j];
				}
				for (int j = 0; j < maxCompanies; j++)
				{
					companyTurnorder[j] = companyTurnorderOld[1][j];
				}


				EndDialog(marketDlg, LOWORD(wParam));
				EndDialog(buildDlg, LOWORD(wParam));
				if (game.getRound() == 1)
				{
					BringWindowToTop(marketDlg);
					SetWindowText(GetDlgItem(marketDlg, IDC_CURRENT_PLAYER), (LPCWSTR)players.name[playerTurnorder[game.getTurn()]]);
				}
				else
				{

					updateBuild(companyTurnorder[game.getTurn()]);
				}
				for (int i = 1; i < 9; i++)
				{
					playersOld[i - 1] = playersOld[i];
					companiesOld[i - 1] = companiesOld[i];
					gameOld[i - 1] = gameOld[i];
					for (int j = 0; j < maxPlayers; j++)
					{
						playerTurnorderOld[i - 1][j] = playerTurnorderOld[i][j];
					}
					for (int j = 0; j < maxCompanies; j++)
					{
						companyTurnorderOld[i - 1][j] = companyTurnorderOld[i][j];
					}
				}
	/*			if (game.getRound() == 1)
				{
					BringWindowToTop(marketDlg);
					SetWindowText(GetDlgItem(marketDlg, IDC_CURRENT_PLAYER), (LPCWSTR)players.name[playerTurnorder[game.getTurn()]]);
				}
				else
				{
					updateBuild(companyTurnorder[game.getTurn()]);
				}*/
				updateInfo();
			}
			break;
		}
		else if (wmId == ID_UNDO_REDO)
		{
			wchar_t name[30];
			wchar_t buildList[100];

			if (gameOld[1].getRound() == 1)
			{
				wcscpy_s(name, playersOld[1].name[playerTurnorder[gameOld[1].getTurn()]]);
			}
			else
			{
				wcscpy_s(name, companiesOld[1].name[companyTurnorder[gameOld[1].getTurn()]]);

			}
			wcscpy_s(buildList, L"Redo turn of ");
			wcscat_s(buildList, name);
			if (MessageBox(hWnd, buildList, L"Undo", MB_OKCANCEL) != 2)
			{
				companies = companiesNew;
				players = playersNew;
				game = gameNew;
				for (int j = 0; j <= maxPlayers; j++)
				{
					playerTurnorder[j] = playerTurnorderNew[j];
				}
				for (int j = 0; j <= maxCompanies; j++)
				{
					companyTurnorder[j] = companyTurnorderNew[j];
				}
				updateInfo();
			}
			break;
		}
		else if (wmId == IDM_EXIT)
		{
		DestroyWindow(hWnd);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	break;
	case WM_LBUTTONDOWN:
	{
		TCHAR szFileName[MAX_PATH];
		HINSTANCE hInstance = GetModuleHandle(NULL);
		GetModuleFileName(hInstance, szFileName, MAX_PATH);
		MessageBox(hWnd, szFileName, greeting, MB_OK | MB_ICONINFORMATION);
	}
	break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(4));
//			TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
INT_PTR CALLBACK About(HWND abDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(abDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Filename(HWND fNDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		wchar_t name[50];
		wcscpy_s(name, L"bodata.csv");
		SetWindowText(GetDlgItem(fNDlg, IDC_FILENAME), (LPCWSTR) name);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_OK)
		{
			int textLength;
			wchar_t name[50];
			// Read Player Names
			textLength = GetWindowTextLength(GetDlgItem(fNDlg, IDC_FILENAME)) + 1;
			textLength = GetWindowText(GetDlgItem(fNDlg, IDC_FILENAME), name, textLength);
			wcscpy_s(wfileName, name);
			EndDialog(fNDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDC_CANCEL)
		{
			EndDialog(fNDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK PlayersP(HWND pBDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDDONE)
		{
			int textLength;
			wchar_t name[20];
			// Read Player Names
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP1))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP1), name, textLength);
			wcscpy_s(players.name[0], name);
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP2))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP2), name, textLength);
			wcscpy_s(players.name[1], name);
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP3))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP3), name, textLength);
			wcscpy_s(players.name[2], name);
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP4))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP4), name, textLength);
			wcscpy_s(players.name[3], name);
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP5))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP5), name, textLength);
			wcscpy_s(players.name[4], name);
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP6))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP6), name, textLength);
			wcscpy_s(players.name[5], name);
			setupPlayers();
			players.getTurnorders(playerTurnorder, numberPlayers);
			EndDialog(pBDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_CHECKP4:
			EnableWindow(GetDlgItem(pBDlg, IDC_EDITP4), TRUE);
			EnableWindow(GetDlgItem(pBDlg, IDC_CHECKP5), TRUE);
			numberPlayers = 4;
			break;
		case IDC_CHECKP5:
			EnableWindow(GetDlgItem(pBDlg, IDC_EDITP5), TRUE);
			EnableWindow(GetDlgItem(pBDlg, IDC_CHECKP6), TRUE);
			numberPlayers = 5;
			break;
		case IDC_CHECKP6:
			EnableWindow(GetDlgItem(pBDlg, IDC_EDITP6), TRUE);
			numberPlayers = 6;
			break;
		}
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Info(HWND iNDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	//	wchar_t cashStr[10];
	infoDlg = iNDlg;


	HWND hwndList = GetDlgItem(iNDlg, IDC_LIST_CURRENT_PLAYER);
	HWND hwndLP2 = GetDlgItem(iNDlg, IDC_LISTPLAYER2);
	HWND hwndCP = GetDlgItem(iNDlg, IDC_CURRENT_PLAYER);
	switch (message)
	{

	case WM_INITDIALOG:
	{
		HFONT bigFont;
		bigFont = CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));

		wchar_t cashStr[20];
		setPlayerNetworth();
		for (int i = 0; i < numberPlayers; i++)
		{
			int cPlayer = playerTurnorder[i];
			ShowWindow(GetDlgItem(iNDlg, IDC_PN1 + i), SW_SHOW);
			SendMessage(GetDlgItem(iNDlg, IDC_PN1 + i), WM_SETFONT, (WPARAM)bigFont, 1);
			SetWindowText(GetDlgItem(iNDlg, IDC_PN1 + i), (LPCWSTR)players.name[cPlayer]);
			_itow_s(players.getCash(cPlayer), cashStr, 10);
			ShowWindow(GetDlgItem(iNDlg, IDC_PC1 + i), SW_SHOW);
			SendMessage(GetDlgItem(iNDlg, IDC_PC1 + i), WM_SETFONT, (WPARAM)bigFont, 1);
			SetWindowText(GetDlgItem(iNDlg, IDC_PC1 + i), (LPCWSTR)cashStr);
			_itow_s(players.getNetworth(cPlayer), cashStr, 10);
			ShowWindow(GetDlgItem(iNDlg, IDC_PW1 + i), SW_SHOW);
			SendMessage(GetDlgItem(iNDlg, IDC_PW1 + i), WM_SETFONT, (WPARAM)bigFont, 1);
			SetWindowText(GetDlgItem(iNDlg, IDC_PW1 + i), (LPCWSTR)cashStr);
			for (int j = 0; j < 10; j++)
			{
				_itow_s(players.getShares(cPlayer, j), cashStr, 10);
				ShowWindow(GetDlgItem(iNDlg, j * (maxPlayers + 2) + IDC_PW7 + i), SW_SHOW);
				SendMessage(GetDlgItem(iNDlg, j * (maxPlayers + 2) + IDC_PW7 + i), WM_SETFONT, (WPARAM)bigFont, 1);
				SetWindowText(GetDlgItem(iNDlg, j * (maxPlayers + 2) + IDC_PW7 + i), (LPCWSTR)cashStr);
			}
		}
		for (int j = 0; j < 10; j++)
		{
			_itow_s(companies.getShares(j), cashStr, 10);
			ShowWindow(GetDlgItem(iNDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers), SW_SHOW);
			SendMessage(GetDlgItem(iNDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers), WM_SETFONT, (WPARAM)bigFont, 1);
			SetWindowText(GetDlgItem(iNDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers), (LPCWSTR)cashStr);
			_itow_s(companies.getOrphans(j), cashStr, 10);
			ShowWindow(GetDlgItem(iNDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers + 1), SW_SHOW);
			SendMessage(GetDlgItem(iNDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers + 1), WM_SETFONT, (WPARAM)bigFont, 1);
			SetWindowText(GetDlgItem(iNDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers + 1), (LPCWSTR)cashStr);

		}
		int tempTurnOrder[maxCompanies] = { 0,1,2,3,4,5,6,7,8,9 };
		companies.getTurnorders(tempTurnOrder);
		for (int j = 0; j < maxCompanies; j++)
		{
			int jc = tempTurnOrder[j];
			SendMessageW(GetDlgItem(iNDlg, IDC_BBO + j), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1 + jc)));
			if ((companies.getStarted(jc))&&(!companies.getRecievership(jc)))
			{
				_itow_s(companies.getCash(jc), cashStr, 10);
				ShowWindow(GetDlgItem(iNDlg, IDC_CC0 + j), SW_SHOW);
				SendMessage(GetDlgItem(iNDlg, IDC_CC0 + j), WM_SETFONT, (WPARAM)bigFont, 1);
				SetWindowText(GetDlgItem(iNDlg, IDC_CC0 + j), (LPCWSTR)cashStr);
				_itow_s(valuations[companies.getValuation(jc)], cashStr, 10);
				ShowWindow(GetDlgItem(iNDlg, IDC_CV0 + j), SW_SHOW);
				SendMessage(GetDlgItem(iNDlg, IDC_CV0 + j), WM_SETFONT, (WPARAM)bigFont, 1);
				SetWindowText(GetDlgItem(iNDlg, IDC_CV0 + j), (LPCWSTR)cashStr);
				_itow_s(companies.getNetprofit(jc), cashStr, 10);
				ShowWindow(GetDlgItem(iNDlg, IDC_CC9 + j), SW_SHOW);
				SendMessage(GetDlgItem(iNDlg, IDC_CC9 + j), WM_SETFONT, (WPARAM)bigFont, 1);
				SetWindowText(GetDlgItem(iNDlg, IDC_CC9 + j), (LPCWSTR)cashStr);
				wchar_t trainList[100];
				wcscpy_s(trainList, L"");
				for (int k = 0; k < companies.getTrainCount(jc); k++)
				{
					_itow_s(companies.getTrainOwned(jc,k)+1, cashStr, 10);
					wcscat_s(trainList, cashStr);
					wcscat_s(trainList, L" ");
				}
				ShowWindow(GetDlgItem(iNDlg, IDC_CTB1 + j), SW_SHOW);
				SendMessage(GetDlgItem(iNDlg, IDC_CTB1 + j), WM_SETFONT, (WPARAM)bigFont, 1);
				SetWindowText(GetDlgItem(iNDlg, IDC_CTB1 + j), (LPCWSTR)trainList);
			}
		}
		SendMessage(GetDlgItem(infoDlg, IDC_MONITOR), EM_SETLIMITTEXT, (WPARAM)512000,1);
		wchar_t monitorText[40];
		wcscpy_s(monitorText, L"ALL ABOARD!!!! Here we go.\n");
		TextToMonitor(monitorText);
	return TRUE;
	}
	case WM_PAINT:
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_NEXT:
		{
			if (game.getRound() == 1)
			{
				CreateDialog(hInst, MAKEINTRESOURCE(IDD_MARKETBOX), iNDlg, Market);
			}
			else
			{
				CreateDialog(hInst, MAKEINTRESOURCE(IDD_BUILDBOX1), iNDlg, Build);
			}
			return TRUE;
		}
		case IDC_CANCEL:
			EndDialog(iNDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK Market(HWND maDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	HWND hwndCP = GetDlgItem(maDlg, IDC_CURRENT_PLAYER);
	switch (message)
	{ 
	case WM_INITDIALOG:
	{
		HFONT bigFont;
		bigFont = CreateFont(40, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |FF_SWISS, TEXT("Arial"));
		SendMessage(hwndCP, WM_SETFONT, (WPARAM)bigFont, 1);
		SetWindowText(hwndCP, (LPCWSTR)players.name[playerTurnorder[game.getTurn()]]);
		game.setMarketSuccess(FALSE);
		game.setBuySuccess(FALSE);
		marketDlg = maDlg;


		return TRUE;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(maDlg, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(4));
		//			TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
		// TODO: Add any drawing code that uses hdc here...h
		EndPaint(maDlg, &ps);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_BUY:
	
			DialogBox(hInst, MAKEINTRESOURCE(IDD_BUYBOX), maDlg, BuyShares);
			if (game.getBuySuccess()) //If bought move on to next turn
			{
				incrementTurn(true);
				game.setMarketSuccess(FALSE);
				updateInfo();
				BringWindowToTop(maDlg);
				SetWindowText(hwndCP, (LPCWSTR)players.name[playerTurnorder[game.getTurn()]]);
			}
			break;
		case IDC_BUTTON_SELL:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SELLBOX), maDlg, SellShares);
			BringWindowToTop(maDlg);
			break;
		case IDC_BUTTON_ORPHANS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ORPHANBOX), maDlg, Orphans);
			if (game.getBuySuccess())//If bought move on to next turn
			{
				updateInfo();
				incrementTurn(true);
				game.setMarketSuccess(FALSE);
				BringWindowToTop(maDlg);
				SetWindowText(hwndCP, (LPCWSTR)players.name[playerTurnorder[game.getTurn()]]);
			}
		break;
		case IDC_BUTTON_DONE:
			if (checkOrphans(playerTurnorder[game.getTurn()]) != 0)
			{
				if (MessageBox(maDlg, L"You are the president of a company with orphan shares. \n Hit CANCEL to return to market.\n Hit OK to continue. ", L"Valuation Warning", MB_OKCANCEL) == 2)
				{
					return TRUE;
				}
			}
			if (incrementTurn(game.getMarketSuccess()) == 0)
			{
				BringWindowToTop(maDlg);
				SetWindowText(hwndCP, (LPCWSTR)players.name[playerTurnorder[game.getTurn()]]);	
			}
			else
			{
				EndDialog(maDlg, LOWORD(wParam));
				return TRUE;
			}		
			break;
		case IDC_CANCEL:
			EndDialog(maDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK Build(HWND bDDlg, UINT message,WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		buildDlg = bDDlg;

		updateBuild(companyTurnorder[game.getTurn()]);

		return TRUE;
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		

		int company = companyTurnorder[game.getTurn()];
		HDC hdcStatic = (HDC)wParam;
		HWND hwdStatic = (HWND)lParam;

		for (int k = 0; k < maxCities; k++)
		{
			HWND hXC = GetDlgItem(buildDlg, IDC_XS0 + k);
			if ((hXC == hwdStatic) && (companies.getCity(company, k)))
			{
				SetBkMode(hdcStatic, TRANSPARENT);
				SetTextColor(hdcStatic, companies.color[company]);
				hbrBkgrnd = CreateSolidBrush(RGB(235, 235, 255));
				return (INT_PTR) hbrBkgrnd;
			}
		}
		for (int k = 0; k < maxCoal; k++)
		{
			HWND hXC = GetDlgItem(buildDlg, IDC_XC0 + k);
			if ((hXC == hwdStatic) && (companies.getCoal(company, k)))
			{
				SetBkMode(hdcStatic, TRANSPARENT);
				SetTextColor(hdcStatic, companies.color[company]);
				hbrBkgrnd = CreateSolidBrush(RGB(235, 235, 255));
				return (INT_PTR)hbrBkgrnd;
			}
		}
		return TRUE;
	}

	case WM_VSCROLL:
	{
		wchar_t intStr[100];
		int company = companyTurnorder[game.getTurn()];
		HWND hwndST = GetDlgItem(bDDlg, IDC_SPIN_TRACK);
		HWND hwndSL = GetDlgItem(bDDlg, IDC_SPIN_TECH);
		HWND hwndNC = GetDlgItem(bDDlg, IDC_NETCASH);
		UINT val1 = LOWORD(SendMessage(hwndST, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		SetDlgItemInt(bDDlg, IDC_TRACK_TO_BUY, val1 * 10, true);
		cBuild.changeCash(company, oldTrackCost-val1 * 10); // Reduces cash by the change in track costs
		oldTrackCost = val1 * 10; //Records current track cost to determine next change
		UINT val2 = LOWORD(SendMessage(hwndSL, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		SetDlgItemInt(bDDlg, IDC_TECH, val2, true);
		cBuild.setTechLevel(val2);
		_itow_s(cBuild.getCash(company), intStr, 10);
		SetWindowText(hwndNC, intStr);
		updateProfits(company);
		break;
	}
	case WM_COMMAND:
		wchar_t intStr[100];
		HWND hwndST = GetDlgItem(bDDlg, IDC_SPIN_TRACK);
		HWND hwndNC = GetDlgItem(bDDlg, IDC_NETCASH);
		int option = LOWORD(wParam);
		int box;
		int company = companyTurnorder[game.getTurn()];
		box = IDC_RCB0 + maxCompanies;
		if ((option >= IDC_TS0) && (option < IDC_TS0 + 5)) // Scrap Trains
		{
			int train = option - IDC_TS0;
			if (IsDlgButtonChecked(bDDlg, option))
			{
				cBuild.changeCash(company,cBuild.scrapTrainOfCompany(companies.getTrainOwned(company,train ),company)); //scrap train and increase cash
			}
			else
			{
				cBuild.changeCash(company, -cBuild.unscrapTrainOfCompany(companies.getTrainOwned(company, train), company)); //unscrap train and decrease cash
			}
			_itow_s(cBuild.getCash(company), intStr, 10);
			SetWindowText(hwndNC, intStr);
			updateProfits(company);
		}
		if ((option >= IDC_TB0) && (option < IDC_TB0 + 5)) // Buy Trains
		{
			int train = cBuild.getTrainForSale();
			if (IsDlgButtonChecked(bDDlg, option))
			{
				cBuild.changeCash(company, -cBuild.buyTrainForSale(company)); //scrap train and increase cash
				ShowWindow(GetDlgItem(bDDlg, option + 1), SW_SHOW);
				ShowWindow(GetDlgItem(bDDlg, IDC_TBN0 + option - IDC_TB0 + 1), SW_SHOW);
				SetDlgItemInt(bDDlg, IDC_TBN0 + option - IDC_TB0 + 1, cBuild.getTrainForSale()+1, true);

			}
			else
			{
				cBuild.changeCash(company, cBuild.returnTrainForSale(company)); //scrap train and increase cash
				ShowWindow(GetDlgItem(bDDlg, option + 1), SW_HIDE);
				ShowWindow(GetDlgItem(bDDlg, IDC_TBN0 + option - IDC_TB0 +1), SW_HIDE);
			}
			_itow_s(cBuild.getCash(company), intStr, 10);
			SetWindowText(hwndNC, intStr);
			// Reset TechLevel
			HWND hwndSL = GetDlgItem(bDDlg, IDC_SPIN_TECH);
			SendMessage(hwndSL, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(6, cBuild.getTechLevel()));
			SendMessage(hwndSL, UDM_SETPOS, (WPARAM)0, (LPARAM)cBuild.getTechLevel());
			UINT val2 = LOWORD(SendMessage(hwndSL, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
			SetDlgItemInt(bDDlg, IDC_TECH, val2, true);
			updateProfits(company);
		}
		if ((option >= IDC_XS0) && (option < IDC_XS0 + maxCities)) // Add cities
		{
			int city = option - IDC_XS0;
			if (IsDlgButtonChecked(bDDlg, option))
			{
				cBuild.setCity(companyTurnorder[game.getTurn()],city);
				if (cBuild.getCompaniesinCity(city)>techLevels.companiesPerCity[cBuild.getTechLevel()])
					MessageBox(bDDlg, L"Companies in city would exceed current limit.", L"Build Error", 0);
			}
			else
			{
				if (companies.getCity(company, city)) // Do not allow current city to be undone
				{
					SendMessage(GetDlgItem(bDDlg, option), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
				}
				else
				{
					cBuild.clearCity(companyTurnorder[game.getTurn()], city);
				}
			}
			updateProfits(company);
		}
		if ((option >= IDC_XC0) && (option < IDC_XC0 + maxCoal))
		{
			int coal = option - IDC_XC0;
			if (IsDlgButtonChecked(bDDlg, option))
			{
				cBuild.setCoal(company, coal);
				if (companies.checkCoal(coal))
 					MessageBox(bDDlg, L"Coal has already been claimed.", L"Build Error", 0);
			}
			else
			{
				if (companies.getCoal(company, coal)) // Do not allow current coal to be undone
				{
					SendMessage(GetDlgItem(bDDlg, option), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
				}
				else
				{
					cBuild.clearCoal(company, coal);
				}
			}
			updateProfits(company);
		}
		if (option == IDC_OK)
		{
//			wchar_t infoStr[100];
			wchar_t buildList[1000];
			int netprofitChange;
			company = companyTurnorder[game.getTurn()];	
			if (cBuild.getCash(company) < 0)
			{
				MessageBox(bDDlg, L"Can not have negative cash balance. Please try again.", L"Build Error", 0);
				updateBuild(companyTurnorder[game.getTurn()]);
			}
			else
			{
				wcscpy_s(buildList, companies.name[company]);
				wcscat_s(buildList, L" building actions: \n");
				// List trains to scrap
				for (int k = 0; k < companies.getTrainCount(company); k++)
				{
					if (IsDlgButtonChecked(bDDlg, IDC_TS0 + k))
					{
						wcscat_s(buildList, L"  *Scrap train ");
						_itow_s(companies.getTrainOwned(company, k)+1, intStr, 10);
						wcscat_s(buildList, intStr);
						wcscat_s(buildList, L". \n");
					}
				}

				// List next train for sale
				for (int k = 0; k < 4; k++)
				{
					if (IsDlgButtonChecked(bDDlg, IDC_TB0 + k))
					{
						wcscat_s(buildList, L"  *Buy train ");
						_itow_s(companies.getTrainForSale() + k+1, intStr, 10);
						wcscat_s(buildList, intStr);
						wcscat_s(buildList, L". \n");
					}
				}

				int track = GetDlgItemInt(bDDlg, IDC_TRACK_TO_BUY, NULL, false);
				wcscat_s(buildList, L"  *Build $");
				_itow_s(track, intStr, 10);
				wcscat_s(buildList, intStr);
				wcscat_s(buildList, L" of track. \n");
				int profit = cBuild.getNetprofit(company);

				wcscat_s(buildList, L"  *Cities added: \n");

				for (int k = 0; k<maxCities; k++)
				{
					if (companies.getCity(company, k) != cBuild.getCity(company, k))
					{
						wcscat_s(buildList, L"   -");
						wcscat_s(buildList, cities.name[k]);
						wcscat_s(buildList, L". \n");
					}
				}
				for (int k = 0; k<maxCoal; k++)
				{
					if (companies.getCoal(company, k) != cBuild.getCoal(company, k))
					{
						wcscat_s(buildList, L" -");
						wcscat_s(buildList, cities.coals[k]);
						wcscat_s(buildList, L". \n");
					}
				}

				wcscat_s(buildList, L"  *Set net profit to $");
				_itow_s(profit, intStr, 10);
				wcscat_s(buildList, intStr);
				wcscat_s(buildList, L". \n");

				int dividend;
				if (IsDlgButtonChecked(bDDlg, IDC_DIST))
				{
					dividend = 0;
					wcscpy_s(intStr, L"  *Distribute dividiens. \n");
					wcscat_s(buildList, intStr);

				}
				else
				{
					dividend = 1;
					wcscpy_s(intStr, L"  *Withhold dividens. \n");
					wcscat_s(buildList, intStr);
				}
				int oldProfit = companies.getNetprofit(company);

				if (profit == 0)
				{
					netprofitChange = -2;
					MessageBox(bDDlg, L"Net profit of 0 will cause reduction in valuation and loss of 1 share for president.", L"Valuation Warning", MB_OK);
				}
				else if (profit > oldProfit)
				{
					netprofitChange = 1;
				}
				else if (profit < oldProfit)
				{
					netprofitChange = -1;
					MessageBox(bDDlg, L"Decrease in net profit will cause reduction in valuation.", L"Valuation Warning", MB_OK);
				}
				else
				{
					netprofitChange = 0;
				}
				if (MessageBox(bDDlg, buildList, L"Confirm Buy", MB_OKCANCEL) == 1)
				{
					if(dividend == 0)
						cBuild.setProfitPerShare(company,(cBuild.getNetprofit(company) / 10) + (valuations[cBuild.getValuation(company)+netprofitChange]- valuations[cBuild.getValuation(company)]));
					else
						cBuild.setProfitPerShare(company, (valuations[cBuild.getValuation(company) + netprofitChange] - valuations[cBuild.getValuation(company)]));
					companies = cBuild;
//					swprintf_s(infoStr, 100, L"++ %wS netprofit is now %d. \n", companies.name[company], profit);
					TextToMonitor(buildList);
					setCompanyDividens(company, dividend);
					setCompanyValuation(company, netprofitChange, dividend);
//					swprintf_s(infoStr, 100, L"++ %wS has $ %d. \n", companies.name[company], companies.getCash(company));
//					TextToMonitor(infoStr);
					updateInfo();
					if (incrementTurn(false) == 0)
					{
						updateBuild(companyTurnorder[game.getTurn()]);
					}
					else
					{
						EndDialog(bDDlg, LOWORD(wParam));
						return TRUE;
					}
				}
			}
		}
		if (option == IDC_CANCEL)
		{
			EndDialog(bDDlg, LOWORD(wParam));
			return TRUE;
		}
		if (HIWORD(wParam) == CBN_SELCHANGE) //Change has been made in the net profit list box
		{
			cBuild.setNetprofit(company, profitList[SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,(WPARAM)0, (LPARAM)0)]); //Set net profit to new value
			return TRUE;
		}
		return TRUE;
	}
	return FALSE;
} 
INT_PTR CALLBACK BuyShares(HWND bSDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	HWND hwndList2 = GetDlgItem(bSDlg, IDC_SHARES_TO_BUY1);
	HWND hwndList3 = GetDlgItem(bSDlg, IDC_SPIN_BUY1);
	HWND hwndList4 = GetDlgItem(bSDlg, IDC_SHARES_TO_BUY2);
	HWND hwndList5 = GetDlgItem(bSDlg, IDC_SPIN_BUY2);
	int Value1, Value2;
	int company;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
		SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
		for (int j = 0; j <maxCompanies; j++)
		{
			if (companies.getAvailable(j, companies.getTechLevel()))
			{
				ShowWindow(GetDlgItem(bSDlg, IDC_RCB0 + j), SW_SHOW);
				SendMessageW(GetDlgItem(bSDlg, IDC_RCB0 + j), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BOS + j)));
			}
		}
		SetDlgItemInt(bSDlg, IDC_TOTAL, 0, true);
		SetFocus(hwndList3);
		return 0;
			break;
	}
	case WM_VSCROLL:
	{
		UINT val1 = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		SetDlgItemInt(bSDlg, IDC_SHARES_TO_BUY1, val1, true);
		UINT val2 = LOWORD(SendMessage(hwndList5, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		SetDlgItemInt(bSDlg, IDC_SHARES_TO_BUY2, val2, true);
		int buyTotal = (val1 + val2)*valuations[requestValuation];
		SetDlgItemInt(bSDlg, IDC_TOTAL, buyTotal, true);

		return 0;
	}
	case WM_COMMAND:
		int option = LOWORD(wParam);
		int box;
		box = IDC_RCB0+maxCompanies;
		if ((option >= IDC_RCB0) && (option < IDC_RCB0 + maxCompanies))
		{
			company = option - IDC_RCB0;
			requestValuation = companies.getValuation(company);
			SetDlgItemInt(bSDlg, IDC_TOTAL, 0, true);
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			if (companies.getStarted(company) == 0)													//If company has not started
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_INITVALBOX), bSDlg, InitValuation);
				if ( requestValuation != -1) // Valuation selected
				{
					requestValuation = requestValuation;
				//	companies.setValuation(company, requestValuation); //Don's set valuation in case of cancelling
				}
				else // Valuation canceled
				{
					SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(0, 0)); //Clear box
					SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(0, 0));
					SendMessage(GetDlgItem(bSDlg, option), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
				}
			}
		}
		if (option == IDC_BUY)
		{
			Value1 = GetDlgItemInt(bSDlg, IDC_SHARES_TO_BUY1, NULL, false);
			Value2 = GetDlgItemInt(bSDlg, IDC_SHARES_TO_BUY2, NULL, false);
			if ((Value1 == 0) && (Value2 == 0))
			{
				MessageBox(bSDlg, L"Requesting to buy 0 shares. Please try again.", L"Buy Error", MB_OK);
				return 0;
			}
			for (int j = 0; j < maxCompanies; j++)
			{
				if (IsDlgButtonChecked(bSDlg, IDC_RCB0 + j))
				{
					company = j;
				}
			}
//			requestValuation = companies.getValuation(company); Do not set valuation yet
			if (attemptBuy(playerTurnorder[game.getTurn()], company, Value1, Value2, bSDlg))
			{
				EndDialog(bSDlg, LOWORD(wParam));
				return 1;
			}
		}
		if(option == IDC_CANCEL)
		{
			EndDialog(bSDlg, LOWORD(wParam));
			return 0;
		}
		return 0;
	}
	return 0;
}
INT_PTR CALLBACK SellShares(HWND sSDlg, UINT message,WPARAM wParam, LPARAM lParam)
{

	HWND hwndList2 = GetDlgItem(sSDlg, IDC_SHARES_TO_SELL);
	HWND hwndList3 = GetDlgItem(sSDlg, IDC_SPIN_SELL);
	int numberShares, company;
	int order = playerTurnorder[game.getTurn()];



	switch (message)
	{
	case WM_INITDIALOG:
	{
		for (int j = 0; j < players.getCompanyCount(order); j++)
		{
			ShowWindow(GetDlgItem(sSDlg, IDC_RCS0 + j), SW_SHOW);
			SendMessageW(GetDlgItem(sSDlg, IDC_RCS0 + j), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BOS + players.getCompanyOwned(order,j))));
		}
		SetDlgItemInt(sSDlg, IDC_SELLTOTAL, 0, true);
		SetFocus(hwndList2);
		return FALSE;
	}
	case WM_VSCROLL:
	{
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		SetDlgItemInt(sSDlg, IDC_STOCKS_TO_SELL, val, true);
		int sellTotal = val*valuations[requestValuation];
		SetDlgItemInt(sSDlg, IDC_SELLTOTAL, sellTotal, true);
	}
	case WM_COMMAND:
		int option = LOWORD(wParam);
		if ((option >= IDC_RCS0) && (option < IDC_RCS0 + maxCompanies))
		{
			company = players.getCompanyOwned(order, option - IDC_RCS0);
			requestValuation = companies.getValuation(company);
			SetDlgItemInt(sSDlg, IDC_SELLTOTAL, 0, true);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
		}
		if (option == IDC_SELL)
		{
			for (int j = 0; j < players.getCompanyCount(order); j++)
			{
				if (IsDlgButtonChecked(sSDlg, IDC_RCS0 + j))
				{
					company = players.getCompanyOwned(order, j);
				}
			}
			if (attemptSell(order, company, GetDlgItemInt(sSDlg, IDC_SHARES_TO_SELL, NULL, false)))
			{
				updateInfo();
				EndDialog(sSDlg, LOWORD(wParam));
				return TRUE;
			}
		}
		if (option == IDC_CANCEL)
		{
			EndDialog(sSDlg, LOWORD(wParam));
			return FALSE;
		}
		return FALSE;
	}
	return FALSE;
}
INT_PTR CALLBACK Orphans(HWND orDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(orDlg, IDC_LIST_CURRENT_PLAYER);
	HWND hwndList2 = GetDlgItem(orDlg, IDC_ORPHANS_TO_BUY);
	HWND hwndList3 = GetDlgItem(orDlg, IDC_SPIN_ORPHANS);
	int numberShares;
	int company; 
	switch (message)
	{
	case WM_INITDIALOG:
	{
		for (int j = 0; j < companies.getCompanyRunCount(playerTurnorder[game.getTurn()]); j++)
		{
			ShowWindow(GetDlgItem(orDlg, IDC_RCO0 + j), SW_SHOW);
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], j);
			SendMessageW(GetDlgItem(orDlg, IDC_RCO0 + j), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BOS + company)));
			if (companies.getOrphans(company) == 0)
			{
				ShowWindow(GetDlgItem(orDlg, IDC_RCO0 + j), SW_HIDE);
			}
		}
		SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		SetDlgItemInt(orDlg, IDC_ORPHANS_TO_BUY, val, true);
		SetFocus(hwndList2);
		break;
	}

	case WM_VSCROLL:
	{
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		SetDlgItemInt(orDlg, IDC_ORPHANS_TO_BUY, val, true);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RCO0:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 0);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO1:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 1);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO2:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 2);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO3:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 3);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO4:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 4);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO5:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 5);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO6:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 6);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO7:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 7);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO8:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 8);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO9:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 9);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_BUY:
			for (int j = 0; j < companies.getCompanyRunCount(playerTurnorder[game.getTurn()]); j++)
			{
				if (IsDlgButtonChecked(orDlg, IDC_RCO0 + j))
				{
					company = companies.getCompanyRun(playerTurnorder[game.getTurn()], j);
				}
			}
			if (attemptOrphan(playerTurnorder[game.getTurn()], company, GetDlgItemInt(orDlg, IDC_ORPHANS_TO_BUY, NULL, false), orDlg))
			{
				EndDialog(orDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
		case IDC_CANCEL:
			EndDialog(orDlg, LOWORD(wParam));
			return 0;
			break;
		}
		return 0;
	}
	return 0;
}
INT_PTR CALLBACK InitValuation(HWND iVDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{

		HWND hwndList1 = GetDlgItem(iVDlg, IDC_VAL1);
		HWND hwndList2 = GetDlgItem(iVDlg, IDC_VAL2);
		HWND hwndList3 = GetDlgItem(iVDlg, IDC_VAL3);
		CheckRadioButton(iVDlg, IDC_RADIOV1, IDC_RADIOV3, IDC_RADIOV2);
		wchar_t valStr[100];
		wchar_t intStr[100];
		wcscpy_s(valStr, L"$ ");
		_itow_s(valuations[techLevels.lowVal[companies.getTechLevel() -1]], intStr, 10);
		wcscat_s(valStr, intStr);
		wcscat_s(valStr, L" per share.");
		SetWindowText(hwndList1,valStr);
		wcscpy_s(valStr, L"$ ");
		_itow_s(valuations[techLevels.medVal[companies.getTechLevel() -1]], intStr, 10);
		wcscat_s(valStr, intStr);
		wcscat_s(valStr, L" per share.");
		SetWindowText(hwndList2, valStr);
		wcscpy_s(valStr, L"$ ");
		_itow_s(valuations[techLevels.highVal[companies.getTechLevel() -1]], intStr, 10);
		wcscat_s(valStr, intStr);
		wcscat_s(valStr, L" per share.");
		SetWindowText(hwndList3, valStr);
		return TRUE;
	}

	
	case WM_COMMAND:
		int option = LOWORD(wParam);
		if (option == IDC_RADIOV1)
		{
			for (int i = 1; i <= 10; i++)
			{
				SetDlgItemInt(iVDlg, IDC_ITC1-1+i, valuations[techLevels.lowVal[companies.getTechLevel() - 1]] * i, true);
			}
		}
		if (option == IDC_RADIOV2)
		{
			for (int i = 1; i <= 10; i++)
			{
				SetDlgItemInt(iVDlg, IDC_ITC1 - 1 + i, valuations[techLevels.medVal[companies.getTechLevel() - 1]] * i, true);
			}
		}
		if (option == IDC_RADIOV3)
		{
			for (int i = 1; i <= 10; i++)
			{
				SetDlgItemInt(iVDlg, IDC_ITC1 - 1 + i, valuations[techLevels.highVal[companies.getTechLevel() - 1]] * i, true);
			}
		}

		
		if (option == IDC_OK)
		{
			if (IsDlgButtonChecked(iVDlg, IDC_RADIOV1))
			{
				requestValuation = techLevels.lowVal[companies.getTechLevel() - 1];
			}
			else if (IsDlgButtonChecked(iVDlg, IDC_RADIOV2))
			{
				requestValuation = techLevels.medVal[companies.getTechLevel() - 1];
			}
			else if (IsDlgButtonChecked(iVDlg, IDC_RADIOV3))
			{
				requestValuation = techLevels.highVal[companies.getTechLevel() - 1];
			}
			else
				MessageBox(iVDlg, L"Must select one option.", L"ERROR", MB_OKCANCEL);
			EndDialog(iVDlg, LOWORD(wParam));
			return TRUE;
		}
		if (option == IDC_CANCEL)
		{
			requestValuation = -1;
			EndDialog(iVDlg, LOWORD(wParam));
			return FALSE;
		}
		return TRUE; 
	}
	return FALSE;
}
INT_PTR CALLBACK ModPlay(HWND mPDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		wchar_t cashStr[20];
		for (int i = 0; i < maxPlayers; i++)
		{
			SetWindowText(GetDlgItem(mPDlg, IDC_ED_PN0 + i), (LPCWSTR)players.name[i]);
			_itow_s(players.getCash(i), cashStr, 10);
			SetWindowText(GetDlgItem(mPDlg, IDC_ED_PN6 + i), (LPCWSTR)cashStr);
			_itow_s(players.getNetworth(i), cashStr, 10);
			SetWindowText(GetDlgItem(mPDlg, IDC_ED_PN12 + i), (LPCWSTR)cashStr);
			_itow_s(players.getNetworth(i), cashStr, 10);
			SetWindowText(GetDlgItem(mPDlg, IDC_ED_PN12 + i), (LPCWSTR)cashStr);
			_itow_s(players.getOrder(i), cashStr, 10);
			SetWindowText(GetDlgItem(mPDlg, IDC_ED_PN18 + i), (LPCWSTR)cashStr);
			for (int j = 0; j < maxCompanies; j++)
			{
				_itow_s(players.getShares(i, j), cashStr, 10);
				SetWindowText(GetDlgItem(mPDlg, IDC_ED_PC9 + i + maxPlayers * j), (LPCWSTR)cashStr); //Mis numbered PN24
				_itow_s(players.getSold(i, j), cashStr, 10);
				SetWindowText(GetDlgItem(mPDlg, IDC_ED_PN84 + i + maxPlayers * j), (LPCWSTR)cashStr);
			}
		}
		for (int j = 0; j < maxCompanies; j++)
		{
			SetWindowText(GetDlgItem(mPDlg, IDC_ED_PN24 + j), (LPCWSTR)companies.name[j]); //misnumbered PNC0
			SetWindowText(GetDlgItem(mPDlg, IDC_ED_PC10 + j), (LPCWSTR)companies.name[j]);
		}

		return TRUE;
	}


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			wchar_t cashStr[20];
			int textLength;
			bool bValue;

			for (int i = 0; i < maxPlayers; i++)
			{
				textLength = GetWindowTextLength(GetDlgItem(mPDlg, IDC_ED_PN0 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mPDlg, IDC_ED_PN0 + i), cashStr, textLength);
				wcscpy_s(players.name[i], cashStr);
				textLength = GetWindowTextLength(GetDlgItem(mPDlg, IDC_ED_PN6 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mPDlg, IDC_ED_PN6 + i), cashStr, textLength);
				players.setCashM(i, _wtoi(cashStr));
				textLength = GetWindowTextLength(GetDlgItem(mPDlg, IDC_ED_PN12 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mPDlg, IDC_ED_PN12 + i), cashStr, textLength);
				players.setNetworth(i, _wtoi(cashStr));
				textLength = GetWindowTextLength(GetDlgItem(mPDlg, IDC_ED_PN18 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mPDlg, IDC_ED_PN18 + i), cashStr, textLength);
				players.setOrder(i, _wtoi(cashStr));
				for (int j = 0; j < maxCompanies; j++)
				{
					textLength = GetWindowTextLength(GetDlgItem(mPDlg, IDC_ED_PC9 + i + maxPlayers * j)) + 1; //misnumbered PN24
					textLength = GetWindowText(GetDlgItem(mPDlg, IDC_ED_PC9 + i + maxPlayers * j), cashStr, textLength);
					players.setShares(i, j, _wtoi(cashStr));
					textLength = GetWindowTextLength(GetDlgItem(mPDlg, IDC_ED_PN84 + i + maxPlayers * j)) + 1;
					textLength = GetWindowText(GetDlgItem(mPDlg, IDC_ED_PN84 + i + maxPlayers * j), cashStr, textLength);
					bValue = _wtoi(cashStr);
					if (bValue)
					{
						players.setSold(i, j);
					}
					else
					{
						players.clearSold(i, j);
					}
				}
			}
			players.setTurnorders();
			updateInfo();
			EndDialog(mPDlg, LOWORD(wParam));
			return TRUE;
			break;
		case IDC_CANCEL:
			EndDialog(mPDlg, LOWORD(wParam));
			return FALSE;
			break;

		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK ModComp(HWND mCDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		wchar_t cashStr[20];
		for (int i = 0; i < maxCompanies; i++)
		{
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC10 + i), (LPCWSTR)companies.name[i]);
			_itow_s(companies.getCash(i), cashStr, 10);
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC20 + i), (LPCWSTR)cashStr);
			_itow_s(companies.getNetprofit(i), cashStr, 10);
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC30 + i), (LPCWSTR)cashStr);
			_itow_s(companies.getValuation(i), cashStr, 10);
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC40 + i), (LPCWSTR)cashStr);
			_itow_s(companies.getOrder(i), cashStr, 10);
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC50 + i), (LPCWSTR)cashStr);
			_itow_s(companies.getPresident(i), cashStr, 10);
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC60 + i), (LPCWSTR)cashStr);
			_itow_s(companies.getShares(i), cashStr, 10);
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC70 + i), (LPCWSTR)cashStr);
			_itow_s(companies.getOrphans(i), cashStr, 10);
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC80 + i), (LPCWSTR)cashStr);
			_itow_s(companies.getStarted(i), cashStr, 10);
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC90 + i), (LPCWSTR)cashStr);
			_itow_s(companies.getSold(i), cashStr, 10);
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC100 + i), (LPCWSTR)cashStr);
			_itow_s(companies.getRecievership(i), cashStr, 10);
			SetWindowText(GetDlgItem(mCDlg, IDC_ED_PC110 + i), (LPCWSTR)cashStr);
		}
		return TRUE;
	}


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			wchar_t cashStr[50];
			int textLength;
			bool bValue;

			for (int i = 0; i < maxCompanies; i++)
			{
				textLength = GetWindowTextLength(GetDlgItem(mCDlg, IDC_ED_PC20 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mCDlg, IDC_ED_PC20 + i), cashStr, textLength);
				companies.setCash(i, _wtoi(cashStr));
				cBuild.setCash(i, _wtoi(cashStr));
				textLength = GetWindowTextLength(GetDlgItem(mCDlg, IDC_ED_PC30 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mCDlg, IDC_ED_PC30 + i), cashStr, textLength);
				companies.setNetprofit(i, _wtoi(cashStr));
				cBuild.setNetprofit(i, _wtoi(cashStr));
				textLength = GetWindowTextLength(GetDlgItem(mCDlg, IDC_ED_PC40 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mCDlg, IDC_ED_PC40 + i), cashStr, textLength);
				companies.setValuationM(i, _wtoi(cashStr));
				cBuild.setValuationM(i, _wtoi(cashStr));
				textLength = GetWindowTextLength(GetDlgItem(mCDlg, IDC_ED_PC50 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mCDlg, IDC_ED_PC50 + i), cashStr, textLength);
				companies.setOrder(i, _wtoi(cashStr));
				cBuild.setOrder(i, _wtoi(cashStr));
				textLength = GetWindowTextLength(GetDlgItem(mCDlg, IDC_ED_PC60 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mCDlg, IDC_ED_PC60 + i), cashStr, textLength);
				companies.setPresident(i, _wtoi(cashStr));
				cBuild.setPresident(i, _wtoi(cashStr));
				textLength = GetWindowTextLength(GetDlgItem(mCDlg, IDC_ED_PC70 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mCDlg, IDC_ED_PC70 + i), cashStr, textLength);
				companies.setShares(i, _wtoi(cashStr));
				cBuild.setShares(i, _wtoi(cashStr));
				textLength = GetWindowTextLength(GetDlgItem(mCDlg, IDC_ED_PC80 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mCDlg, IDC_ED_PC80 + i), cashStr, textLength);
				companies.setOrphans(i, _wtoi(cashStr));
				cBuild.setOrphans(i, _wtoi(cashStr));
				textLength = GetWindowTextLength(GetDlgItem(mCDlg, IDC_ED_PC90 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mCDlg, IDC_ED_PC90 + i), cashStr, textLength);
				bValue = _wtoi(cashStr);
				if (bValue)
				{
					companies.setStarted(i);
					cBuild.setStarted(i);
				}
				else
				{
					companies.clearStarted(i);
					cBuild.clearStarted(i);
				}
				textLength = GetWindowTextLength(GetDlgItem(mCDlg, IDC_ED_PC100 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mCDlg, IDC_ED_PC100 + i), cashStr, textLength);
				bValue = _wtoi(cashStr);
				if (bValue)
				{
					companies.setSold(i);
					cBuild.setSold(i);
				}
				else
				{
					companies.clearSold(i);
					cBuild.clearSold(i);
				}
				textLength = GetWindowTextLength(GetDlgItem(mCDlg, IDC_ED_PC110 + i)) + 1;
				textLength = GetWindowText(GetDlgItem(mCDlg, IDC_ED_PC110 + i), cashStr, textLength);
				bValue = _wtoi(cashStr);
				if (bValue)
				{
					companies.setRecievership(i);
					cBuild.setRecievership(i);
				}
				else
				{
					companies.clearRecievership(i);
					cBuild.clearRecievership(i);
				}
			}
			companies.setTurnorders();
			updateInfo();
			EndDialog(mCDlg, LOWORD(wParam));
			return TRUE;
			break;
		case IDC_CANCEL:
			EndDialog(mCDlg, LOWORD(wParam));
			return FALSE;
			break;

		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK ModCity(HWND mXDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		cityModInit(modCompany, mXDlg);
		return TRUE;
	}


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			cityModOK(modCompany, mXDlg);
			EndDialog(mXDlg, LOWORD(wParam));
			return TRUE;
			break;
		case IDC_CANCEL:
			EndDialog(mXDlg, LOWORD(wParam));
			return FALSE;
			break;

		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK ModTrain(HWND mTDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		trainModInit(modCompany, mTDlg);
		return TRUE;
	}


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			trainModOK(modCompany, mTDlg);
			EndDialog(mTDlg, LOWORD(wParam));
			return TRUE;
			break;
		case IDC_CANCEL:
			EndDialog(mTDlg, LOWORD(wParam));
			return FALSE;
			break;

		}
		return TRUE;
	}
	return FALSE;
}

int TextToMonitor(wchar_t theText[])
{
	HWND hwndList = GetDlgItem(infoDlg, IDC_MONITOR);
	int ndx = GetWindowTextLength(hwndList);
	SetFocus(hwndList);

#ifdef WIN32
	SendMessage(hwndList, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
#else
	SendMessageA(hwndList, EM_SETSEL, 0, MAKELONG(ndx, ndx));
#endif
	SendMessage(hwndList, EM_REPLACESEL, 0, (LPARAM)(theText));
	pushMonitor(theText);
	return(0);
}
void setupPlayers()
{
	// Set up players names

	int tempOrder[maxPlayers] = { 0,1,2,3,4,5 };
	long int currentTime = static_cast<long int>(time(0)); //Generate random seed
	srand(currentTime);

	for (int i = 0; i < numberPlayers; i++)
	{
		players.setCash(i, rand() % 10000); // assign random number
	}
	players.getTurnorders(tempOrder, numberPlayers);

	for (int i = 0; i < numberPlayers; i++)
	{
		int player = tempOrder[i];
		players.setCash(player, totalCash / numberPlayers);
		for (int j = 0; j < maxCompanies; j++)
		{
			players.setShares(player, j, 0);
			players.clearSold(player, j);
		}
//		players.getInfo(player);
	}
}
bool attemptBuy(int player, int company, int numberShares, int numberOrphans, HWND hwnd)
{
	// Player requests to buy


	if ((numberShares + numberOrphans)*valuations[requestValuation] > players.getCash(player))
	{
		MessageBox(hwnd, L"Not enough funds to buy shares. Please try again.", L"Buy Error", MB_OK);
		return false;
	}
	else if (players.getSold(player, company))
	{
		MessageBox(hwnd, L"Can not buy shares of company sold during round. Please try again.", L"Buy Error", MB_OK);
		return false;
	}
	else
	{
		// Proceed with sale
		int test;
		test = companies.setStarted(company);
		companies.clearRecievership(company);
		test = companies.setValuation(company, requestValuation);
		test = companies.changeShares(company, -numberShares);
		test = companies.changeCash(company, (numberShares * valuations[requestValuation]));
		test = companies.changeOrphans(company, -numberOrphans);
		players.changeShares(player, company, (numberShares + numberOrphans));
		players.changeCash(player, -((numberShares + numberOrphans) * valuations[requestValuation]));
		wchar_t infoStr[100];
		swprintf_s(infoStr, 100, L"++ %wS now has  %d shares of %wS. \n", players.name[player],   players.getShares(player, company),companies.name[company]);
		TextToMonitor(infoStr);
		swprintf_s(infoStr, 100, L"++ %wS now has $%d. \n", players.name[player], players.getCash(player));
		TextToMonitor(infoStr);
		swprintf_s(infoStr, 100, L"++ %wS now has %d regualr shares and %d orphan shares. \n", companies.name[company],companies.getShares(company),companies.getOrphans(company));
		TextToMonitor(infoStr);
		swprintf_s(infoStr, 100, L"++ %wS now has $%d. \n",companies.name[company] ,companies.getCash(company));
		TextToMonitor(infoStr);
		test = setCompanyPresident(company);
		game.setMarketSuccess(TRUE);
		game.setBuySuccess(TRUE);
		return true;
	}
}
bool attemptSell(int player, int company, int numberShares)
// This function will return a 1 when the player has sucessfully sold a stock for their turn.
{
	wchar_t infoStr[300];
	if (players.getShares(player, company) == 0)
	{
		return (0);
	}
	if (numberShares > players.getShares(player, company))
	{
		return false;
	}
	else if (players.totalSharesOwned(company) <= numberShares)
	{
		swprintf_s(infoStr, 300, L"****Can not sell all shares of company %wS. Reduce request.**** \n",companies.name[company]);
		TextToMonitor(infoStr);
		return false;
	}
	else  // Proceed with sale
	{
		players.changeShares(player, company, -numberShares);
		players.changeCash(player, (numberShares * valuations[companies.getValuation(company)]));
		companies.changeOrphans(company, numberShares);
		wchar_t infoStr[300];
		swprintf_s(infoStr, 300, L"++ %wS now has $%d. \n", players.name[player],players.getCash(player));
		TextToMonitor(infoStr);
		swprintf_s(infoStr, 300, L"++ %wS now has %d shares of %wS \n", players.name[player], players.getShares(player, company),companies.name[company]);
		TextToMonitor(infoStr);
		swprintf_s(infoStr, 300, L"++ %wS now has %d regualr shares and %d orphan shares. \n",companies.name[company],companies.getShares(company),companies.getOrphans(company));
		TextToMonitor(infoStr);
		swprintf_s(infoStr, 300, L"++ %wS now has $%d. \n",companies.name[company],companies.getCash(company));
		TextToMonitor(infoStr);

		// Reduce valuation if first sale of round by player. If second or greater sale there is no additional reduction.
		if (!players.getSold(player, company))
		{
			companies.setProfitPerShare(company, companies.getProfitPerShare(company) + (valuations[companies.getValuation(company) - 1] - valuations[companies.getValuation(company)]));
			companies.decreaseValuation(company);
			swprintf_s(infoStr, 300, L"++ %wS  has been sold this turn and its valuation has been reduced to $%d per share. \n", companies.name[company], valuations[companies.getValuation(company)]);
			TextToMonitor(infoStr);
		}
		setCompanyPresident(company);
		players.setSold(player, company);
		game.setMarketSuccess(TRUE);
		return true;
	}
	return false;
}
bool attemptOrphan(int player, int company, int numberShares, HWND hwnd)
{
	wchar_t infoStr[100];
	if (numberShares*valuations[companies.getValuation(company)] > companies.getCash(company))
	{
		MessageBox(hwnd, L"Not enough funds to buy shares. Please try again.", L"Buy Error", MB_OK);
		return false;
	}
	else
	{

		companies.changeOrphans(company, -numberShares);
		companies.changeShares(company, numberShares);
		companies.changeCash(company, -numberShares * valuations[companies.getValuation(company)]);
		swprintf_s(infoStr, 100, L" %wS now has $%d and %d regular shares and %d orphan shares. \n",companies.name[company], companies.getCash(company), companies.getShares(company),companies.getOrphans(company));
		TextToMonitor(infoStr);
		game.setMarketSuccess(TRUE);
		game.setBuySuccess(TRUE);
		return true;
	}
}
int reduceForOrphans()
{
	wchar_t infoStr[100];
	for (int j = 0; j < maxCompanies; j++)
	{
		if (companies.getOrphans(j) != 0)
		{
			companies.setProfitPerShare(j, companies.getProfitPerShare(j)+(valuations[companies.getValuation(j) -1] - valuations[companies.getValuation(j)]));
			companies.decreaseValuation(j);
			swprintf_s(infoStr, 100, L"++ %wS valuation decreases to $%d per share due to orphaned shares. \n", companies.name[j], valuations[companies.getValuation(j)]);
			TextToMonitor(infoStr);
		}

	}
	return (0);
}
void setPlayerNetworth()
{
	for (int i = 0; i < numberPlayers; i++)
	{
		int sum = players.getCash(i);
		for (int j = 0; j<maxCompanies; j++)
		{
			sum += players.getShares(i, j) * valuations[companies.getValuation(j)];
		}
		players.setNetworth(i, sum);
	}
	return;
}
void setCompanyValuation(int company, int netProfitChange, char companyDividenPlan)
{
	wchar_t infoStr[100];
	if (netProfitChange < 0)
	{
		if (companies.getNetprofit(company) != 0)
		{
			companies.decreaseValuation(company);
			swprintf_s(infoStr, 100, L"++ %wS valuation decreases to $%d  per share. \n", companies.name[company],valuations[companies.getValuation(company)]);
			TextToMonitor(infoStr);
		}
		else
		{
			// Unprofitable Company
			int badPresident = companies.getPresident(company);
			companies.decreaseValuation(company);
			companies.decreaseValuation(company);
			swprintf_s(infoStr, 100, L"++ %wS valuation decreases to $%d per share. \n", companies.name[company],valuations[companies.getValuation(company)]);
			TextToMonitor(infoStr);
			companies.changeShares(company, 1);
			players.changeShares(setCompanyPresident(company), company, -1);
			swprintf_s(infoStr, 100, L"++ %wS shares in %wS has been reduced to %d shares. \n", players.name[badPresident], companies.name[company],players.getShares(badPresident, company));
			TextToMonitor(infoStr);
			if (players.getShares(setCompanyPresident(company), company) == 0)
			{
				// Company goes into recievership
				swprintf_s(infoStr, 100, L"++ %wS goes into recievership. \n", companies.name[company]);
				TextToMonitor(infoStr);
				companies.setRecievership(company); // Place into revievership
//				companies.clearStarted(company); // Clear started flag
				companies.setValuation(company, 4); //Set valuation to $50
				companies.scrapCompanyTrains(company); //Scrap all trains
				companies.setCash(company, 0); //Clear out treasurey
				companies.changeOrphans(company, -companies.getOrphans(company)); //Clear out orphans
				companies.changeShares(company, 10 - companies.getShares(company)); // Put all shares regular
				for (int j = 0; j < maxCompanies; j++)// Hide info of company
				{
						ShowWindow(GetDlgItem(infoDlg, IDC_CC0 + j), SW_HIDE);
						ShowWindow(GetDlgItem(infoDlg, IDC_CV0 + j), SW_HIDE);
						ShowWindow(GetDlgItem(infoDlg, IDC_CC9 + j), SW_HIDE);
				}
				swprintf_s(infoStr, 100, L"++ %wS now has $%d and %d regular shares and %d orphaned shares. \n", companies.name[company], companies.getCash(company), companies.getShares(company), companies.getOrphans(company));
				TextToMonitor(infoStr);
			}
		}
	}
	else if ((netProfitChange > 0) && (companyDividenPlan == 0))
	{
		companies.increaseValuation(company);
		swprintf_s(infoStr, 100, L"++ %wS valuation increases to $%d per share. \n", companies.name[company], valuations[companies.getValuation(company)]);
		TextToMonitor(infoStr);
	}
	else
	{
		swprintf_s(infoStr, 100, L"++ %wS valuation remains the same at $%d per share. \n",companies.name[company],valuations[companies.getValuation(company)]);
		TextToMonitor(infoStr);
	}
	return;
} 
void setCompanyDividens(int company, int option)
{
	wchar_t infoStr[100];
	if (option == 0)
	{
		int cDividend = (companies.getShares(company) * companies.getNetprofit(company)) / 10;
		companies.changeCash(company, cDividend);
		swprintf_s(infoStr, 100, L"++ %wS recieved $%d from %d shares and now has $%d. \n", companies.name[company] , cDividend ,companies.getShares(company), companies.getCash(company));
		TextToMonitor(infoStr);
		for (int i = 0; i < numberPlayers; i++)
		{
			if (players.getShares(i, company) != 0)
			{
				int pDividend = players.getShares(i, company)*companies.getNetprofit(company) / 10;
				players.changeCash(i, pDividend);
				swprintf_s(infoStr, 100, L"++ %wS revieved $%d from %d shares of %wS and now has $%d. \n", players.name[i],  pDividend ,players.getShares(i, company) , companies.name[company] , players.getCash(i));
				TextToMonitor(infoStr);
			}
		}
	}
	else
	{
		companies.changeCash(company, companies.getNetprofit(company));
		swprintf_s(infoStr, 100, L"++ %wS  recieved net profit of  $%d and now has $%d. \n", companies.name[company] ,companies.getNetprofit(company), companies.getCash(company));
		TextToMonitor(infoStr);
	}
	return;
}
char readValue(ifstream& inputStream, char cArray[])
{
	char getChar;
	inputStream.get(getChar);
	int j = 0;
	while ((getChar != ',') && (getChar != '\n'))
	{
		cArray[j] = getChar;
		j++;
		inputStream.get(getChar);
	}
	cArray[j] = '\0';
	return(getChar);
}
char findEOL(ifstream& inputStream)
{
	char getChar;
	do
	{
		inputStream.get(getChar);
	} while (getChar != '\n');
	return(getChar);
}
int fetchFileData(wchar_t pfileName[])
{

	char getChar;
	wchar_t infoStr[100];
	char testArray[30];
	ifstream inDataStream;

	int rnumberPlayers = 0;
	inDataStream.open(pfileName);
	if (inDataStream.fail())
	{
		swprintf_s(infoStr, 100, L"Input file stream open failed \n");
		TextToMonitor(infoStr);
		return(1);
	}
	size_t numChars=20;
	getChar = readValue(inDataStream, testArray);
	getChar = readValue(inDataStream, testArray);
	while (strcmp(testArray, "Done#") != 0)	//Read player names
	{
		mbstowcs_s(&numChars,players.name[rnumberPlayers],20,testArray,20);
		rnumberPlayers++;
		getChar = readValue(inDataStream, testArray);
	}
	numberPlayers = rnumberPlayers;
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int i = 0; i < numberPlayers; i++) // Read player cash
	{
		getChar = readValue(inDataStream, testArray);
		players.setCash(i, atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) // Read player shares owned
	{
		for (int i = 0; i < numberPlayers; i++)
		{
			getChar = readValue(inDataStream, testArray);
			players.setShares(i, j, atoi(testArray));
		}
		findEOL(inDataStream);
		getChar = readValue(inDataStream, testArray);
	}
	for (int j = 0; j < maxCompanies; j++) // Read player sold company
	{
		for (int i = 0; i < numberPlayers; i++)
		{
			getChar = readValue(inDataStream, testArray);
			if (atoi(testArray) == 1)
				players.setSold(i, j);
			else
				players.clearSold(i, j);
		}
		findEOL(inDataStream);
		getChar = readValue(inDataStream, testArray);
	}

	for (int j = 0; j < maxCompanies; j++) //Read company name (and discard since fixed)
	{
		getChar = readValue(inDataStream, testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++)//Read company cash
	{
		getChar = readValue(inDataStream, testArray);
		companies.setCash(j, atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company net profit
	{
		getChar = readValue(inDataStream, testArray);
		companies.setNetprofit(j, atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company valuation
	{
		getChar = readValue(inDataStream, testArray);
		companies.setValuation(j, atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company presidents
	{
		getChar = readValue(inDataStream, testArray);
		companies.setPresident(j, atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company shares
	{
		getChar = readValue(inDataStream, testArray);
		companies.changeShares(j, atoi(testArray) - companies.getShares(j));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read comapny orphans
	{
		getChar = readValue(inDataStream, testArray);
		companies.changeOrphans(j, atoi(testArray) - companies.getOrphans(j));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company started
	{
		getChar = readValue(inDataStream, testArray);
		if (atoi(testArray) == 1)
			companies.setStarted(j);
		else
			companies.clearStarted(j);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company recievership
	{
		getChar = readValue(inDataStream, testArray);
		if (atoi(testArray) == 1)
			companies.setRecievership(j);
		else
			companies.clearRecievership(j);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company turn order
	{
		getChar = readValue(inDataStream, testArray);
		companies.setOrder(j, atoi(testArray));
	}
	findEOL(inDataStream);
	for (int m = 0; m < maxCities; m++) // Read cities
	{
		getChar = readValue(inDataStream, testArray);
		for (int j = 0; j < maxCompanies; j++)
		{
			getChar = readValue(inDataStream, testArray);
			if (atoi(testArray) == 1)
				companies.setCity(j, m);
			else
				companies.clearCity(j, m);
		}
		findEOL(inDataStream);
	}
	for (int m = 0; m < maxCoal; m++) // Read coal
	{
		getChar = readValue(inDataStream, testArray);
		for (int j = 0; j < maxCoal; j++)
		{
			getChar = readValue(inDataStream, testArray);
			if (atoi(testArray) == 1)
				companies.setCoal(j, m);
			else
				companies.clearCoal(j, m);
		}
		findEOL(inDataStream);
	}

	for (int k = 0; k < maxTrains; k++) // Read trains
	{
		getChar = readValue(inDataStream, testArray);
		for (int j = 0; j < maxCompanies; j++)
		{
			getChar = readValue(inDataStream, testArray);
			companies.setTrains(j, k, atoi(testArray));
		}
		findEOL(inDataStream);
	}
	getChar = readValue(inDataStream, testArray);
	getChar = readValue(inDataStream, testArray);
	game.setRound(atoi(testArray));
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	getChar = readValue(inDataStream, testArray);
	game.setTurn(atoi(testArray));
	companies.getTechLevel();
	return(0);
}
int pushFileData(wchar_t pfileName[])
{
	wchar_t infoStr[100];
	char *nstring = new char[42];
	size_t convertedChars = 0;
	ofstream outDataStream;
	outDataStream.open(pfileName);
	if (outDataStream.fail())
	{
		swprintf_s(infoStr, 100, L"Output file stream open failed \n");
		TextToMonitor(infoStr);
		return(1);
	}

	outDataStream << "Name,";
	for (int i = 0; i < numberPlayers; i++) // Save player name
	{
		wcstombs_s(&convertedChars, nstring, 42, players.name[i], _TRUNCATE);
		outDataStream << nstring << ",";
	}
	outDataStream << "Done#," << "\n";
	outDataStream << "Cash,";
	for (int i = 0; i < numberPlayers; i++) // Save player cash
	{
		outDataStream << players.getCash(i) << ",";
	}
	outDataStream << "\n";
	for (int j = 0; j < maxCompanies; j++) // Save player shares owned
	{
		wcstombs_s(&convertedChars, nstring, 42, companies.name[j], _TRUNCATE);
		outDataStream << nstring << ",";
		for (int i = 0; i < numberPlayers; i++)
		{
			outDataStream << players.getShares(i, j) << ",";
		}
		outDataStream << "\n";
	}
	for (int j = 0; j < maxCompanies; j++) // Save player sold company
	{
		wcstombs_s(&convertedChars, nstring, 42, companies.name[j], _TRUNCATE);
		outDataStream << nstring << ",";
		for (int i = 0; i < numberPlayers; i++)
		{
			outDataStream << players.getSold(i, j) << ",";
		}
		outDataStream << "\n";
	}	outDataStream << "Names,";
	for (int j = 0; j < maxCompanies; j++) // Save company names
	{
		wcstombs_s(&convertedChars, nstring, 42, companies.name[j], _TRUNCATE);
		outDataStream << nstring << ",";
	}
	outDataStream << "\n";
	outDataStream << "Cash, ";
	for (int j = 0; j < maxCompanies; j++) // Save company cash
	{
		outDataStream << companies.getCash(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "NetProfit,";
	for (int j = 0; j < maxCompanies; j++) // Save company netprofit
	{
		outDataStream << companies.getNetprofit(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "Valuation, ";
	for (int j = 0; j < maxCompanies; j++) //Save valuation
	{
		outDataStream << companies.getValuation(j) << ",";
	}
	outDataStream << "\n";

	outDataStream << "President, ";
	for (int j = 0; j < maxCompanies; j++) //Save president
	{
		outDataStream << companies.getPresident(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "Shares, ";
	for (int j = 0; j < maxCompanies; j++) // Save Company shares
	{
		outDataStream << companies.getShares(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "Orphans, ";
	for (int j = 0; j < maxCompanies; j++) // Sace Company orphans
	{
		outDataStream << companies.getOrphans(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "Started, ";
	for (int j = 0; j < maxCompanies; j++) // SAve company started
	{
		outDataStream << companies.getStarted(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "Reship, ";
	for (int j = 0; j < maxCompanies; j++)//Save company recievership
	{
		outDataStream << companies.getRecievership(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "TurnOrder, ";
	for (int j = 0; j < maxCompanies; j++) // Save company turnorder
	{
		outDataStream << companies.getOrder(j) << ",";
	}
	outDataStream << "\n";
	for (int m = 0; m < maxCities; m++) // Save cities
	{
		wcstombs_s(&convertedChars, nstring, 42, cities.name[m], _TRUNCATE);
		outDataStream << nstring << ",";
		for (int j = 0; j < maxCompanies; j++)
		{
			outDataStream << companies.getCity(j, m) << ",";
		}
		outDataStream << "\n";
	}
	for (int m = 0; m < maxCoal; m++) // Save coal
	{
		wcstombs_s(&convertedChars, nstring, 42, cities.coals[m], _TRUNCATE);
		outDataStream << nstring << ",";	
		for (int j = 0; j < maxCompanies; j++)
		{
			outDataStream << companies.getCoal(j, m) << ",";
		}
		outDataStream << "\n";
	}
	for (int k = 0; k < maxTrains; k++) // Save trains
	{
		outDataStream << k + 1 << ",";
		for (int j = 0; j < maxCompanies; j++)
		{
			outDataStream << companies.getTrains(j, k) << ",";
		}
		outDataStream << "\n";
	}
	outDataStream << ("Round,");
	outDataStream << (game.getRound()) << ", \n";
	outDataStream << ("Turn,");
	outDataStream << (game.getTurn()) << ", \n";
	for (int k = 0; k < maxTrains + 1; k++) // make sure rows are long enough
	{
		outDataStream << k << ",";
	}
	outDataStream << "\n";
	outDataStream.close();
	companies.getTechLevel();
	delete nstring;
	return(0);
}
int setCompanyPresident(int company)
{
	wchar_t infoStr[100];
	int oldpresident = companies.getPresident(company);
	int president = oldpresident;
	for (int i = 0; i < numberPlayers; i++)
	{
		if (players.getShares(i, company) > players.getShares(president, company))
		{
			companies.setPresident(company, i);
			president = i;
		}
	}
	if (president != oldpresident)
	{
		swprintf_s(infoStr, 100, L"++ %wS is the new president for %wS. \n", players.name[president], companies.name[company]);
		TextToMonitor(infoStr);
	}
	return(president);
}
int updateInfo()
{
	wchar_t cashStr[10];
	HFONT bigFont = CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |FF_SWISS, TEXT("Arial"));
	setPlayerNetworth();

	for (int i = 0; i < numberPlayers; i++)
	{
		int cPlayer = playerTurnorder[i];
		SetWindowText(GetDlgItem(infoDlg, IDC_PN1 + i), (LPCWSTR)players.name[cPlayer]);
		_itow_s(players.getCash(cPlayer), cashStr, 10);
		SetWindowText(GetDlgItem(infoDlg, IDC_PC1 + i), (LPCWSTR)cashStr);
		_itow_s(players.getNetworth(cPlayer), cashStr, 10);
		SetWindowText(GetDlgItem(infoDlg, IDC_PW1 + i), (LPCWSTR)cashStr);
		for (int j = 0; j < 10; j++)
		{
			_itow_s(players.getShares(cPlayer, j), cashStr, 10);
			ShowWindow(GetDlgItem(infoDlg, j * (maxPlayers+2) + IDC_PW7 + i), SW_SHOW);
			SendMessage(GetDlgItem(infoDlg, j * (maxPlayers + 2) + IDC_PW7 + i), WM_SETFONT, (WPARAM)bigFont, 1);
			SetWindowText(GetDlgItem(infoDlg, j * (maxPlayers + 2) + IDC_PW7 + i), (LPCWSTR)cashStr);
		}
	}
	for (int j = 0; j < 10; j++)
	{
		_itow_s(companies.getShares(j), cashStr, 10);
		ShowWindow(GetDlgItem(infoDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers), SW_SHOW);
		SendMessage(GetDlgItem(infoDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers), WM_SETFONT, (WPARAM)bigFont, 1);
		SetWindowText(GetDlgItem(infoDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers), (LPCWSTR)cashStr);
		_itow_s(companies.getOrphans(j), cashStr, 10);
		ShowWindow(GetDlgItem(infoDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers + 1), SW_SHOW);
		SendMessage(GetDlgItem(infoDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers + 1), WM_SETFONT, (WPARAM)bigFont, 1);
		SetWindowText(GetDlgItem(infoDlg, j * (maxPlayers + 2) + IDC_PW7 + maxPlayers + 1), (LPCWSTR)cashStr);

	}
	int tempTurnOrder[maxCompanies];
	companies.getTurnorders(tempTurnOrder);
	for (int j = 0; j < maxCompanies; j++)
	{
		int jc = tempTurnOrder[j];
		SendMessageW(GetDlgItem(infoDlg, IDC_BBO + j), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1 + jc)));
		ShowWindow(GetDlgItem(infoDlg, IDC_CC0 + j), SW_HIDE);
		ShowWindow(GetDlgItem(infoDlg, IDC_CV0 + j), SW_HIDE);
		ShowWindow(GetDlgItem(infoDlg, IDC_CC9 + j), SW_HIDE);
		if ((companies.getStarted(jc)) && (!companies.getRecievership(jc)))
		{
			_itow_s(companies.getCash(jc), cashStr, 10);
			ShowWindow(GetDlgItem(infoDlg, IDC_CC0 + j), SW_SHOW);
			SendMessage(GetDlgItem(infoDlg, IDC_CC0 + j), WM_SETFONT, (WPARAM)bigFont, 1);
			SetWindowText(GetDlgItem(infoDlg, IDC_CC0 + j), (LPCWSTR)cashStr);
			_itow_s(valuations[companies.getValuation(jc)], cashStr, 10);
			ShowWindow(GetDlgItem(infoDlg, IDC_CV0 + j), SW_SHOW);
			SendMessage(GetDlgItem(infoDlg, IDC_CV0 + j), WM_SETFONT, (WPARAM)bigFont, 1);
			SetWindowText(GetDlgItem(infoDlg, IDC_CV0 + j), (LPCWSTR)cashStr);
			_itow_s(companies.getNetprofit(jc), cashStr, 10);
			ShowWindow(GetDlgItem(infoDlg, IDC_CC9 + j), SW_SHOW);
			SendMessage(GetDlgItem(infoDlg, IDC_CC9 + j), WM_SETFONT, (WPARAM)bigFont, 1);
			SetWindowText(GetDlgItem(infoDlg, IDC_CC9 + j), (LPCWSTR)cashStr);
			wchar_t trainList[100];
			wcscpy_s(trainList, L"");
			for (int k = 0; k < companies.getTrainCount(jc); k++)
			{
				_itow_s(companies.getTrainOwned(jc, k) + 1, cashStr, 10);
				wcscat_s(trainList, cashStr);
				wcscat_s(trainList, L" ");
			}
			ShowWindow(GetDlgItem(infoDlg, IDC_CTB1 + j), SW_SHOW);
			SendMessage(GetDlgItem(infoDlg, IDC_CTB1 + j), WM_SETFONT, (WPARAM)bigFont, 1);
			SetWindowText(GetDlgItem(infoDlg, IDC_CTB1 + j), (LPCWSTR)trainList);
		}
	}

	return(0);
}
int updateBuild(int company)
{
	HWND hwndCC = GetDlgItem(buildDlg, IDC_CURRENT_COMPANY);
	HWND hwndST = GetDlgItem(buildDlg, IDC_SPIN_TRACK);
	HWND hwndSL = GetDlgItem(buildDlg, IDC_SPIN_TECH);
	HWND hwndNC = GetDlgItem(buildDlg, IDC_NETCASH);
	HWND hwndBR = GetDlgItem(buildDlg, IDC_BUILD_ROUND);
	wchar_t intStr[100];
	HFONT bigFont = CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |FF_SWISS, TEXT("Arial"));
	HFONT medFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |FF_SWISS, TEXT("Arial"));
	HFONT medFontB = CreateFont(14, 0, 0, 0, FW_EXTRABOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH |FF_SWISS, TEXT("Arial"));
	//		wchar_t cashStr[20];
	company = companyTurnorder[game.getTurn()];
	cBuild = companies;
	oldTrackCost = 0; //Reset current track cost for next company
	_itow_s(cBuild.getCash(company), intStr, 10);
	SetWindowText(hwndNC, intStr);
	SendMessage(hwndCC, WM_SETFONT, (WPARAM)bigFont, 1);
	SetWindowText(hwndCC, (LPCWSTR)cBuild.name[company]);
	_itow_s(game.getRound()-1, intStr, 10);
	SetWindowText(hwndBR, intStr);

	//Clean up train boxes
	for (int j = 0; j < 5; j++)
	{
		SendMessage(GetDlgItem(buildDlg, IDC_TS0 + j), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
		SendMessage(GetDlgItem(buildDlg, IDC_TB0 + j), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
		ShowWindow(GetDlgItem(buildDlg, IDC_TS0 + j), SW_HIDE);
		ShowWindow(GetDlgItem(buildDlg, IDC_TB0 + j), SW_HIDE);
		ShowWindow(GetDlgItem(buildDlg, IDC_TSN0 + j), SW_HIDE);
		ShowWindow(GetDlgItem(buildDlg, IDC_TBN0 + j), SW_HIDE);
	}

	// List trains to scrap
	for (int j = 0; j < cBuild.getTrainCount(company); j++)
	{
		ShowWindow(GetDlgItem(buildDlg, IDC_TS0 + j), SW_SHOW);
		ShowWindow(GetDlgItem(buildDlg, IDC_TSN0 + j), SW_SHOW);
		SetDlgItemInt(buildDlg, IDC_TSN0 + j, cBuild.getTrainOwned(company, j) + 1, true);
	}
	// List train for sale
	int train = cBuild.getTrainForSale();
	ShowWindow(GetDlgItem(buildDlg, IDC_TB0), SW_SHOW);
	ShowWindow(GetDlgItem(buildDlg, IDC_TBN0), SW_SHOW);
	SetDlgItemInt(buildDlg, IDC_TBN0, train + 1, true);

	// Intitialize Track Spin Boxes
	SendMessage(hwndST, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(40, 0));
	SendMessage(hwndST, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
	UINT val1 = LOWORD(SendMessage(hwndST, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
	SetDlgItemInt(buildDlg, IDC_TRACK_TO_BUY, val1 * 10, true);
	// Intitialize TechLevel Spin Boxes
	SendMessage(hwndSL, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(6, cBuild.getTechLevel()));
	SendMessage(hwndSL, UDM_SETPOS, (WPARAM)0, (LPARAM)cBuild.getTechLevel());
	UINT val2 = LOWORD(SendMessage(hwndSL, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
	SetDlgItemInt(buildDlg, IDC_TECH, val2, true);
	// Intialize Dividen Radio Buttons
	SendMessage(GetDlgItem(buildDlg, IDC_DIST), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
	SendMessage(GetDlgItem(buildDlg, IDC_WITH), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
	//	Initialize Cities
	for (int k = 0; k < maxCities; k++)
	{
		HWND hXC = GetDlgItem(buildDlg, IDC_XS0 + k);
		if (cBuild.getCity(company, k))
		{
			SendMessage(hXC, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
			SendMessage(hXC, WM_SETFONT, (WPARAM)medFontB, TRUE);
		}
		else
		{
			SendMessage(hXC, BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
			SendMessage(hXC, WM_SETFONT, (WPARAM)medFont, TRUE);
		}
	}
	//	Initialize Coal
	for (int k = 0; k < maxCoal; k++)
	{
		HWND hXC = GetDlgItem(buildDlg, IDC_XC0 + k);
		if (cBuild.getCoal(company, k))
		{
			SendMessage(hXC, BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
			SendMessage(hXC, WM_SETFONT, (WPARAM)medFontB, TRUE);

		}
		else
		{
			SendMessage(hXC, BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
			SendMessage(hXC, WM_SETFONT, (WPARAM)medFont, TRUE);
		}
	}
	//		Initialize Cash
	_itow_s(cBuild.getCash(company), intStr, 10);
	SetWindowText(hwndNC, intStr);
	//		Intialize Net Profit
	updateProfits(company);
	return(0);
}
int updateProfits(int company)
{
	wchar_t intStr[100];
	SendMessage(GetDlgItem(buildDlg, IDC_NPVS), (UINT)CB_RESETCONTENT, (WPARAM)0, (LPARAM)intStr);
	int numProfits = cBuild.calcNP(company, profitList);
	SendMessage(GetDlgItem(buildDlg, IDC_NPVS), CB_SETCURSEL, (WPARAM)cBuild.getCityCount(company), (LPARAM)0);
	for (int k = 0; k < numProfits; k++)
	{
		_itow_s(profitList[k], intStr, 10);
		SendMessage(GetDlgItem(buildDlg, IDC_NPVS), (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)intStr);
		if (profitList[k] == cBuild.getNetprofit(company))
		{
			SendMessage(GetDlgItem(buildDlg, IDC_NPVS), CB_SETCURSEL, (WPARAM)k, (LPARAM)0);
		}
	}
	LRESULT resutl = SendMessage(GetDlgItem(buildDlg, IDC_NPVS), CB_SETTOPINDEX, (WPARAM)numProfits-1, (LPARAM)0);
	return (0);
}
int incrementTurn(bool success)
{
	int round = game.getRound();
	int turn = game.getTurn();
	pushAnalytics();
	switch (round)
	{
	case 1:
	{
		if (!success)
		{
			game.incNumberPasses();
		}
		if (game.getNumberPasses() != numberPlayers)
		{
			if (turn == numberPlayers-1)
			{
				game.setNumberPasses(0);
				turn = 0;
			}
			else
			{
				turn++;
			}
			SetWindowText(GetDlgItem(marketDlg, IDC_CURRENT_PLAYER), (LPCWSTR)players.name[game.getTurn()]);
			game.setMarketSuccess(FALSE);
			game.setBuySuccess(FALSE);
			game.setTurn(turn);
			game.setRound(round);
			pushFileData(wfileName);
			saveState();
			return(0);
		}
		else
		{
			game.setNumberPasses(0);
			game.setMarketSuccess(FALSE);
			turn = 0;
			round++;
			// Clear sold array and 
			for (int i = 0; i < numberPlayers; i++)
			{
				for (int j = 0; j < maxCompanies; j++)
				{
					players.clearSold(i, j);
				}
			}
			reduceForOrphans();
			companies.getTurnorders(companyTurnorder);
			updateInfo();
			game.setTurn(turn);
			game.setRound(round);
			pushFileData(wfileName);
			for (int j = 0; j < maxCompanies; j++) // Clear ProfitPerShare
				companies.setProfitPerShare(j, 0);
			saveState();
			return(1);
		}
	}
	case 2:
	{
		if ((turn < maxCompanies-1) && (companies.getStarted(companyTurnorder[turn + 1])) && (!companies.getRecievership(companyTurnorder[turn + 1])))
		{
			turn++;
			game.setTurn(turn);
			game.setRound(round);
			pushFileData(wfileName);
			saveState();
			return(0);
		}
		else
		{
			turn = 0;
			round++;
			companies.getTurnorders(companyTurnorder);
			game.setTurn(turn);
			game.setRound(round);
			pushFileData(wfileName);
			for (int j = 0; j < maxCompanies; j++) // Clear ProfitPerShare
				companies.setProfitPerShare(j, 0);
			saveState();
			return(1);
		}
	}
	case 3:
	{
		if ((turn < maxCompanies - 1) && (companies.getStarted(companyTurnorder[turn + 1])) && (!companies.getRecievership(companyTurnorder[turn + 1])))
		{
			turn++;
			game.setTurn(turn);
			game.setRound(round);
			pushFileData(wfileName);
			saveState();
			return(0);
		}
		else
		{
			if (companies.getTechLevel() < 6)
			{
				turn = 0;
				round = 1;
				players.getTurnorders(playerTurnorder, numberPlayers);
				game.setTurn(turn);
				game.setRound(round);
				pushFileData(wfileName);
				for (int j = 0; j < maxCompanies; j++) // Clear ProfitPerShare
					companies.setProfitPerShare(j, 0);
			}
			else
			{
				endGame();
				round = 0;
			}
			saveState();
			return(1);
		}
	}
	}
	saveState();
	return(1);
}
int checkOrphans(int player)
{
	for (int j = 0; j < maxCompanies; j++)
	{
		if ((player == companies.getPresident(j)) && (companies.getOrphans(j) != 0))
		{
			return(companies.getOrphans(j));
		}
	}
	return(0);
}
int endGame()
{
	wchar_t infoStr[100];
	swprintf_s(infoStr, 100, L"End of the game.");
	TextToMonitor(infoStr);
	int winner = 0;
	for (int i = 0; i < numberPlayers; i++)
	{
		if (players.getNetworth(i) > players.getNetworth(winner))
		{
			winner = i;
		}
	}
	swprintf_s(infoStr, 100, L" The winner is %wS! \n", players.name[winner]);
	TextToMonitor(infoStr);
	MessageBox(infoDlg, infoStr, L"END OF GAME", MB_OK);
	return(0);
}
int cityModInit(int company, HWND mXDlg)
{
	wchar_t cashStr[20];
	int test;
	SetWindowText(GetDlgItem(mXDlg, IDC_XM_NAME), (LPCWSTR)companies.name[company]);
	for (int m = 0; m < maxCities; m++)
	{
		test = IDC_MX410 + m;
		 SetWindowText(GetDlgItem(mXDlg, IDC_MX410 + m), (LPCWSTR)cities.name[m]);
		_itow_s(companies.getCity(company, m), cashStr, 10);
		test = IDC_MXR410 + m;
		SetWindowText(GetDlgItem(mXDlg, IDC_MXR450 + m), (LPCWSTR)cashStr);
	}
	for (int m = 0; m < maxCoal; m++)
	{
		SetWindowText(GetDlgItem(mXDlg, IDC_MX445 + m), (LPCWSTR)cities.coals[m]);
		for (int j = 0; j < maxCompanies; j++)
		{
			_itow_s(companies.getCoal(company, m), cashStr, 10);
			SetWindowText(GetDlgItem(mXDlg, IDC_MXR485 + m), (LPCWSTR)cashStr);
		}
	}
	return(0);
}
int cityModOK(int company, HWND mXDlg)
{
	wchar_t cashStr[50];
	int textLength;
	int bValue;
	for (int m = 0; m < maxCities; m++)
	{
		textLength = GetWindowTextLength(GetDlgItem(mXDlg, IDC_MXR450 + m)) + 1;
		textLength = GetWindowText(GetDlgItem(mXDlg, IDC_MXR450 + m), cashStr, textLength);
		bValue = _wtoi(cashStr);
		if (bValue != 0)
		{
			companies.setCity(company, m);
			cBuild.setCity(company, m);
		}
		else
		{
			companies.clearCity(company, m);
			cBuild.clearCity(company, m);

		}
	}
	for (int m = 0; m < maxCoal; m++)
	{
		for (int j = 0; j < maxCompanies; j++)
		{
			textLength = GetWindowTextLength(GetDlgItem(mXDlg, IDC_MXR485 + m)) + 1;
			textLength = GetWindowText(GetDlgItem(mXDlg, IDC_MXR485 + m), cashStr, textLength);
			bValue = _wtoi(cashStr);
			if (bValue)
			{
				companies.setCoal(company, m);
				cBuild.setCoal(company, m);
			}
			else
			{
				companies.clearCoal(company, m);
				cBuild.clearCoal(company, m);

			}
		}
	}
	return(0);
}
int trainModInit(int company, HWND mXDlg)
{
	wchar_t cashStr[20];
	int test;
	SetWindowText(GetDlgItem(mXDlg, IDC_XM_NAME), (LPCWSTR)companies.name[company]);
	for (int m = 0; m < maxTrains; m++)
	{
		_itow_s(m+1, cashStr, 10);
		SetWindowText(GetDlgItem(mXDlg, IDC_MX410 + m), (LPCWSTR)cashStr);
		_itow_s(companies.getTrain(company, m), cashStr, 10);
		test = IDC_MXR410 + m;
		SetWindowText(GetDlgItem(mXDlg, IDC_MXR450 + m), (LPCWSTR)cashStr);
	}
	return(0);
}
int trainModOK(int company, HWND mXDlg)
{
	wchar_t cashStr[50];
	int textLength;
	for (int m = 0; m < maxTrains; m++)
	{
		textLength = GetWindowTextLength(GetDlgItem(mXDlg, IDC_MXR450 + m)) + 1;
		textLength = GetWindowText(GetDlgItem(mXDlg, IDC_MXR450 + m), cashStr, textLength);
		companies.setTrains(company, m, _wtoi(cashStr));
		cBuild.setTrains(company, m, _wtoi(cashStr));
	}
	return(0);
}
int saveState()
{
	for (int i = 8; i >= 0; i--)
	{
		playersOld[i + 1] = playersOld[i];
		companiesOld[i + 1] = companiesOld[i];
		gameOld[i + 1] = gameOld[i];
		for (int j = 0; j < maxPlayers; j++)
		{
			playerTurnorderOld[i + 1][j] = playerTurnorderOld[i][j];
		}
		for (int j = 0; j < maxCompanies; j++)
		{
			companyTurnorderOld[i + 1][j] = companyTurnorderOld[i][j];
		}
	}
	playersOld[0] = players;
	companiesOld[0] = companies;
	gameOld[0] = game;
	for (int j = 0; j < maxPlayers; j++)
	{
		playerTurnorderOld[0][j] = playerTurnorder[j];
	}
	for (int j = 0; j < maxCompanies; j++)
	{
		companyTurnorderOld[0][j] = companyTurnorder[j];
	}

	return(0);
}
int openAnalytics()
{
	wchar_t infoStr[100];
    char *nstring = new char[42];
	size_t convertedChars = 0;
	ofstream analyDataStream;
	analyDataStream.open(L"BOAnalytics.csv");
	if (analyDataStream.fail())
	{
		swprintf_s(infoStr, 100, L"Analytics file stream open failed \n");
		TextToMonitor(infoStr);
		return(1);
	}
	analyDataStream  << "Round,";
	analyDataStream << "Turn,";
	analyDataStream << "Entity,";
	for (int i = 0; i < numberPlayers; i++) // Save player cash
	{
		wcstombs_s(&convertedChars, nstring, 42, players.name[i], _TRUNCATE);
		analyDataStream << nstring << " cash,";
	}
	for (int i = 0; i < numberPlayers; i++) // Save player net worth
	{
		wcstombs_s(&convertedChars, nstring, 42, players.name[i], _TRUNCATE);
		analyDataStream << nstring << " net,";
	}
	for (int j = 0; j < maxCompanies; j++) // Save company cash
	{
		wcstombs_s(&convertedChars, nstring, 42, companies.name[j], _TRUNCATE);
		analyDataStream << nstring << " cash,";
	}
	for (int j = 0; j < maxCompanies; j++) // Save company netprofit
	{
		wcstombs_s(&convertedChars, nstring, 42, companies.name[j], _TRUNCATE);
		analyDataStream << nstring << " np,";
	}
	for (int j = 0; j < maxCompanies; j++) //Save valuation
	{
		wcstombs_s(&convertedChars, nstring, 42, companies.name[j], _TRUNCATE);
		analyDataStream << nstring << " val,";
	}
	for (int j = 0; j < maxCompanies; j++) //Save profit per share
	{
		wcstombs_s(&convertedChars, nstring, 42, companies.name[j], _TRUNCATE);
		analyDataStream << nstring << " pps,";
	}


	for (int j = 0; j < maxCompanies; j++) // Save Company shares
	{
		wcstombs_s(&convertedChars, nstring, 42, companies.name[j], _TRUNCATE);
		analyDataStream << nstring << " sh,";
	}
	for (int j = 0; j < maxCompanies; j++) // Sace Company orphans
	{
		wcstombs_s(&convertedChars, nstring, 42, companies.name[j], _TRUNCATE);
		analyDataStream << nstring << " or,";
	}
	analyDataStream << "Train \n";;
	analyDataStream.close();
	delete nstring;
	return(0);
}
int pushAnalytics()
{
	wchar_t infoStr[100];
	char *nstring = new char[42];
	size_t convertedChars = 0;
	ofstream analyDataStream;
	analyDataStream.open(L"BOAnalytics.csv", std::ofstream::app);
	if (analyDataStream.fail())
	{
		swprintf_s(infoStr, 100, L"Analytics file stream open failed \n");
		TextToMonitor(infoStr);
		return(1);
	}
	analyDataStream << (game.getRound()) << ",";
	analyDataStream << (game.getTurn()) << ",";
	if (game.getRound() == 1)
	{
		wcstombs_s(&convertedChars, nstring, 42, players.name[playerTurnorder[game.getTurn()]], _TRUNCATE);
		analyDataStream << nstring << ",";
	}
	else
	{
		wcstombs_s(&convertedChars, nstring, 42, companies.name[companyTurnorder[game.getTurn()]], _TRUNCATE);
		analyDataStream << nstring << ",";
	}
	for (int i = 0; i < numberPlayers; i++) // Save player cash
	{
		analyDataStream << players.getCash(i) << ",";
	}
	for (int i = 0; i < numberPlayers; i++) // Save player cash
	{
		analyDataStream << players.getNetworth(i) << ",";
	}
	for (int j = 0; j < maxCompanies; j++) // Save company cash
	{
		analyDataStream << companies.getCash(j) << ",";
	}
	for (int j = 0; j < maxCompanies; j++) // Save company netprofit
	{
		analyDataStream << companies.getNetprofit(j) << ",";
	}
	for (int j = 0; j < maxCompanies; j++) //Save valuation
	{
		analyDataStream << valuations[companies.getValuation(j)] << ",";
	}
	for (int j = 0; j < maxCompanies; j++) //Save profit per share
	{
		analyDataStream << companies.getProfitPerShare(j) << ",";
	}
	for (int j = 0; j < maxCompanies; j++) // Save Company shares
	{
		analyDataStream << companies.getShares(j) << ",";
	}
	for (int j = 0; j < maxCompanies; j++) // Sace Company orphans
	{
		analyDataStream << companies.getOrphans(j) << ",";
	}
	analyDataStream << companies.getTrainForSale()+1;
	analyDataStream << "\n";
	analyDataStream.close();
	delete nstring;
	return(0);
}
int openMonitor()
{
	wchar_t infoStr[100];
    char *nstring = new char[42];
	size_t convertedChars = 0;
	ofstream monitorDataStream;
	monitorDataStream.open(L"BOMonitor.txt");
	if (monitorDataStream.fail())
	{
		swprintf_s(infoStr, 100, L"Monitor file stream open failed \n");
		return(1);
	}
	monitorDataStream  << "Monitor File \n \n";
	
	monitorDataStream.close();
	delete nstring;
	return(0);
}
int pushMonitor(wchar_t theText[])
{
	wchar_t infoStr[100];
	char *nstring = new char[300];
	size_t convertedChars = 0;
	ofstream monitorDataStream;
	monitorDataStream.open(L"BOmonitor.txt", std::ofstream::app);
	if (monitorDataStream.fail())
	{
		swprintf_s(infoStr, 100, L"Monitor file stream open failed \n");
		TextToMonitor(infoStr);
		return(1);
	}
	wcstombs_s(&convertedChars, nstring, 300, theText, _TRUNCATE);
	monitorDataStream << nstring;
	monitorDataStream.close();
	delete nstring;
	return(0);
}


