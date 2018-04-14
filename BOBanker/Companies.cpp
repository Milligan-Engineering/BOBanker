#include "stdafx.h"
#include "Companies.h"
#include "Trains.h"
#include "Cities.h"


using namespace std;

Trains trainsC(0);
Cities citiesC(0);

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
		for (int k = 0; k < maxCoal; k++)
		{
			cities[j][k] = false;
		}
		for (int k = 0; k < maxTrains; k++)
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
int Companies::getCompanyRunCount(int player)
{
	int count = 0;
	for (int j = 0; j < maxCompanies; j++)
	{
		if (president[j] == player)
		{
			count++;
		}
	}
	return(count);
}

int Companies::getCompanyRun(int player, int index)
{
	int count = 0;
	int companiesRun[maxCompanies];
	for (int j = 0; j < maxCompanies; j++)
	{
		if (president[j] == player)
		{
			companiesRun[count] = j;
			count++;
		}
	}
	return(companiesRun[index]);
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
int Companies::getCityCount(int company)
{
	int count = 0;
	for (int k = 0; k < maxCities; k++)
	{
		if (cities[company][k])
		{
			count++;
		}
	}
	return(count);
}

int Companies::getCityServiced(int company, int number)
{
	int count = 0;
	int cityServiced[maxCities];
	for (int k = 0; k < maxCities; k++)
	{
		if (cities[company] [k])
		{
			cityServiced[count] = k;
			count++;
		}
	}
	return(cityServiced[number]);
}

//Coal functions
bool Companies::getCoal(int company, int coal)
{
	return(coals[company][coal]);
}
int Companies::setCoal(int company, int coal)
{
	coals[company][coal] = true;
	return (coals[company][coal]);
}
int Companies::clearCoal(int company, int coal)
{
	coals[company][coal] = false;
	return (coals[company][coal]);
}
int Companies::getCoalCount(int company)
{
	int count = 0;
	for (int k = 0; k < maxCoal; k++)
	{
		if (coals[company][k])
		{
			count++;
		}
	}
	return(count);
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


//Train Funtions


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
int Companies::buyTrainForSale(int company)
{
	int train = getTrainForSale();
	trains[company][train] = 1;
	cout << "++Train " << train + 1 << " has been purchased for $" << trainsC.cost[train] << endl;
	setTechLevel(0);
	return (trainsC.cost[train]); //Return cost of highest train not owned
}
int Companies::returnTrainForSale(int company)
{
	int train = getTrainForSale();
	trains[company][train-1] = 0;
	cout << "++Train " << train + 1 << " has been returned for $" << trainsC.cost[train] << endl;
	setTechLevel(0);
	return (trainsC.cost[train-1]); //Return cost of highest train not owned
}
int Companies::getTrainCount(int company)
{
	int numberTrains = 0;
	for (int k = 0; k < 30; k++)
	{
		if (trains[company][k] == 1)
		{
			numberTrains++;
		}
	}
	return (numberTrains);
}
int Companies::getTrainOwned(int company, int index)
{
	int count = 0;
	int trainsOwned[maxTrains];
	for (int k = 0; k < 30; k++)
	{
		if (trains[company][k] == 1)
		{
			trainsOwned[count] = k;
			count++;
		}
	}
	return (trainsOwned[index]);
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
int Companies::unscrapTrainOfCompany(int train, int company)
{
	trains[company][train] = 1;
	cout << "++Train " << train + 1 << " has been unscrapped for for $" << trainsC.scrap[train] << endl;
	return (trainsC.scrap[train]);
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
int Companies::getTrains(int company, int train)
{
	return(trains[company][train]);
}
int Companies::setTrains(int company, int train, int value)
{
	trains[company][train] = value;
	return(0);
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



// Tech Level functions

int Companies::getTechLevel()
{
		return(techLevel);
}

int Companies::setTechLevel(int level)
{
	int train = getTrainForSale();
	if (level <= 0)
	{
		if (train > 0)
		{
			techLevel = (train + 4) / 5;
		}
		else
		{
			techLevel = 1; //Return tech level 1 if not started
		}

	}
	else
	{
		techLevel = level;
	}
	return((train + 4) / 5);
}



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
int Companies::calcNP(int company, int netProfitList[])
{
	bool profits[100];
	for (int i = 0; i < 100; i++)
		profits[i] = false;
	int n = getCityCount(company);
	int k = getServiceCapacity(company);
	if ((k == 0)||(n==0))
	{
		netProfitList[0] = 0;
		return(1);
	}
	if (k > n)
	{
		k = n;
	}
	int sum = 0;
	for (int i = 0; i < n - k + 1; i++)
	{
		addone(profits, sum, n, k, i, 1, company);
	}
	int count = 0;
	for (int i = 0; i < 100; i++)
	{
		if (profits[i])
		{
			netProfitList[count] = (i * 10) - (getTechLevel()*getTrainCount(company) * 10) + getCoalCount(company) * 40;
			count++;
		}
	}
	return(count);
}
int Companies::addone(bool profits[], int sum, int n, int k, int start, int nsum, int company)
{
	sum = sum + citiesC.cprofit[getCityServiced(company,start)][getTechLevel()-1];
	if (nsum > k - 1)
	{
		cout << sum << endl;
		profits[sum / 10] = true;
		return(sum);
	}
	else
	{
		for (int i = start; i < n - k + nsum; i++)
		{
			addone(profits, sum, n, k, i + 1, nsum + 1, company);
		}
		return(sum);
	}
}




