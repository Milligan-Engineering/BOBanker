#pragma once
class Cities
{
public:
	Cities(int setup);

	const int cprofit[maxCities][6] = {

		{ 30, 30, 40, 40, 40, 50 },
	{ 20, 20, 20, 20, 30, 40 },
	{ 20, 30, 30, 40, 40, 50 },
	{ 30, 30, 40, 40, 50, 50 },
	{ 20, 30, 30, 40, 50, 60 },

	{ 10, 20, 20, 20, 30, 30 },
	{ 10, 20, 20, 20, 20, 20 },
	{ 20, 30, 50, 70, 90, 100 },
	{ 30, 40, 50, 50, 60, 70 },
	{ 20, 30, 40, 50, 60, 60 },

	{ 20, 20, 20, 20, 20, 30 },
	{ 20, 30, 40, 60, 80, 90 },
	{ 10, 10, 10, 20, 20, 20 },
	{ 10, 20, 20, 30, 40, 50 },
	{ 10, 10, 20, 20, 20, 20 },

	{ 20, 20, 20, 30, 30, 30 },
	{ 10, 10, 20, 30, 30, 40 },
	{ 20, 30, 30, 40, 50, 60 },
	{ 10, 20, 20, 30, 30, 30 },
	{ 20, 30, 30, 40, 40, 50 },

	{ 20, 20, 30, 30, 30, 40 },
	{ 30, 40, 50, 60, 70, 80 },
	{ 20, 20, 30, 30, 30, 40 },
	{ 30, 40, 40, 40, 50, 60 },
	{ 20, 30, 40, 60, 70, 80 },

	{ 20, 20, 20, 20, 20, 30 },
	{ 20, 30, 30, 30, 30, 30 },
	{ 30, 30, 20, 20, 20, 30 },
	{ 20, 20, 20, 20, 20, 20 },
	{ 30, 40, 50, 60, 70, 90 },

	{ 10, 10, 20, 20, 20, 30 },
	{ 10, 20, 20, 30, 30, 40 },
	{ 10, 10, 10, 20, 20, 20 },
	{ 20, 20, 30, 30, 30, 30 },
	{ 20, 20, 30, 40, 50, 60 },
	};

	const wchar_t name[maxCities][20] = { L"Albany", L"Augusta", L"Baltimore", L"Boston", L"Buffalo", L"Burlington", L"Cairo", L"Chicago", L"Cincinnati", L"Cleveland",
		L"Concord", L"Detroit", L"Dover", L"FortWayne", L"Harrisburg", L"Hartford", L"Hunington", L"Indainapolis", L"Lexington", L"Lousiville", L"NewHaven", L"NewYork",
		L"Norfolk", L"Philadelphia", L"Pittsburgh", L"Portsmouth", L"Provience", L"Richmond", L"Roanoke", L"SaintLouis", L"Springfield", L"Syracuse", L"Utica", L"Washington", L"Wheeling" };

	const wchar_t coals[maxCoal][20] = { L"Coal1", L"Coal2", L"Coal3", L"Coal4", L"Coal5" };	
	const wchar_t lumbers[maxLumber][20] = { L"Lumber" };

	const int coalValue[maxCoal] = { 40,40,40,40,40 };
	const int lumberValue[maxLumber] = { 10 };

};