// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <fstream>
#include <CommCtrl.h>
#include <malloc.h>

#include <tchar.h>
#include <stdio.h>
#include <iostream>
#include <time.h>   
#include "targetver.h"
#include <stdlib.h>
#include <windef.h>
#include <sstream>
#include <WinUser.h>


//#include <cstring>  
//#include <string>   
//#include <memory.h>
using namespace std;

const int maxPlayers = 6;
const int maxCompanies = 10;
const int maxTrains = 30;
const int maxCities = 35;
const int maxCoal = 5;
const int initialCash[6] = { 1500, 750, 500, 375, 300, 250 };
const int valuations[26] = { 34, 37, 41, 45, 50, 55, 60, 66, 74, 82, 91, 100, 110, 121, 133, 146, 160, 176, 194, 213, 234, 257, 282, 310, 341, 375 };
const int initialValuations[6][3] = { { 5, 6, 7 },
{ 6, 7, 8 },
{ 7, 8, 9 },
{ 8, 9, 10 },
{ 9, 10, 11 } };
const int totalCash = 1500;


// TODO: reference additional headers your program requires here
