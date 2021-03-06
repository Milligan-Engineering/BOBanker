#include "stdafx.h"
#include "Game.h"


//Game::Game(int pTurn, int pRound)
//{
//	turn = pTurn;
//	round = pRound;
//}
Game::Game()
{
	numberPasses = 0;
}

int Game::getTurn()
{
	return(turn);
}
int Game::setTurn(int pTurn)
{
	turn = pTurn;
	return(0);
}

int Game::getRound()
{
	return(round);
}
int Game::setRound(int pRound)
{
	round = pRound;
	return(0);
}

int Game::getEndLevel()
{
	return(endLevel);
}
int Game::setEndLevel(int pLevel)
{
	endLevel = pLevel;
	return(0);
}

bool Game::getMarketSuccess()
{
	return(marketSuccess);
}

bool Game::setMarketSuccess(bool value)
{
	marketSuccess = value;
	return(value);
}

bool Game::getBuySuccess()
{
	return(buySuccess);
}

bool Game::setBuySuccess(bool value)
{
	buySuccess = value;
	return(value);
}

int Game::getNumberPasses()
{
	return(numberPasses);
}
int Game::incNumberPasses()
{
	numberPasses++;
	return(numberPasses);
}
int Game::setNumberPasses(int value)
{
	numberPasses = value;
	return(numberPasses);
}

// Version for B&O
int Game::incTurn(int order[], int number)
{

	switch (round)
	{
	case 0:
	{
		if (turn < number - 1)
		{
			turn++;
		}
		else
		{
			turn = 0;
			round++;
		}
		return(order[turn]);
		break;
	}
	case 1:
	{
		if (turn < number - 1)
		{
			turn++;
		}
		else
		{
			turn = 0;
			round++;
		}
		return(order[turn]);
	}
	case 2:
	{
		if (turn < number - 1)
		{
			turn++;
		}
		else
		{
			turn = 0;
			round = 0;
		}
		if (endLevel < 6)
		{
			return(order[turn]);
		}
		else
		{
			return(-1);
		}
	}
	}
	return(0);
}

// Version for Catan
int Game::incTurn2(int order[], int number)
{

	switch (round)
	{
	case 0:
	{
		if (turn != number - 1)
		{
			turn++;
		}
		else
		{
			round++;
			turn = 0;
		}
		return(order[turn]);
		break;
	}
	case 1:
	{
		if (turn != 0)
		{
			turn--;
		}
		else
		{
			round++;
			turn = 0;
		}
		return(order[turn]);
		break;
	}
	case 2:
	{
		if (turn != number - 1)
		{
			turn++;
		}
		else
		{
			turn = 0;
		}
		return(order[turn]);
		if (endLevel < 12)
		{
			return(order[turn]);
		}
		else
		{
			return(-1);
		}
	}
	}
	return(order[turn]);
}

