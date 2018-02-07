// BOBanker.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BOBanker.h"
#include <iostream>

#define MAX_LOADSTRING 100

#define BUTTON_IDENTIFIER 1



// Global Variables:
HINSTANCE hInst;                                // current instance
HINSTANCE ghInstance = NULL;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int numberPlayers = 3;
wchar_t  playerName[6][20];
int Value = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Players(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PlayerInfo(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Market(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    BuyShares(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SellShares(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Orphans(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    InitValuation(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Scrap(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Train(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Track(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Profit(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	
	// TODO: Place code here.


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



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
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

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	TCHAR greeting[] = _T("Hello World!");
    switch (message)
    {
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_START:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_PLAYERBOX), hWnd, Players);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_PLAYERINFOBOX), hWnd, PlayerInfo);			
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MARKETBOX), hWnd, Market);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
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

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Message handler for player box.
INT_PTR CALLBACK Players(HWND pDlg, UINT message, WPARAM wParam, LPARAM lParam)
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


			// Read Player Names
			textLength = GetWindowTextLength(GetDlgItem(pDlg, IDC_EDITP1))+1;
			textLength = GetWindowText(GetDlgItem(pDlg, IDC_EDITP1), playerName[0], textLength);
			textLength = GetWindowTextLength(GetDlgItem(pDlg, IDC_EDITP2))+1;
			textLength = GetWindowText(GetDlgItem(pDlg, IDC_EDITP2), playerName[1], textLength);
			textLength = GetWindowTextLength(GetDlgItem(pDlg, IDC_EDITP3))+1;
			textLength = GetWindowText(GetDlgItem(pDlg, IDC_EDITP3), playerName[2], textLength);
			textLength = GetWindowTextLength(GetDlgItem(pDlg, IDC_EDITP4))+1;
			textLength = GetWindowText(GetDlgItem(pDlg, IDC_EDITP4), playerName[3], textLength);
			textLength = GetWindowTextLength(GetDlgItem(pDlg, IDC_EDITP5))+1;
			textLength = GetWindowText(GetDlgItem(pDlg, IDC_EDITP5), playerName[4], textLength);
			textLength = GetWindowTextLength(GetDlgItem(pDlg, IDC_EDITP6))+1;
			textLength = GetWindowText(GetDlgItem(pDlg, IDC_EDITP6), playerName[5], textLength);



			EndDialog(pDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_CHECKP4:
			EnableWindow(GetDlgItem(pDlg, IDC_EDITP4), TRUE);
			EnableWindow(GetDlgItem(pDlg, IDC_CHECKP5), TRUE);
			numberPlayers = 4;
			break;
		case IDC_CHECKP5:
			EnableWindow(GetDlgItem(pDlg, IDC_EDITP5), TRUE);
			EnableWindow(GetDlgItem(pDlg, IDC_CHECKP6), TRUE);
			numberPlayers = 5;
			break;
		case IDC_CHECKP6:
			EnableWindow(GetDlgItem(pDlg, IDC_EDITP6), TRUE);
			break;
		}
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK PlayerInfo(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Add items to list. 
		HWND hwndList = GetDlgItem(hDlg, IDC_LISTPLAYER);
		for (int i = 0; i < numberPlayers; i++)
		{
			int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0,(LPARAM)playerName[i]); //Write line to list box
			SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i); // Associate index with each line
		}
		// Set input focus to the list box.
		SetFocus(hwndList);
		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		case IDC_LISTPLAYER:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				HWND hwndList = GetDlgItem(hDlg, IDC_LISTPLAYER);

				// Get selected index.
				int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

				// Get item data.
				int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);

				// Do something with the data from Roster[i]
	//			TCHAR buff[MAX_PATH];
//				StringCbPrintf(buff, ARRAYSIZE(buff),
//					TEXT("Position: %s\nGames played: %d\nGoals: %d"),
//					playerName[i]);

//				SetDlgItemText(hDlg, IDD_PLAYERINFOBOX, buff);
				return TRUE;
			}
			}
		}
		return TRUE;
		}
	}
	return FALSE;
}

INT_PTR CALLBACK Market(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HWND hwndList = GetDlgItem(hDlg, IDC_LIST_CURRENT_PLAYER);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Add items to list. 
		int i = 0;
		int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0,
			(LPARAM)playerName[i]);
		// Set the array index of the player as item data.
		// This enables us to retrieve the item from the array
		// even after the items are sorted by the list box.
		SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i);
		// Set input focus to the list box.
		SetFocus(hwndList);
		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_BUY:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_BUYBOX), hDlg, BuyShares);
			break;
		case IDC_BUTTON_SELL:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SELLBOX), hDlg, SellShares);
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_SCRAPBOX), hDlg, Scrap);
			break;
		case IDC_BUTTON_ORPHANS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_TRAINBOX), hDlg, Train);
//			DialogBox(hInst, MAKEINTRESOURCE(IDD_ORPHANBOX), hDlg, Orphans);
			break;
		case IDC_BUTTON_PASS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_TRACKBOX), hDlg, Track);
//			MessageBox(hDlg, L"Do you want to pass?", L"Confirm", MB_OKCANCEL);
			break;
		case IDC_CANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK BuyShares(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(hDlg, IDC_LIST_CURRENT_PLAYER);
	HWND hwndList2 = GetDlgItem(hDlg, IDC_STOCKS_TO_BUY);


	switch (message)
	{
	case WM_INITDIALOG:
	{
		// List Player Information. 
		int i = 0;
		int pos = (int)SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)playerName[i]);
		// List Stocks to Buy. 
		char STB[20] = "Stocks to buy";
		pos = (int)SendMessage(hwndList2, LB_ADDSTRING, 0, (LPARAM)STB); // Write line to list box
		SendMessage(hwndList2, LB_SETITEMDATA, pos, (LPARAM)i); // Assocaite index with each line
		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUY:
			Value = GetDlgItemInt(hDlg, IDC_SHARES_TO_BUY, NULL, false);
			Value = Value++;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_INITVALBOX), hDlg, InitValuation);
			break;
		case IDC_CANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}


INT_PTR CALLBACK SellShares(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(hDlg, IDC_LIST_CURRENT_PLAYER);
	HWND hwndList2 = GetDlgItem(hDlg, IDC_STOCKS_TO_SELL);
	HWND hwndList3 = GetDlgItem(hDlg, IDC_SHARES_SLIDER);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// List Player Information. 
		int i = 0;
		int pos = (int)SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)playerName[i]);
		// List Stocks to Sell. 
		char STB[20] = "Stocks to sell";
		pos = (int)SendMessage(hwndList2, LB_ADDSTRING, 0, (LPARAM)STB); // Write line to list box
		SendMessage(hwndList2, LB_SETITEMDATA, pos, (LPARAM)i); // Assocaite index with each line
//		SendMessage(hwndList3, SBM_SETRANGE, pos, MAKELPARAM(0, 5)); // Assocaite index with each line		
//		SendMessage(hwndList3, TBM_SETRANGE, pos, MAKELPARAM(0,5)); // Assocaite index with each line


		SetFocus(hwndList3);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SELL:
			Value = GetDlgItemInt(hDlg, IDC_SHARES_TO_SELL, NULL, false);
			Value = Value++;
			break;
		case IDC_CANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}


INT_PTR CALLBACK Orphans(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(hDlg, IDC_LIST_CURRENT_PLAYER);
	HWND hwndList2 = GetDlgItem(hDlg, IDC_ORPHAN_STOCKS);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// List Player Information. 
		int i = 0;
		int pos = (int)SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)playerName[i]);
		// List Stocks to Buy. 
		char STB[30] = "Orphan stocks to buy";
		pos = (int)SendMessage(hwndList2, LB_ADDSTRING, 0, (LPARAM)STB); // Write line to list box
		SendMessage(hwndList2, LB_SETITEMDATA, pos, (LPARAM)i); // Assocaite index with each line
		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUY:
			Value = GetDlgItemInt(hDlg, IDC_SHARES_TO_BUY, NULL, false);
			Value = Value++;
			break;
		case IDC_CANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK InitValuation(HWND hDlg, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{

		HWND hwndList1 = GetDlgItem(hDlg, IDC_VAL1);
		HWND hwndList2 = GetDlgItem(hDlg, IDC_VAL2);
		HWND hwndList3 = GetDlgItem(hDlg, IDC_VAL3);
		CheckRadioButton(hDlg, IDC_RADIOV1, IDC_RADIOV3, IDC_RADIOV2);
		SetWindowText(hwndList1, L"$55");
		SetWindowText(hwndList2, L"$60");
		SetWindowText(hwndList3, L"$66");
		return TRUE;
	}

	
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			if (IsDlgButtonChecked(hDlg, IDC_RADIOV1))
			{
				Value = 0;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_RADIOV2))
			{
				Value = 1;
			}
			else if (IsDlgButtonChecked(hDlg, IDC_RADIOV3))
			{
				Value = 2;
			}
			else
				MessageBox(hDlg, L"Must select one option.", L"ERROR", MB_OKCANCEL);
			return TRUE;
			break;
		}
		return TRUE; 
	}
	return FALSE;
}

INT_PTR CALLBACK Scrap(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(hDlg, IDC_LIST_CURRENT_COMPANY);
	HWND hwndList2 = GetDlgItem(hDlg, IDC_LIST_TRAINS_OWNED);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// List Company Information. 

		int i = 0;
		int pos = (int)SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)playerName[i]);
		// List Trains Owned. 
		char STB[20] = "T";
		pos = (int)SendMessage(hwndList2, LB_ADDSTRING, 0, (LPARAM)playerName[i]); // Write line to list box
		SendMessage(hwndList2, LB_SETITEMDATA, pos, (LPARAM)i); // Assocaite index with each line
		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SCRAP:
			Value = GetDlgItemInt(hDlg, IDC_LIST_TRAINS_OWNED, NULL, false);
			Value = Value++;
			break;
		case IDC_CANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK Train(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(hDlg, IDC_LIST_CURRENT_COMPANY);
	HWND hwndList2 = GetDlgItem(hDlg, IDC_LIST_TRAIN_FORSALE);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// List Company Information. 

		int i = 0;
		int pos = (int)SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)playerName[i]);
		// List Trains Owned. 
		char STB[20] = "T";
		pos = (int)SendMessage(hwndList2, LB_ADDSTRING, 0, (LPARAM)playerName[i]); // Write line to list box
		SendMessage(hwndList2, LB_SETITEMDATA, pos, (LPARAM)i); // Assocaite index with each line
		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUY:
			Value = GetDlgItemInt(hDlg, IDC_TRACK_TO_BUY, NULL, false);
			Value = Value++;
			break;
		case IDC_CANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}

INT_PTR CALLBACK Track(HWND hDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(hDlg, IDC_LIST_CURRENT_COMPANY);
	HWND hwndList2 = GetDlgItem(hDlg, IDC_TRACK_TO_BUY);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// List Company Information. 

		int i = 0;
		int pos = (int)SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)playerName[i]);
		// List Trains Owned. 
		char STB[20] = "T";
		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUY:
			Value = GetDlgItemInt(hDlg, IDC_TRACK_TO_BUY, NULL, false);
			Value = Value++;
			break;
		case IDC_CANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}

// Profit window proc will go here.