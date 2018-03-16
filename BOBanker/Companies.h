#pragma once


class Companies
{
public:
	
	Companies(int setup);

	int getCash(int company);
    int setCash(int company, int cash);
	int changeCash(int company, int value);

	int getPresident(int company);
    int setPresident(int company, int player);

	int getNetprofit(int company);
    int setNetprofit(int company, int netprofit);

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

    int getNext(int order);
    void getInfo(int company);

	bool getSold(int company);
    int setSold(int company);
    int clearSold();
	int clearSold(int company);

	int getValuation(int company);
    int setValuation(int company, int value);
    int increaseValuation(int company);
	int decreaseValuation(int company);

	bool getAvailable(int company, int techLevel);

	int getOrder(int company);
	int setOrder(int company, int turn);
	int getTurnorders(int companyTurnorder[]);
	int setTurnorders();
    int getorderCompany(int oldTurnorder[]);
    //Precondition: The current turnorders of each companies are stored in turnorder.
    //Postcondition: This values are stored in oldTurnorder with the first company index
    // in 0, the second in 1, ...
    int setTurnorder(int company, int direction);


	int getTrainForSale();
	//Precondition: None
	//Postcondition: Returns the cost of the train that is currently for sale
	int getCompanyTrains(int companyVal);
	// Precondition: index of company purchasing the train - postive integer
	// Postcondition: Prints list of trains a company owns
	int scrapTrainOfCompany(int indexVal, int companyVal);
	//Precondition: Index of train that company owns and wants to scrap - number between 1 and 30.
	//Postcondition: Returns the scrap value of the train if no error. Returns
	//0 if there is and error. Also removes train from company's ownership.
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
	int buyTrainForSale(int company, int & techLevel);
	int getTechLevel(int currentTechLevel);

	const char name[maxCompanies][35] = { "Baltimore & Ohio", "Boston & Maine", "Chesapeake & Ohio", "Illinois Central", "Erie", "New York Central","Nickel Plate", "New York New Haven & Hartford", "Pennsylvania", "Wabash" };
	const int available[maxCompanies] = { 1,1,1,3,3,1,3,1,1,3 };


private:

	int cash[maxCompanies];
	int netprofit[maxCompanies];
	int valuation[maxCompanies];
	int shares[maxCompanies];
	int orphans[maxCompanies];
	int president[maxCompanies];
	bool started[maxCompanies];
	bool sold[maxCompanies];
	int order[maxCompanies];
	bool recievership[maxCompanies];
	bool cities[maxCompanies][maxCities];
	int trains[maxCompanies][maxTrains];


};

