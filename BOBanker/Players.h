#pragma once


class Players
{
public:
	Players(int value);

	wchar_t name[maxPlayers][20];
	void getInfo(int player);
	int getCash(int player);
	int getNetworth(int player);
	int getOrder(int player);
	int getShares(int player, int company);
	int getSold(int player, int company);

	int setCash(int player, int amount);
	int setNetworth(int player, int amount);
	int setOrder(int player, int order);
	int setShares(int player, int company, int amount);
	int totalSharesOwned(int company);

	int setSold(int player, int company);
	int clearSold(int player, int company);

	int changeCash(int player, int amount);
	int changeShares(int player, int company, int amount);
	int getTurnorders(int playerTurnorder[], int numberPlayers);
	// Preconditions: Current turn order is passed to function with number of player.
	// Postcondition: The function will use the current player cash to re-evaluated turn order so that 
	//				  the index of the lowest cash in playerTurnorder[0] and highest cash is in playerTurnorder[numberPlayers].
	//				Returns 0 if successful.
	int setTurnorders();
	// Preconditions: Call function when cash changed.
	// Postcondition: Function will revevaluate the turn order so that the lowest cash in hase order =0 and 
	//				the highest cash player has order = numberPlayers. Should be called when cash changes.
	//				Returns 0 if successful.




private:


	int cash[maxPlayers] = { 120,20,55,35,204,10 };
int networth[maxPlayers] = { 1000, 300, 555, 234, 1090, 49 };
int order[maxPlayers];
int shares[maxPlayers][maxCompanies];
int sold[maxPlayers][maxCompanies];

};

