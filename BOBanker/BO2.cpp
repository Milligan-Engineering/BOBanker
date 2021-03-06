
#include "stdafx.h"

#include "Trains.h"
#include "TechLevels.h"
#include "Companies.h"
#include "Players.h"
#include "Cities.h"


using namespace std;

// Game functions

int setGame(int numberPlayers);
//Precondition: number of players has been set and passed in numberPlayers
//Postcondition: game conditions set and current tech level is passed.
//Game will resume at beginning of market round

int startGame();


void setupPlayers();
//Precondition: None
//Postcondition: After running all the players will be loaded into the class players
// with initial values and turn orders set

int endGame();

// Market

int marketRound();
//Precondition: 
//Postcondition:

int marketTurn(int player);
//Precondition: Given player index in player
//Postcondition: Will  run player's turn until done and then
//   return a 0. If player passes returns a 1.

bool attemptBuy(int player);
//Precondition: player < numberPlayers
//Postcondition: returns 1 if successfull and handles all the buying transactions

bool attemptSell(int player);
// This function will return a 1 when the player has sucessfully sold a stock for their turn.

bool attemptOrphan(int player);
// This function will return a 1 when a player has successfully had a company buy back orphaned stock.

int reduceForOrphans();
//Preconditions:To be run at end of market round
//Postconditions: Valuation of companies that have orphan shares
//				is reduced by one level.

//Building
int businessRound();
//Precondition: To be run after market round in loop of two attempts
//Postcondition: All companies get a turn.

int businessTurn(int company);
//Precondition: Comapny whose turn it is sent
//Postcondition: Business functions run.

int attemptScrapTrain(int company);
// Precondition: Company index - postitive intger
// Postcondition: Train is scrapped and money added to company or error given

int attemptBuyTrain(int company);
// Precondition: Company index - postitive intger
//Postcondition: Ifunds sufficient, company is designated owner of train and cost stutracted from cash.

void attemptBuildTrack(int company);
// Precondition: Company index - postitive intger

void setCompanyDividens(int currentCompany, char companyDividenPlan);
//Preconditions: For the current company given in currentCompany to pay dividens to shareholders and itself. Uses companyNetprofit, companySharesowned, and playerShares.
//Postcondition: Profits distributed to companyCash and playerCash

void setCompanyValuation(int company, int netProfitChange, char companyDividenPlan);
//Preconditions: For the company given by CurrentCompany that had a net profit change given by netProfitChange (decrease = -1, increase = 1
// and same = 0) and the decision to withhold (w) or distribute (d) dividens given in companyDividenPlan. Function also uses companyValuations
//Postcondition: Valuation of company is decrease if it had a decrease in net profit. It increase if net profit increased and dividens were paied. 
//Otherwise valuation stays the same.

// Utilities

void setPlayerNetworth();
//Precondition: players object has current cash and shares. Companies has current valuations
//Postcondition: players networth is calculated.

int setCompanyPresident(int company);
// Preconditions: company is a vaild company. The shares ownd by players is passed through playerShares and the current president it passed
// through companyPresident
// Postcondidtion: The number of shares owned by each player is compared and if a new player has more shares their number is returned through setCompanyPresident.
// If not the old president is passed.

char readValue(ifstream& inputStream, char cArray[]);
//Precondtions: Open stream in in inputStrean and character array is returned in cArray
//Postcondition: Will return last character read.

char findEOL(ifstream& inputStream);
//Precondition: Stream to be read for end of line is in inputStream
//				It is expected that all relevant values have been read
//Postcondition: Stream cleared. Last character returned. Should be /n if successful


// Data processing 

int fetchFileData(char pfileName[]);

int getManualData();

int pushFileData(char pfileName[]);





const int totalCash = 1500;
int trainOwner[30] = { -1,-1, -1, -1,-1, -1, -1, -1,-1, -1, -1,-1, -1, -1,-1, -1, -1,-1, -1, -1,-1, -1, -1,-1, -1, -1,-1, -1, -1,-1 };

Players players(totalCash);
Trains trains(0);
TechLevels techLevels;
Cities cities;
Companies companies(0);
int numberPlayers;
int techLevel = 1;
char testArray[30];
char wfileName[30];


int main()
{
	// BO Banker with Object
	char confirm;
	cout << "BO Banker Program \n \n";
	startGame();
	do
	{
		// Market Phase
		int numberPasses = 0;
		cout << "\nMARKET PHASE \n";
		marketRound();
		// Build Phase
		for (int l = 0; l < 2; l++)
		{
			cout << "Begin Build Round " << l + 1 << endl;
			businessRound();
		}
		pushFileData(wfileName);
		cout << "\nEnd of Business Rounds " << endl;;
	} while (techLevel < 6);
	endGame();
	cout << "Enter a key to end:";
	cin >> confirm;
	return 0;
}

int startGame()
{
	char option;

	// Set up options

	cout << "Name of game data file:";
	cin >> wfileName;
	strcat_s(wfileName, 30, ".csv");
	cout << "Enter the option of how you want to start the game:\n";
	cout << "What would you like to do? \n";
	cout << "   'n' = Start new game\n";
	cout << "   'm' = Manually enter game state \n";
	cout << "   'f' = Read game state from file\n";
	cout << "Option: ";
	cin >> option;
	cout << endl;

	switch (option)
	{
	case 'm':
		techLevel = getManualData();
		break;
	case 'f':
		techLevel = fetchFileData(wfileName);
		break;
	default:
		setupPlayers();
	}
	//Sort players by cash
	setPlayerNetworth();
	for (int i = 0; i < numberPlayers; i++)
		players.getInfo(i);
	for (int j = 0; j < maxCompanies; j++)
	{
		if (companies.getStarted(j) == 1)
			cout << j << ": " << companies.name[j] << " has $" << companies.getCash(j) << " and " << companies.getShares(j) << " regular shares and " << companies.getOrphans(j) << " orphaned shares valued at $" << valuations[companies.getValuation(j)] << " per share \n";
	}
	cout << endl;

	pushFileData(wfileName);
	return(0);
}
void setupPlayers( )
{
	// Set up players names
	char name[20];
	int tempOrder[maxPlayers] = { 0,1,2,3,4,5 };
	long int currentTime = static_cast<long int>(time(0)); //Generate random seed
	srand(currentTime);

//*****************************************
	cout << "Number of players: ";
	cin >> numberPlayers;
	cout << "There are " << numberPlayers << " of " << maxPlayers << " players. \n";
	cout << "Enter player names in any order. \n";
	for (int i = 0; i < numberPlayers; i++)
	{
		cout << "Player " << i + 1 << ": ";
		cin >> name;
		cout << "\n";
		strcpy_s(players.name[i], name);
		players.setCash(i,rand() % 10000); // assign random number
	}
	players.getTurnorders(tempOrder, numberPlayers);

	for (int i = 0; i < numberPlayers; i++)
	{
		int player = tempOrder[i];
		players.setCash(player,totalCash / numberPlayers);
		for (int j = 0; j < maxCompanies; j++)
		{
			players.setShares(player,j,0);
			players.clearSold(player,j);
		}
		players.getInfo(player);
	}
}
int endGame()
{
	cout << "End of the game.";
	int winner = 0;
	for (int i = 0; i < numberPlayers; i++)
	{
		if (players.getNetworth(i) > players.getNetworth(winner))
		{
			winner = i;
		}
	}
	cout << " The winner is " << players.name[winner] << "! \n";
	return(0);
}

int marketRound()
{
	int numberPasses = 0;
	int player;
	int orderPlayers[maxPlayers];

	while (numberPasses < numberPlayers)
	{
		numberPasses = 0;
		players.getTurnorders(orderPlayers, numberPlayers);
		for (int i = 0; i < numberPlayers; i++)
		{
			player = orderPlayers[i];
			numberPasses += marketTurn(player);
		}
		cout << "\nEnd of Market Turn \n";

		setPlayerNetworth();
		pushFileData(wfileName);
		//			for (int i = 0; i < numberPlayers; i++)
		//				players.getInfo(i);
		for (int j = 0; j < maxCompanies; j++)
		{
			if (companies.getStarted(j) == 1)
				cout << j << ": " << companies.name[j] << " has $" << companies.getCash(j) << " and " << companies.getShares(j) << " regular shares and " << companies.getOrphans(j) << " orphaned shares valued at $" << valuations[companies.getValuation(j)] << " per share \n";
		}
		cout << endl;
	}
	cout << "All players have passed \n";
	// Clear sold array
	for (int i = 0; i < numberPlayers; i++)
	{
		for (int j = 0; j < maxCompanies; j++)
		{
			players.clearSold(i, j);
		}
	}
	cout << "\nEnd of Market Round \n";
	numberPasses = 0;
	reduceForOrphans();
	return(0);
}
int marketTurn(int player)
{
	int pass = 1;
	int playerSuccess = 0;
	char option;
	do
	{
		cout << "\nCURRENT PLAYER:" << players.name[player] << " has $" << players.getCash(player) << "cash.\n";
		cout << "What would you like to do? \n";
		cout << "   'p' = pass \n";
		cout << "   'o' = orphan share buyback \n";
		cout << "   's' = sell shares\n";
		cout << "   'b' = buy shares\n";
		cout << "   'f' = finished with turn after selling \n ";
		cout << "Option: ";
		cin >> option;
		cout << endl;

		switch (option)
		{
		case 'p':
			cout << players.name[player] << " passes \n";
			playerSuccess = 1;
			break;
		case 'o':
			if (attemptOrphan(player))
			{
				playerSuccess = 1;
				pass = 0;
			}
			else
				playerSuccess = 0;
			break;
		case 's':
			if (attemptSell(player))
			{
				pass = 0;
			}
			playerSuccess = 0;
			break;
		case 'b':
			if (attemptBuy(player))
			{
				playerSuccess = 1;
				pass = 0;
			}
			else
				playerSuccess = 0;
			break;
		case 'f':
			playerSuccess = 1;
			break;
		default:
			cout << "Invalid option. Try agian. \n";
		}
		setPlayerNetworth();
		cout << endl;
		for (int i = 0; i < numberPlayers; i++)
			players.getInfo(i);
	} while (playerSuccess != 1);
	return(pass);
}
bool attemptBuy(int player)
{
	// Player requests to buy

	int numberShares;
	int requestValuation;
	int test;
	int option;
	int company;

	cout << "Select company to buy from\n";
	//List regular shares for sale
	for (int j = 0; j < maxCompanies; j++)
	{
		if (companies.getAvailable(j, techLevel))
		{
			if (players.getSold(player,j) != 1)
			{
				if (companies.getStarted(j) == 0)
				{
					cout << j << ": " << companies.name[j] << " is available to be started at $" << valuations[techLevels.lowVal[techLevel - 1]] << ", $" << valuations[techLevels.medVal[techLevel - 1]] << " or $" << valuations[techLevels.highVal[techLevel - 1]] << " per share\n";
				}
				else
				{
					cout << j << ": " << companies.name[j] << " has $" << companies.getCash(j) << " and " << companies.getShares(j) << " regular shares and " << companies.getOrphans(j) << " orphaned shares valued at $" << valuations[companies.getValuation(j)] << " per share \n";
				}
			}
		}
	}
	cout << "Selection:";
	cin >> company;
	// Three options: not avialable, buy regular shares, buy orphaned shares.

	if ((company < 0) || (company > maxCompanies + maxCompanies - 1)) // Company not avialable
	{
		cout << "Company  nubmer" << company << " is not valid. Please try again. \n";
		cin.clear();
		return 0;
	}
	else if (players.getSold(player,company) != 0)
	{
		cout << "Can not buy shares of company sold during this round. Please try again. \n";
		cin.clear();
		return 0;
	}
	else if (company < maxCompanies) // Regular Shares
	{
		if (!companies.getAvailable(company, techLevel))
		{
			cout << "Company " << companies.name[company] << " is not available. \n";
			return 0;
		}
		// Check to see if company has not been started and needs intial value set
		if (!companies.getStarted(company))
		{
			cout << "\t 1: $" << valuations[techLevels.lowVal[techLevel - 1]] << endl;
			cout << "\t 2: $" << valuations[techLevels.medVal[techLevel - 1]] << endl;
			cout << "\t 3: $" << valuations[techLevels.highVal[techLevel - 1]] << endl;
			cout << "Select inital company valuation for " << companies.name[company] << " :";
			cin >> option;
			if (option == 1)
				requestValuation = techLevels.lowVal[techLevel - 1];
			else if (option == 2)
				requestValuation = techLevels.medVal[techLevel - 1];
			else if (option == 3)
				requestValuation = techLevels.highVal[techLevel - 1];
			else
			{
				cout << "Request not valid. Please try again.";
				return 0;
			}
			cout << companies.name[company] << " has requested an intial valuations of $" << valuations[requestValuation] << " per share \n";
		}
		else
		{
			requestValuation = companies.getValuation(company);
		}
		cout << "How many shares regular shares of " << companies.name[company] << " do you want to buy: ";
		cin >> numberShares;
		// check validity of request
		if (numberShares < 0)
		{
			cout << "Request not valid. Please try again. \n";
			cin.clear();
			return 0;
		}
		if (numberShares > companies.getShares(company))
		{
			cout << "Only " << companies.getShares(company) << " shares available.\n";
			cin.clear();
			return 0;
		}
		else if (numberShares*valuations[requestValuation] > players.getCash(player))
		{
			cout << "Need " << numberShares * valuations[requestValuation] << " for purchase\n";
			return 0;
		}
		else // Proceed with sale
		{
			test = companies.setStarted(company);
			test = companies.setValuation(company, requestValuation);
			test = companies.changeShares(company, -numberShares);
			test = companies.changeCash(company, (numberShares * valuations[requestValuation]));
			players.changeShares(player,company,numberShares);
			players.changeCash(player,-(numberShares * valuations[requestValuation]));
			cout << companies.name[company] << " now has $" << companies.getCash(company) << " and " << companies.getShares(company) << " regular shares and " << companies.getOrphans(company) << " orphaned shares valued at $" << valuations[companies.getValuation(company)] << " per share \n";
			cout << players.name[player] << " now owns " << players.getShares(player,company) << " shares of " << companies.name[company] << " and $" << players.getCash(player) << endl;
			test = setCompanyPresident(company);

			if (companies.getOrphans(company) != 0) // Offer Orphans if any
			{
				cout << "How many ORPHAN shares of " << companies.name[company] << " do you want to buy: ";
				cin >> numberShares;
				// check validity of request
				if (numberShares < 0)
				{
					cout << "Request not valid. Please try again. \n";
					cin.clear();
					return false;
				}
				if (numberShares > companies.getOrphans(company))
				{
					cout << "Only " << companies.getOrphans(company) << " shares available.\n";
					cin.clear();
					return false;
				}
				else if (numberShares*valuations[requestValuation] > players.getCash(player))
				{
					cout << "Need " << numberShares * valuations[requestValuation] << " for purchase\n";
					return false;
				}
				else // Proceed with sale
				{
					test = companies.changeOrphans(company, -numberShares);
					players.changeShares(player,company,numberShares);
					players.changeCash(player, -(numberShares * valuations[requestValuation]));
					cout << companies.name[company] << " now has $" << companies.getCash(company) << " and " << companies.getShares(company) << " regular shares and " << companies.getOrphans(company) << " orphaned shares valued at $" << valuations[companies.getValuation(company)] << " per share \n";
					cout << players.name[player] << " now owns " << players.getShares(player,company) << " shares of " << companies.name[company] << " and $" << players.getCash(player) << endl;
					test = setCompanyPresident(company);
				}
			}
		}
		return true;
	}
	return false;
}
bool attemptSell(int player)
	// This function will return a 1 when the player has sucessfully sold a stock for their turn.
{
	int numberShares;
	int test;
	int company;

	cout << players.name[player] << " wants to sell stock. \n";
	for (int j = 0; j < maxCompanies; j++)
	{
		if ((players.getShares(player,j) != 0))
		{
			cout << j << " " << companies.name[j] << ": " << players.getShares(player,j) << " at $" << valuations[companies.getValuation(j)] << " per share \n";
		}
	}
	cout << "Select company to sell from:";
	cin >> company;
	if ((company < 0) || (company > maxCompanies - 1))
	{
		
		//*************************************************************************
		cout << "Company  nubmer" << company << " is not valid. Please try again. \n";
		cin.clear();
		cin.ignore(10, '\n');
		return false;
	}
	
	// Removed to allow multiple sales of same stock during turn and round
	/*if (players.getSold(player,company))
	{
		cout << "Company  nubmer" << company << " has been sold by you this turn. You must wait until a later turn to sell more. \n"; 
		cin.clear();
		cin.ignore(10, '\n');
		return false; 
	} */
	cout << "How many shares of " << companies.name[company] << " do you want to sell: ";
	cin >> numberShares;
	// check validity of request
	if (numberShares< 0)
	{
		cout << "Request not valid. Please try again. \n";
		cin.clear();
		cin.ignore(10, '\n');
		return false;
	}
	if (numberShares > players.getShares(player,company))
	{
		cout << "Only " << players.getShares(player,company) << " shares owned. Reduce request.\n";
		cin.clear();
		cin.ignore(10, '\n');
		return false;
	}
	else if (players.totalSharesOwned(company) <= numberShares)
	{
		cout << "Can not sell all shares of company " << companies.name[company] << ". Reduce request. \n";
		return false;
	}
	else  // Proceed with sale
	{
        test = companies.changeOrphans(company,numberShares);
        players.changeShares(player,company, -numberShares);
        players.changeCash(player,(numberShares * valuations[companies.getValuation(company)]));
 		// Reduce valuation if first sale of round by player. If second or greater sale there is no additional reduction.
		if (!players.getSold(player,company))
		{
			test = companies.decreaseValuation(company);
			cout << companies.name[company] << " has been sold this turn its valuation has been reduced to $" << valuations[companies.getValuation(company)] << " per share. \n";
		}
		cout << companies.name[company] << " now has $" << companies.getCash(company) << " and " << companies.getShares(company) << " regular shares and " << companies.getOrphans(company) << " orphaned shares valued at $" << valuations[companies.getValuation(company)] << " per share \n";
		cout << players.name[player] << " now owns " << players.getShares(player,company) << " shares of " << companies.name[company] << " and $" << players.getCash(player) << endl;
		test = setCompanyPresident(company);
		players.setSold(player,company);
		return true;
	}
	return false;
}
bool attemptOrphan(int player)
{
	int numberShares;
	int numberOrphanCompaniesControlled = 0;
	int company;
	int test;

	cout << players.name[player] << " wants a company to buy orphaned stock \n";
	cout << "Select company to buy its orphaned stock:\n";
	//List orphaned shares for sale
	for (int j = 0; j < maxCompanies; j++)
	{
		if (companies.getOrphans(j) != 0 && player == companies.getPresident(j))
		{
			cout << j << ": " << companies.name[j] << " has " << companies.getOrphans(j)<< " orphaned shares available at $" << valuations[companies.getValuation(j)] << " per share. \n";
			numberOrphanCompaniesControlled++;
		}
	}
	if (numberOrphanCompaniesControlled == 0)
	{
		cout << players.name[player] << " owns no companies with orphan stocks \n";
		return 0;
	}
	cin >> company;
	if ((company < 0) || (company > maxCompanies - 1))
	{
		cout << "Company  nubmer" << company << " is not valid. Please try again. \n";
		cin.clear();
		cin.ignore(10, '\n');
		return false;
	}
	if (player != companies.getPresident(company))
	{
		cout << "Only " << companies.name[company] << " president, " << players.name[companies.getPresident(company)] << " can buy orphan shares on behalf of company \n";
		return false;
	}
	cout << "How many orphaned shares of " << companies.name[company] << " do you want to buy back: ";
	cout << companies.name[company] << " has $" << companies.getCash(company)<< endl;
	cin >> numberShares;
	// check validity of request
	if (numberShares< 0)
	{
		cout << "Request not valid. Please try again. \n";
		cin.clear();
		cin.ignore(10, '\n');
		return false;
	}
	if (numberShares > companies.getOrphans(company))
	{
		cout << "Only " << companies.getOrphans(company) << " shares available.\n";
		cin.clear();
		cin.ignore(10, '\n');
		return false;
	}
	else if (numberShares*valuations[companies.getValuation(company)] > companies.getCash(company))
	{
		cout << "Need " << numberShares*valuations[companies.getValuation(company)] << " for purchase\n";
		return false;
	}
	else
	{

        test = companies.changeOrphans(company,-numberShares);
        test = companies.changeShares(company,numberShares);
        test = companies.changeCash(company, -numberShares*valuations[companies.getValuation(company)]);
        cout << companies.name[company] << " now has $" << companies.getCash(company) << " and " << companies.getShares(company) << " regular shares and " << companies.getOrphans(company) << " orphaned shares \n";
  		return true;
	}
}
int reduceForOrphans()
	{
		for (int j = 0; j < maxCompanies; j++)
		{
			if (companies.getOrphans(j) != 0)
			{
				companies.decreaseValuation(j);
				cout << companies.name[j] << " valuation decreases to $" << valuations[companies.getValuation(j)] << " per share due to orphaned share. \n";
			}

		}
		return (0);
	}

int businessRound()
{
	int company;
	int orderCompany[maxCompanies];

	companies.getTurnorders(orderCompany);
	for (int j = 0; j < maxCompanies; j++)
	{
		company = orderCompany[j];
		businessTurn(company);
	}
	setPlayerNetworth();
	for (int j = 0; j < maxCompanies; j++)
	{
		if (companies.getStarted(j) == 1)
			cout << j << ": " << companies.name[j] << " has $" << companies.getCash(j) << " and " << companies.getShares(j) << " regular shares and " << companies.getOrphans(j) << " orphaned shares valued at $" << valuations[companies.getValuation(j)] << " per share \n";
	}
	return(0);
}
int businessTurn(int company)
{
	int profit;
	char confirm;
	char option;

	if (companies.getStarted(company))
	{
		cout << endl << companies.name[company] << "building phase. \n\n";
		cout << companies.name[company] << " has $ " << companies.getCash(company) << " Would you like to scrap a train ? (y/n): ";
		cin >> confirm;
		cin.clear();
		cin.ignore(10, '\n');
		while ((confirm != 'n') && (confirm != 'N'))
		{
			attemptScrapTrain(company);
			cout << companies.name[company] << " has $ " << companies.getCash(company) << " Would you like to scrap a train ? (y/n): ";
			cin >> confirm;
			cin.clear();
			cin.ignore(10, '\n');
		}
		cout << companies.name[company] << " has $ " << companies.getCash(company) << " Would you like to buy the next train ? (y/n): ";
		cin >> confirm;
		cin.clear();
		cin.ignore(10, '\n');
		while ((confirm != 'n') && (confirm != 'N'))
		{
			if ((confirm == 'y') || (confirm == 'Y'))
			{
				attemptBuyTrain(company);
			}
			cout << companies.name[company] << " has $ " << companies.getCash(company) << " Would you like to buy the next train ? (y/n): ";
			cin >> confirm;
			cin.clear();
			cin.ignore(10, '\n');
		}
		cout << companies.name[company] << " has $ " << companies.getCash(company) << " Would you like to buy track ? (y/n): ";
		cin >> confirm;
		cin.clear();
		cin.ignore(10, '\n');
		while ((confirm != 'n') && (confirm != 'N'))
		{
			if ((confirm == 'y') || (confirm == 'Y'))
			{
				attemptBuildTrack(company);
			}
			cout << companies.name[company] << " has $ " << companies.getCash(company) << " Would you like to buy track ? (y/n): ";
			cin >> confirm;
			cin.clear();
			cin.ignore(10, '\n');
		}

		// Run Company
		cout << companies.name[company] << " finished building. \n";
		int netprofitChange;
		do
		{
			cout << "Enter new net profit $";
			cin >> profit;

			if ((profit >= 0) && (profit < 1000))
			{
				cout << "New net profit for " << companies.name[company] << " is $" << profit << ". Is this correct? (y/n): ";
				cin >> confirm;
				cin.clear();
				cin.ignore(10, '\n');
			}
			else
			{
				cout << "Invalid entry. Please try again. \n";
				cin.clear();
				cin.ignore(10, '\n');
				confirm = 'n';
			}
		} while (confirm != 'y');
		if (profit == 0)
		{
			netprofitChange = -2;
			cout << "Net profit for " << companies.name[company] << " is 0. Company is unprofitable \n";
		}
		else if (profit > companies.getNetprofit(company))
		{
			netprofitChange = 1;
			cout << "Net profit for " << companies.name[company] << " has increased. \n";
		}
		else if (profit < companies.getNetprofit(company))
		{
			netprofitChange = -1;
			cout << "Net profit for " << companies.name[company] << " has decreased. \n";
		}
		else
		{
			netprofitChange = 0;
			cout << "Net profit for " << companies.name[company] << " has remained the same. \n";
		}
		companies.setNetprofit(company, profit);
		do
		{
			cout << "Does " << companies.name[company] << " want to distribute=d or withold=w dividens? ";
			cin >> option;
			if (option == 'd')
			{
				cout << companies.name[company] << " would like to distribute dividens, Is this correct? (y/n): ";
				cin >> confirm;
				cin.clear();
				cin.ignore(10, '\n');
			}
			else if (option == 'w')
			{
				cout << companies.name[company] << " would like to withhold dividens, Is this correct? (y/n): ";
				cin >> confirm;
				cin.clear();
				cin.ignore(10, '\n');
			}
			else
			{
				cout << "Invalid entry. Please try again \n";
				confirm = 'n';
			}
		} while (confirm != 'y');
		setCompanyDividens(company, option);
		setCompanyValuation(company, netprofitChange, option);
		setPlayerNetworth();
		cout << endl;
		for (int i = 0; i < numberPlayers; i++)
			players.getInfo(i);
	}
	pushFileData(wfileName);
	return(0);
}
int attemptScrapTrain(int company)
	{
		int trainNumber;

		if (companies.getCompanyTrains(company) <= 0)
		{
			cout << " Company has no trains to scrap \n";
		}
		else
		{
			cout << "Select train to scrap: ";
			cin >> trainNumber;
			if (companies.getTrains(company, trainNumber - 1) == 1)
			{
				companies.changeCash(company, +companies.scrapTrainOfCompany(trainNumber - 1, company));
				cout << companies.name[company] << " now has $" << companies.getCash(company) << endl;
			return(trainNumber);	
			}
			else
			{
				cout << " Company does not own train: " << trainNumber << endl;
				return(0);

			}
		}

	}
int attemptBuyTrain(int company)
{
	int train = companies.getTrainForSale();
	if (train >= 30)
	{
		cout << "There are no more trains for sale. \n";
		return (30);
	}
	if (companies.getCash(company) < trains.cost[train])
	{
		cout << "Company does not have enough cash to buy train. \n";
		return (-1);
	}
	companies.changeCash(company, -companies.buyTrainForSale(company, techLevel));
	cout << companies.name[company] << " now has $" << companies.getCash(company) << endl;
	techLevel = companies.getTechLevel(techLevel);
	return(train);
}
void attemptBuildTrack(int company)
	{
		int amount;
		cout << "What is the total track build abount: ";
		cin >> amount;
		if ((amount < 0) || (amount > 500))
		{ 
			cout << "$" << amount << " is not a valid amount. \n";
			cin.clear();
			cin.ignore(10, '\n');
		}
		else if (companies.getCash(company) >= amount)
		{
			companies.changeCash(company,-amount);
			cout << companies.name[company] << " now has $" << companies.getCash(company) << endl;
		}
		else
		{
			cout << "Company does not have enough cash to build this amount of track \n";
		}
		return;
	}
void setCompanyValuation(int company, int netProfitChange, char companyDividenPlan)
{
	if (netProfitChange < 0)
	{
		if (companies.getNetprofit(company) != 0)
		{
			companies.decreaseValuation(company);
			cout << companies.name[company] << " valuation decreases to $" << valuations[companies.getValuation(company)] << " per share. \n";
		}
		else
		{
			// Unprofitable Company
			int badPresident = companies.getPresident(company);
			companies.decreaseValuation(company);
			companies.decreaseValuation(company);
			cout << companies.name[company] << " valuation decreases to $" << valuations[companies.getValuation(company)] << " per share. \n";
			companies.changeShares(company,1);
			players.changeShares(setCompanyPresident(company),company, -1);
			cout << players.name[badPresident] << " shares in " << companies.name[company] << " has been reduced to " << players.getShares(badPresident,company) << " shares. \n";
			if (players.getShares(setCompanyPresident(company),company) == 0)
			{
				// Company goes into recievership
				cout << companies.name[company] << " goes into recievership. \n";
				companies.setRecievership(company); // Place into revievership
				companies.clearStarted(company); // Clear started flag
				companies.setValuation(company, 4); //Set valuation to $50
				companies.scrapCompanyTrains(company); //Scrap all trains
				companies.setCash(company, 0); //Clear out treasurey
				companies.changeOrphans(company, -companies.getOrphans(company)); //Clear out orphans
				companies.changeShares(company, 10 - companies.getShares(company)); // Put all shares regular
				cout << companies.name[company] << " now has $" << companies.getCash(company) << " and " << companies.getShares(company) << " regular shares and " << companies.getOrphans(company) << " orphaned shares \n";
			}
		}
}
    else if ((netProfitChange > 0) && (companyDividenPlan == 'd'))
    {
        companies.increaseValuation(company);
		cout << companies.name[company] << " valuation increases to $" << valuations[companies.getValuation(company)] << " per share. \n";

    }
    else
    {
        cout << companies.name[company] << " valuation remains the same at $" << valuations[companies.getValuation(company)] << " per share. \n";
    }
    return;
}
void setCompanyDividens(int company, char option)
	{
		if (option == 'd')
		{
			companies.changeCash(company, (companies.getShares(company) * companies.getNetprofit(company)) / 10);
			for (int i = 0; i < numberPlayers; i++)
			{
				if (players.getShares(i,company) != 0)
				{
					int pDividend = players.getShares(i,company)*companies.getNetprofit(company) / 10;
					players.changeCash(i,pDividend);
					cout << players.name[i] << " recieved $" << pDividend << " from " << players.getShares(i,company) << " shares of " << companies.name[company] << " and now has $" << players.getCash(i) << endl;
				}
			}
		}
		else
		{
			companies.changeCash(company, companies.getNetprofit(company));
		}
			cout << companies.name[company] << " now has $" << companies.getCash(company) << " and " << companies.getShares(company) << " regular shares and " << companies.getOrphans(company) << " orphaned shares \n";
			return;
	}

int setCompanyPresident(int company)
{
	int president = companies.getPresident(company);
	for (int i = 0; i < numberPlayers; i++)
	{
		if (players.getShares(i,company) > players.getShares(president,company))
		{
			companies.setPresident(company,i);
			president = i;
		}
	}
	cout << "The president for " << companies.name[company] << " is " << players.name[president] << endl;
	return(president);
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

int fetchFileData(char pfileName[])
{

	char getChar;
	ifstream inDataStream;
	int rnumberPlayers = 0;
	inDataStream.open(pfileName);
	if (inDataStream.fail())
	{
		cout << "Input file stream open failed \n";
		return(1);
	}
	getChar = readValue(inDataStream, testArray);
	getChar = readValue(inDataStream, testArray);
	while (strcmp(testArray,"Done#")!=0)	//Read player names
	{
		strcpy_s(players.name[rnumberPlayers], testArray);
		rnumberPlayers++;
		getChar = readValue(inDataStream, testArray);
	}
	numberPlayers = rnumberPlayers;
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int i = 0; i < numberPlayers; i++) // Read player cash
	{
		getChar = readValue(inDataStream, testArray);
		players.setCash(i,atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) // Read player shares owned
	{
		for (int i = 0; i < numberPlayers; i++)
		{
			getChar = readValue(inDataStream, testArray);
			players.setShares(i,j,atoi(testArray));
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
				players.setSold(i,j);
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
	return(companies.getTechLevel(1));
}
int getManualData()
{
	do
	{
		cout << "Enter number of players between 2 and 6: ";
		cin >> numberPlayers;
		cin.clear();
		cin.ignore(10, '\n');
	} while ((numberPlayers < 2) || (numberPlayers > 6));
	cout << "There are " << numberPlayers << " of " << maxPlayers << " players. \n";
	techLevel = setGame(numberPlayers);
	setPlayerNetworth();
	for (int i = 0; i < numberPlayers; i++)
		players.getInfo(i);
	for (int j = 0; j < maxCompanies; j++)
	{
		if (companies.getStarted(j) == 1)
		{
			setCompanyPresident(j);
			cout << j << ": " << companies.name[j] << " has $" << companies.getCash(j) << " and " << companies.getShares(j) << " regular shares and " << companies.getOrphans(j) << " orphaned shares valued at $" << valuations[companies.getValuation(j)] << " per share \n";
		}
	}
	return(techLevel);
}
int setGame(int numberPlayers)
{
	// Restet TechLevel
	int techLevel;
	cout << "Enter current Tech Level: ";
	cin >> techLevel;
	int value;
	char confirm;
	// Reset Companies
	for (int j = 0; j < maxCompanies; j++)
	{
		if (companies.getAvailable(j, techLevel))
		{
			cout << "\n For company: " << companies.name[j] << " \n";
			cout << "Company started (y/n):";
			cin >> confirm;
			cin.clear();
			cin.ignore(10, '\n');
			if ((confirm == 'y') || (confirm == 'Y'))
			{
				companies.setStarted(j);
				cout << "\nCompany in recievership (y/n):";
				cin >> confirm;
				cin.clear();
				cin.ignore(10, '\n');
				if ((confirm == 'y') || (confirm == 'Y'))
				{
					companies.setRecievership(j);
					companies.clearStarted(j); // Clear started flag
					companies.setValuation(j, 4); //Set valuation to $50
				}
				else
				{
					cout << "\nCash: ";
					cin >> value;
					companies.setCash(j, value);
					cout << "\nNet Profit: ";
					cin >> value;
					companies.setNetprofit(j, value);
					cout << "\nValuation: ";
					cin >> value;
					for (int n = 0; n < 26; n++)
					{
						if (valuations[n] == value)
							companies.setValuation(j, n);
					}
					cout << "\nShares: ";
					cin >> value;
					companies.changeShares(j, value - 10);
					cout << "\nOrphans: ";
					cin >> value;
					companies.changeOrphans(j, value);
					cout << "\nTurn Order (0 is first): ";
					cin >> value;
					companies.setOrder(j, value);
					cout << endl;
					do
					{
						cout << "\nEnter train number of train(s) owned (0 to quit):";
						cin >> value;
						companies.setTrains(j,value, 1);
					} while (value > 0);

				}
			}
		}
	}
	// Reset Scrapped Trains
	do
	{
		cout << "\nEnter train number of train(s) scrapped (0 to quit):";
		cin >> value;
		companies.setTrains(0,value, -1);
	} while (value > 0);

	// Reset Players

	for (int i = 0; i < numberPlayers; i++)
	{
		cout << "\n\nFor player: " << players.name[i] << endl;
		cout << "\nCash: ";
		cin >> value;
		players.setCash(i, value);
		for (int j = 0; j < maxCompanies; j++)
		{
			if (companies.getAvailable(j, techLevel))
			{
				if (companies.getStarted(j) == 1)
				{
					cout << "\nEnter number of shares of " << companies.name[j] << " owned: ";
					cin >> value;
					players.setShares(i, j, value);
					cout << endl;
				}
			}
		}


	}
	return(techLevel);

}
int pushFileData(char pfileName[])
{

	ofstream outDataStream;
	outDataStream.open(pfileName);
	if (outDataStream.fail())
	{
		cout << "Output file stream open failed \n";
		return(1);
	}

	outDataStream << "Name, ";
	for (int i = 0; i < numberPlayers; i++) // Save player name
	{
		outDataStream << players.name[i] << ",";
	}
	outDataStream <<  "Done#," <<"\n";
	outDataStream << "Cash, ";
	for (int i = 0; i < numberPlayers; i++) // Save player cash
	{
		outDataStream << players.getCash(i) << ",";
	}
	outDataStream << "\n";
	for (int j = 0; j < maxCompanies; j++) // Save player shares owned
	{
		outDataStream << companies.name[j] << ",";
		for (int i = 0; i < numberPlayers; i++)
		{
			outDataStream << players.getShares(i,j) << ",";
		}
		outDataStream << "\n";
	}
	for (int j = 0; j < maxCompanies; j++) // Save player sold company
	{
		outDataStream << companies.name[j] << ",";
		for (int i = 0; i < numberPlayers; i++)
		{
			outDataStream << players.getSold(i, j) << ",";
		}
		outDataStream << "\n";
	}	outDataStream << "Names, ";
	for (int j = 0; j < maxCompanies; j++) // Save company names
	{
		outDataStream << companies.name[j] << ",";
	}
	outDataStream << "\n";
	outDataStream << "Cash, ";
	for (int j = 0; j < maxCompanies; j++) // Save company cash
	{
		outDataStream << companies.getCash(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "NetProfit, ";
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
		outDataStream << cities.name[m] << ",";
		for (int j = 0; j < maxCompanies; j++)
		{
			outDataStream << companies.getCity(j, m) << ",";
		}
		outDataStream << "\n";
	}
	for (int k = 0; k < maxTrains; k++) // Save trains
	{
		outDataStream << k+1 << ",";
		for (int j = 0; j < maxCompanies; j++)
		{
			outDataStream << companies.getTrains(j, k) << ",";
		}
		outDataStream << "\n";
	}
	outDataStream.close();
	return(techLevel);
}



