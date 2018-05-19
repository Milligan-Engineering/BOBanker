#pragma once


class Companies 
{
public:
	
	Companies();

	int getCash(int company);
    int setCash(int company, int cash);
	int changeCash(int company, int value);

	int getPresident(int company);
    int setPresident(int company, int player);
	int getCompanyRunCount(int player);
	int getCompanyRun(int player, int index);


	int getNetprofit(int company);
    int setNetprofit(int company, int netprofit);

	int getProfitPerShare(int company);
	int setProfitPerShare(int company, int value);

	int getShares(int company);
	int setShares(int company, int value);
    int changeShares(int company, int sharesVal);

	int getOrphans(int company);
	int setOrphans(int company, int value);
    int changeOrphans(int company, int sharesVal);


    bool getStarted(int company);
    int setStarted(int company);
	int clearStarted(int company);

	bool getRecievership(int company);
	int setRecievership(int company);
	int clearRecievership(int company);
	
	bool getCity(int company, int city);
	int setCity(int company, int city);
	int clearCity(int company, int city);
	int getCityCount(int company);
	// Precondition: index of company 
	// Postcondition: Returns number of cities the company currently services

	int getCityServiced(int company, int number);
	//Precondition: company and number of the city serviced by the company in order
	// For example: 3 would be the third city serviced of 8 total.
	//Postcondition: Returns the city idex

	bool getCoal(int company, int coal);
	int setCoal(int company, int coal);
	int clearCoal(int company, int coal);
	int getCoalCount(int company);
	// Precondition: index of company 
	// Postcondition: Returns number of cities the coals currently services




    int getNext(int order);
    void getInfo(int company);

	bool getSold(int company);
    int setSold(int company);
    int clearSold();
	int clearSold(int company);

	int getValuation(int company);
    int setValuation(int company, int value);
	int setValuationM(int company, int value);
    int increaseValuation(int company);
	int decreaseValuation(int company);

	bool getAvailable(int company, int techLevel);

	int getOrder(int company);
	int setOrder(int company, int turn);
	int getTurnorders(int companyTurnorder[]);
    //Precondition: The current turnorders of each companies are stored in turnorder.
    //Postcondition: This values are stored in oldTurnorder with the first company index
    // in 0, the second in 1, ...
	int setTurnorders();



	int getTrainForSale();
	//Precondition: None
	//Postcondition: Returns the index of the train that is currently for sale
	int returnTrainForSale(int company);
	int getTrainCount(int companyVal);
	// Precondition: index of company
	// Postcondition: Returns number of trains a company owns
	int getTrainOwned(int company, int index);
		//Precondition: company and index of how many trains owned by the company in order
		//Postcondition: Returns the train index.
	int scrapTrainOfCompany(int indexVal, int companyVal);
	//Precondition: Index of train that company owns and wants to scrap - number between 1 and 30.
	//Postcondition: Returns the scrap value of the train if no error. Returns
	//0 if there is and error. Also removes train from company's ownership.
	int unscrapTrainOfCompany(int indexVal, int companyVal);
	//Precondition: Index of train that company owns and wants to unscrap - number between 1 and 30.
	//Postcondition: Returns the scrap value of the train if no error. Returns
	//0 if there is and error. Also returns train from company's ownership.
	int getServiceCapacity(int companyVal);
	// Precondition: index of company purchasing the train - postive integer
	// Postcondition:  Returns the number of cities that the company can service.
	int scrapCompanyTrains(int companyVal);
	// Precondition: index of company purchasing the train - postive integer
	// Postcondition: Scraps all trains belowing to company
	//Precondition: None
	//Postcondition: Returns the current tech level based on trains purchased
	int getTrains(int company, int trains);
	//Precondition: index of train in indexVal
	//Postcondion: Returns the index of the company owning the train.
	int setTrains(int company, int train, int value);
	//Precondition: index of train in indexVal, and index owner of train in 
	//Postcondion: Returns the index of the company owning the train.
	int buyTrainForSale(int company);
	int getTrain(int company, int index);
	int setTrain(int company, int index, int value);



	int getTechLevel();
	//Postcondition: Returns current value stored in techLevel
	int setTechLevel(int level);
	// Precondition: The level to set techLevel to is passed in level
	//				If level = 0, techLevel is determined by the trains bought
	// Postcondition: Returns techLevel based on trains.

	int calcNP(int company, int netProfitList[]);
	//Precondition:
	//Postconditions:

	int addone(bool profits[], int sum, int n, int k, int start, int nsum, int company);


	COLORREF color[maxCities];
	wchar_t name[maxCompanies][35] = { L"Baltimore & Ohio", L"Boston & Maine", L"Chesapeake & Ohio", L"Illinois Central", L"Erie", L"New York Central",L"Nickel Plate", L"New York New Haven & Hartford", L"Pennsylvania", L"Wabash" };
	int available[maxCompanies] = { 1,1,1,3,3,1,3,1,1,3 };

private:

	int cash[maxCompanies];
	int netprofit[maxCompanies];
	int profitPerShare[maxCompanies];
	int valuation[maxCompanies];
	int shares[maxCompanies];
	int orphans[maxCompanies];
	int president[maxCompanies];
	bool started[maxCompanies];
	bool sold[maxCompanies];
	int order[maxCompanies];
	bool recievership[maxCompanies];
	bool cities[maxCompanies][maxCities];
	bool coals[maxCompanies][maxCoal];
	int trains[maxCompanies][maxTrains];
	int techLevel;
};

