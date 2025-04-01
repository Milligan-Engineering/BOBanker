#pragma once
class Game
{
public:
//	Game(int pTurn, int pRound);

	Game();

	int setTurn(int turn);
	//Precondition: The index of the turn order is passed in turn
	//Postcondition: Value is stored in turn.

	int getTurn();
	//Precondition: None
	//Postcondition: Current value of turn is returned.

	int getStep();
	//Precondition: None
	//Postcondition: Current value of step is returned.

	int incStep();
	//Precondition: None
	//Postcondition: stepStore in incresed by 1, stored in step and the value is returned.

	int setStep(int pStep);
	//Precondition: The step is passed in pStep
	//Postcondition: Value is stored in step.

	int setRound(int round);
	//Precondition: The index of the round is passed in turn
	//Postcondition: Value is stored in turn.

	int getRound();
	//Precondition: None
	//Postcondition: Current value of turn is returned.

	int getStage();
	//Precondition: None
	//Postcondition: Current value of stage is returned.

	int incStage();
	//Precondition: None
	//Postcondition: stage in incresed by 1 and the value is returned.

	int setStage(int pStage);
	//Precondition: The stage is passed in pStage
	//Postcondition: Value is stored in stage.




	int setEndLevel(int level);
	//Precondition: A value of the end level is passed in turn
	//Postcondition: Value is stored in turn.

	int getEndLevel();
	//Precondition: None
	//Postcondition: Current value of the end level is returned.

	bool getMarketSuccess();
	bool setMarketSuccess(bool value);
	
	bool getBuySuccess();
	bool setBuySuccess(bool value);

	int getNumberPasses();
	int incNumberPasses();
	int setNumberPasses(int value);


	int incTurn(int order[], int number);
		// Precondition: The order of the entities is stored in array order with the index of the first entity
		//				stored in order[0]. The number of entities is stored in number. The current round is 
		//				stored in round.
		// Postcondition: The index of the entity to go next is returned. Round is updated as appropirate. A -1 is returned if the conditions
		//				for the value of endLevel is greater than or equal to the end game threshold.

	int incTurn2(int order[], int number);


private:
	int turn = 0; // Index of current Player or company
	int round = 0; // Current round
	int step = 0; // Cumulative total of turns
	int stepStore = 0; // Storage for current step
	int stage = 0; // Cumulative total of rounds
	bool marketSuccess; //Indicates player did not only pass
	bool buySuccess; // Inicates player bought shares and market turn should end
	int numberPasses; //Number of passes in turn
	int playerOrder[maxPlayers] = { 0,1,2,3,4,5,6,7,8};
	int companyOrder[maxCompanies] = { 0,1,2,3,4,5,6,7,8 };
	int endLevel = 2;
};

