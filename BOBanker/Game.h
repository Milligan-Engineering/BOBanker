#pragma once
class Game
{
public:
	Game(int pTurn, int pRound);

	int setTurn(int turn);
	int getTurn();
	int setRound(int round);
	int getRound();
	int getPasses();
	int setPasses(int value);


private:
	int turn; // Index of current Player or company
	int round; // Current round
	bool marketSuccess; //Indicates player did not pass
	int numberPasses; //Number of passes in turn
	int playerOrder[maxPlayers];
	int companyOrder[maxCompanies];

};

