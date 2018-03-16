#include "stdafx.h"
#include "Companies.h"
#include "Trains.h"


using namespace std;

Trains trainsC(0);

Companies::Companies(int setup)

{
	for (int j = 0; j < maxCompanies; j++)
	{
		cash[j] = 0;
		netprofit[j] = 0;
		valuation[j] = 0;
		shares[j] = 10;
		orphans[j] = 0;
		president[j] = maxPlayers;
		order[j] = j;
		started[j] = false;
		recievership[j] = false;
		for (int k = 0; k < maxCities; k++)
		{
			cities[j][k] = false;
		}
		for (int k = 0; k < maxCities; k++)
		{
			trains[j][k] = 0;
		}
	}
}



// Simple Get Routines

//Cash functions
int Companies::getCash(int company)
{
	return (cash[company]);
}
int Companies::setCash(int company, int value)
{
	cash[company] = value;
	return (cash[company]);
}
int Companies::changeCash(int company, int value)
{
	cash[company] += value;
	return (shares[company]);
}

//President functions
int Companies::getPresident(int company)
{
	return (president[company]);
}
int Companies::setPresident(int company, int player)
{
	president[company] = player;
	return(president[company]);
}

//Valuation functions
int Companies::getValuation(int company)
{
	return (valuation[company]);
}
int Companies::setValuation(int company, int value)
{
	valuation[company] = value;
	setTurnorders();
	return (valuation[company]);
}
int Companies::increaseValuation(int company)
{
	if (valuation[company] < 26)
	{
		valuation[company] ++;
		setTurnorders();
	}
	//	cout << name[company] << " now has a valuation of $" << valuations[valuation[company]] << " per share \n";
	return(valuation[company]);
}
int Companies::decreaseValuation(int company)
{
	if (valuation[company] > 0)
	{
		valuation[company] --;
		setTurnorders();
	}
	//	cout << name[company] << " now has a valuation of $" << valuations[valuation[company]] << " per share \n";
	return(valuation[company]);
}

//Net Profit functions
int Companies::getNetprofit(int company)
{
	return (netprofit[company]);
}
int Companies::setNetprofit(int company, int netprofitVal)
{
	netprofit[company] = netprofitVal;
	return(netprofit[company]);
}

//Shares fucntions
int Companies::getShares(int company)
{
	return (shares[company]);
}
int Companies::setShares(int company, int value)
{
	shares[company] = value;
	return (shares[company]);
}
int Companies::changeShares(int company, int value)
{
	shares[company] += value;
	return (shares[company]);
}

//Orphan functions
int Companies::getOrphans(int company)
{
	return (orphans[company]);
}
int Companies::setOrphans(int company, int value)
{
	orphans[company] = value;
	return (orphans[company]);
}
int Companies::changeOrphans(int company, int value)
{
	orphans[company] += value;
	return (orphans[company]);
}

//City functions
bool Companies::getCity(int company, int city)
{
	return(cities[company][city]);
}
int Companies::setCity(int company, int city)
{
	cities[company][city] = true;
	return (cities[company][city]);
}
int Companies::clearCity(int company, int city)
{
	cities[company][city] = false;
	return (cities[company][city]);
}

//Started functions
bool Companies::getStarted(int company)
{
	return (started[company]);
}
int Companies::setStarted(int company)
{
	started[company] = true;
	return (started[company]);
}
int Companies::clearStarted(int company)
{
	started[company] = false;
	return (started[company]);
}

//Recievership functions
bool Companies::getRecievership(int company)
{
	return (recievership[company]);
}
int Companies::setRecievership(int company)
{
	recievership[company] = true;
	return (recievership[company]);
}
int Companies::clearRecievership(int company)
{
	for (int j = 0; j< maxCompanies; j++)
		recievership[j] = false;
	return (0);
}

//Turn Order functions
int Companies::getOrder(int company)
{
	return (order[company]);
}
int Companies::setOrder(int company, int turn)
{
	order[company] = turn;
	return (order[company]);
}
int Companies::getTurnorders(int companyTurnorder[])
{
	for (int i = 0; i < maxCompanies; i++)
	{
		companyTurnorder[order[i]] = i; //Place local order in turnorder
	}
	return(0);
}
int Companies::setTurnorders()
{
	// Determine  company turn order		
	// do not change external turn order array
	int temp;
	int Turnorder[maxCompanies];
	for (int i = 0; i < maxCompanies; i++)
	{
		Turnorder[order[i]] = i; //Place local order in turnorder
	}
	for (int i = 0; i < maxCompanies - 1; i++)
	{
		for (int j = 0; j < maxCompanies - i - 1; j++)
		{
			if (valuation[Turnorder[j]] < valuation[Turnorder[j + 1]])
			{
				temp = Turnorder[j];
				Turnorder[j] = Turnorder[j + 1];
				Turnorder[j + 1] = temp;
			}
		}
	}
	//Store new turn order in local turn order
	for (int i = 0; i < maxCompanies; i++)
	{
		order[Turnorder[i]] = i;
	}
	return(0);
}


//Train routines


int Companies::getTrainForSale()
{
	int train =0;
	for (int k=0; k<maxTrains; k++)	// Go through trains starting with lowest
	{
		for (int j = 0; j < maxCompanies; j++)
		{
			if (trains[j][k]!=0) // If train owned or scrapped
			train = k+1;      // Indicate next highest train couold be for sale
		}
	}
	return (train); //Return highest train not owned
}

int Companies::buyTrainForSale(int company, int & techLevel)
{
	int train = getTrainForSale();
	trains[company][train] = 1;
	cout << "++Train " << train + 1 << " has been purchased for $" << trainsC.cost[train] << endl;
	techLevel = getTechLevel(techLevel);
	return (trainsC.cost[train]); //Return cost of highest train not owned
}

int Companies::getCompanyTrains(int company)
{
	int numberTrains = 0;
	for (int k = 0; k < 30; k++)
	{
		if (trains[company][k]==1)
		{
			cout << trainsC.number[k] << ": Train " << trainsC.number[k] << " with scrap value $" << trainsC.scrap[k] << endl;
			numberTrains++;
		}
	}
	return (numberTrains);
}
int Companies::scrapTrainOfCompany(int train, int company)
{
	if ((train<0) || (train>=30))
	{
		cout << "The company does not own the train to scrap \n";
		cin.clear();
		cin.ignore(10, '\n');
		return (0);
	}
	else if (trains[company][train] != 1)
	{
		cout << "The company does not own the train to scrap \n";
		return (0);
	}
	else
	{
		trains[company][train]= -1;
		cout << "++Train " << train+1 << " has been scrapped for for $" << trainsC.scrap[train] << endl;
		return (trainsC.scrap[train]);
	}
}

int Companies::getServiceCapacity(int company)
{
	int capacity = 0;
	for (int k = 0; k < 30; k++)
	{
		if (trains[company][k]==1)
		{
			capacity = capacity + trainsC.level[k];
		}
	}
	return(capacity);
}

int Companies::scrapCompanyTrains(int company)
{
	int numberTrains = 0;
	for (int k = 0; k < 30; k++)
	{
		if (trains[company][k] == 1)
		{
			trains[company][k] = -1;
			numberTrains++;
		}
	}
	return (numberTrains);
}
int Companies::getTechLevel(int currentTechLevel)
{
	int train = getTrainForSale();
	if (train > 0)
	{
		if (trainsC.level[train - 1] != currentTechLevel)
		cout << "++New tech level is " << trainsC.level[train - 1] << endl;
		return trainsC.level[train - 1];
	}
	else
	{
		return(1); //Return tech level 1 if not started
	}
	
}

int Companies::getTrains(int company, int train)
{
	return(trains[company][train]);
}

int Companies::setTrains(int company, int train, int value)
{
	trains[company][train] = value;
	return(0);
}


/*

ForSale(int companyVal)
{
int k = 0;
while ((owner[k] != -1) && (k < maxTrains))
{
k++;
}
else
{
cout << "Company bought train " << k << " for $" << cost[k] << endl;
owner[k] = companyVal;
return (cost[k]);
}
}


int Companies::priceTrainForSale()
{
int k = 0;
while ((owner[k] != -1) && (k < maxTrains))
{
k++;
}
if (k < maxTrains)
{
cout << "Train " << k << " is for sale for $" << cost[k] << endl;
}
return (cost[k]);
}

int Companies::buyTrain
int Companies::getCost(int indexVal)
{
return (cost[indexVal - 1]);
}


*/



// Complex Get Routines
bool Companies::getAvailable(int company, int techLevel)
{
    return ((available[company] <= techLevel));
}
void Companies::getInfo(int company)
{
	cout << name[company] << "Cash: $" << cash[company] << netprofit[company] << " val $" <<
		valuation[company] << " shr " << shares[company] << " pres " <<
		orphans[company] << " orp " << president[company] << " TO " <<
		order[company] << " strt " << started[company] << "sold " << sold[company] << endl;
		return;
}



