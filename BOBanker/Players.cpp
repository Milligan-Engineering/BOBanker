#include "stdafx.h"
#include "Players.h"

using namespace std;

//
/*
Players::Players(char nameVal[20], int cashVal, int networthVal, int sharesVal[maxCompanies])
{
 strcpy_s(name, nameVal);
 cash = cashVal;
 networth = networthVal;
 for (int j = 0; j < maxCompanies; j++)
 {
	 shares[j] = sharesVal[j];
 }
} */

Players::Players(int value)
{
	for (int i = 0; i < maxPlayers; i++)
	{
		cash[i] = 1000000;
		networth[i] = 0;
		order[i] = i;
		for (int j = 0; j < maxCompanies; j++)
		{
			shares[i][j] = 0;
			sold[i][j] = 0;
		}
	}

}

// Cash functions
int  Players::getCash(int player)
{
	return (cash[player]);
}
int  Players::setCash(int player, int amount)
{
	cash[player] = amount;
	setTurnorders();
	return(cash[player]);
}
int  Players::changeCash(int player, int amount)
{
	cash[player] += amount;
	setTurnorders();
	return(cash[player]);
}
int  Players::setCashM(int player, int amount)
{
	cash[player] = amount;
	return(cash[player]);
}


//Shares Routines
int  Players::getShares(int player, int company)
{
	return (shares[player][company]);
}
int  Players::setShares(int player, int company, int pshares)
{
	shares[player][company] = pshares;
	return(pshares);
}
int  Players::changeShares(int player, int company, int amount)
{
	shares[player][company] += amount;
	return(shares[player][company]);
}
int  Players::totalSharesOwned(int company)
{
	int sum = 0;
	for (int i = 0; i < maxPlayers; i++)
	{
		sum += shares[i][company];
	}
	return(sum);
}
int Players::getCompanyCount(int player)
{
	int count = 0;
	for (int j = 0; j < maxCompanies; j++)
	{
		if (shares[player][j] != 0)
		{
			count++;
		}
	}
	return(count);
}

int Players::getCompanyOwned(int player, int index)
{
	int count = 0;
	int companiesOwned[maxCompanies];
	for (int j = 0; j < maxCompanies; j++)
	{
		if (shares[player][j] != 0)
		{
			companiesOwned[count] = j;
			count++;
		}
	}
	return(companiesOwned[index]);
}





//Sold functions
int  Players::getSold(int player, int company)
{
	return (sold[player][company]);
}
int  Players::setSold(int player, int company)
{
	sold[player][company] = 1;
	return(1);
}
int  Players::clearSold(int player, int company)
{
	sold[player][company] = 0;
	return(0);
}

//Order functions
int  Players::getOrder(int player)
{
	return (order[player]);
}
int  Players::setOrder(int player, int porder)
{
	order[player] = porder;
	return(porder);
}
int Players::getTurnorders(int playerTurnorder[], int numberPlayers)
{
	// Determine  player turn order		
	// Sort players in order of cash
	for (int i = 0; i < maxPlayers; i++)
	{
		playerTurnorder[order[i]] = i; //Place local order in turnorder
	}

	return(0);
}
int Players::setTurnorders()
{
	// Determine  player turn order		
// do not change external turn order array
	int temp;
	int Turnorder[maxPlayers];
	for (int i = 0; i < maxPlayers; i++)
	{
		Turnorder[order[i]] = i; //Place local order in turnorder
	}
	for (int i = 0; i < maxPlayers - 1; i++)
	{
		for (int j = 0; j < maxPlayers - i - 1; j++)
		{
			if (cash[Turnorder[j]] > cash[Turnorder[j + 1]])
			{
				temp = Turnorder[j];
				Turnorder[j] = Turnorder[j + 1];
				Turnorder[j + 1] = temp;
			}
		}
	}
	//Store new turn order in local turn order
	for (int i = 0; i < maxPlayers; i++)
	{
		order[Turnorder[i]] = i;
	}
	return(0);
}

//Networth functions
int  Players::getNetworth(int player)
{
	return (networth[player]);
}
int  Players::setNetworth(int player, int amount)
{
	networth[player] = amount;
	return(networth[player]);
}


//Other functions

void Players::getInfo(int player)
{

	cout <<  name[player] << " has $" << cash[player] << " and a networth of $" << networth[player] << " and a turn order of " << order[player] << endl;

}

