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

	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 }
	};

	const char name[maxCities][20] = { "Albany", "Augusta", "Baltimore", "Boston", "Buffalo", "Burlington", "Cairo", "Chicago", "Cincinnati", "Cleveland",
		"Concord", "Detroit", "Dover", "FortWayne", "Harrisburg", "Hartford", "Hunington", "Indainapolis", "Lexington", "Lousiville", "NewHaven", "NewYork",
		"Norfolk", "Philadelphia", "Pittsburg", "Portsmouth", "Provience", "Richmond", "Roanoke", "SaintLouis", "Springfield", "Syracuse", "Utica", "Washington", "Wheeling", "Coal1", "Coal2", "Coal3", "Coal4", "Coal5" };

};
