#include "stdafx.h"
#include "Game.h"


Game::Game(int pTurn, int pRound)
{
	turn = pTurn;
	round = pRound;
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

int Game::getPasses()
{
	return(numberPasses);
}
int Game::setPasses(int value)
{
	numberPasses = value;
	return(0);
}




