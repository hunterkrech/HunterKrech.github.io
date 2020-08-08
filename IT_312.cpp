/*
	Hunter Krech
	IT 312
	6/12/2020
	6-3 Final Project Building Block Four: Programming (Reading From a File)
*/

#include <iostream>
#include <fstream>//For file reader
#include <string>//For text reader (getline)
#include <stdlib.h>//For rand
#include <time.h>//For time seed


using namespace std;//Needed for cout/cin

//Function Definitions
void PlayerRoll(int);
void DisplayRules();
int GetPlayers();
string GetPlayerNames(int);
int CountScore(int[]);
void PlayerWon(int);
int ReRoll(int[],int, int, int[]);
void NextPlayersTurn(int);
void ReRollPrompt(int, int[]);
//Global Variables
string *playerNames;//
int *playerScores;
bool* isPlaying;
int totalPlayers;

int main()
{
	
	DisplayRules();//Function to display rules

	totalPlayers = GetPlayers();//Get total player
	playerNames = new string[totalPlayers];//Just a array of player names
	playerScores = new int[totalPlayers];//Hold all the player scores
	isPlaying = new bool[totalPlayers];//This will hold if player has started playing meaning they scored > 1000 already
	//Record all the players names And initialize all scores to zero
	for (int i = 0; i < totalPlayers; i++)
	{
		playerNames[i] = GetPlayerNames(i);
		playerScores[i] = 0;
	}
	PlayerRoll(0);
}
void DisplayRules() 
{
	string currentLine;//This will hold the current line of text
	ifstream rulesFile("Rules.txt");//Define the file
	if (rulesFile.is_open())//Open the file
	{
		while (getline (rulesFile, currentLine))
		{
			cout << currentLine << '\n';//Displays line of file
		}
		rulesFile.close();//Can close the file now
	}
	else 
	{
		cout << "Rules file could not be found (Add it to the destination please)";//Should be fine where it is but just incase to prevent crash
	}
}
int GetPlayers()//This function returns the total amount of players
{
	int playerRetValue = 0;//Value that we will be returning
	cout << "How many players are playing?" << endl;
	cin >> playerRetValue;
	//Make sure entry is valid
	if (cin.fail() || playerRetValue < 2)//Fisrt do an initail chack to make sure it as least an int or there is more then 2 players
	{
		do {
			cin.clear();//Clear the last response
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please try again to enter a number of players greater then 2\n";//Prompt the user for another number of total players
			cin >> playerRetValue;//Store the value they enter
		} while (cin.fail() || playerRetValue < 2);//Again see if it fails or is not an int and greater then 2 else prompt the user again
	}

	return playerRetValue;//Return the number to assign it
}
string GetPlayerNames(int playerNumber)//This function returns the players name(s)
{
		string thisPlayersName;
		cout << "What is player " << (playerNumber + 1) << "'s name?" << endl;//i + 1 because remember arrays start at 0, but player zero is weird. This just prompts the user
		cin >> thisPlayersName;//Save to return this

		return thisPlayersName;//Return it
		
}
void PlayerRoll(int playerIdNumber)//This will handle rolling all 6 dice
{
	
	cout << playerNames[playerIdNumber] << "'s turn" << endl;
	int diceRoll[6];
	srand(time(NULL));//Random seed
	for (int i = 0; i < 6; i++) {
		diceRoll[i] = rand() % 6 + 1;//Random number between 1 and 6
		cout << "Die number " << (i + 1) << " lands on " << diceRoll[i] << endl;//i + 1 because we start at 0
	}
	//Check if they won first
	if ((playerScores[playerIdNumber] + CountScore(diceRoll)) >= 10000)
	{
		PlayerWon(playerIdNumber);
	}
	ReRollPrompt(playerIdNumber, diceRoll);
	
}
int CountScore(int thisRoll[])
{
	int tripples[7] = { 0 };
	int totalPoints = 0;//Keeps tally of toal point this roll
	for (int i = 0; i < 6; i++)
	{
		if (thisRoll[i] == 1)//worth 100 point
			totalPoints += 100;
		else if (thisRoll[i] == 5)//worth
		{
			totalPoints += 50;
		}
		tripples[thisRoll[i]]++;//Count the tripples
	}
	//See if there were any triples
	for (int j = 1; j < 7; j++)
	{
		if (tripples[j] >= 3) {
			totalPoints += (j * 100);//Worth face value * 100
			//if they became triples delete thyeyre old value
			if (j == 5)
			{
				totalPoints -= 150;
			}
			else if (j == 1)
			{
				totalPoints -= 300;
			}

		}

	}
	return totalPoints;
}
void PlayerWon(int playerIdNumber)
{
	cout << "Congrat's " << playerNames[playerIdNumber] << " you won the game!"<<endl;
	//system("Pause");
}
int ReRoll(int diceRoll[], int totalReRolling, int playerIdNumber, int playerDiceToReRoll[])//Re-rolls on the dice the player chose
{

	bool scoringDice = false;
	int tripples[7] = { 0 };
	srand(time(NULL));//Random seed
	for (int i = 0; i < totalReRolling; i++)
	{
		diceRoll[playerDiceToReRoll[i]] = rand() % 6 + 1;//Random number between 1 and 6
		cout << "Die number " << playerDiceToReRoll[i] + 1 << " lands on " << diceRoll[playerDiceToReRoll[i]] << endl;
		tripples[diceRoll[i]]++;
		if (diceRoll[playerDiceToReRoll[i]] == 1)//Scoring dice
		{
			scoringDice = true;
		}
		else if (diceRoll[playerDiceToReRoll[i]] == 5)//Scoring dice
		{
			scoringDice = true;
		}
		for (int j = 1; j < 7; j++)
		{
			if (tripples[j] >= 3) 
			{
				scoringDice = true;
			}

		}
	}
	if (scoringDice == false)
	{
		cout << "Thats a Farkle! You earned no Points!" << endl;
		NextPlayersTurn(playerIdNumber);
	}
	else 
	{
		ReRollPrompt(playerIdNumber, diceRoll);
	}
	return 0;
}
void NextPlayersTurn(int playerIdNumber) //Shuffle to next player
{
	if ((totalPlayers -1) > playerIdNumber) 
	{
		PlayerRoll(playerIdNumber + 1);
	}
	else 
	{
		PlayerRoll(0);
	}
}
void ReRollPrompt(int playerIdNumber, int diceRoll[]) //See's if the player wants a chance to roll again
{
	//Check if they won first
	if ((playerScores[playerIdNumber] + CountScore(diceRoll)) >= 10000)
	{
		PlayerWon(playerIdNumber);
	}

	cout << "You scored " << CountScore(diceRoll) << " Points, would you like to score more points? (Y/N)" << endl;//Prompt the user if they want to continue their turn
	string keepPlaying = "";
	cin >> keepPlaying;
	string moreDice = "";
	int diceToRollAgain = 0;
	int totalDiceRollingAgain = 0;
	int reRollDiceId[6];
	if (keepPlaying == "Y") {//If the player wants to roll more dice
		do {
			int diceToRollAgain;
			cout << "What dice would you like to re-roll? (One at a time please enter the die number)" << endl;
			cin >> diceToRollAgain;
			diceRoll[diceToRollAgain - 1] = 0;
			reRollDiceId[totalDiceRollingAgain] = diceToRollAgain;
			totalDiceRollingAgain++;
			cout << "Anymore dice you want to re-roll?(Y/N)" << endl;
			cin >> moreDice;
		} while (moreDice == "Y");
		ReRoll(diceRoll, totalDiceRollingAgain, playerIdNumber, reRollDiceId);
	}
	else if (keepPlaying == "N")//If the answer is no
	{
		if (isPlaying[playerIdNumber] == 1)//if player already is playing
		{
			playerScores[playerIdNumber] += CountScore(diceRoll);
			cout << CountScore(diceRoll) << " Points added to your score giving you a total of " << playerScores[playerIdNumber] << " Points" << endl;
		}
		else if (isPlaying[playerIdNumber] == false && CountScore(diceRoll) >= 1000)//hadn't scored over 100 but did this turn
		{
			cout << "Congrats you scored at least 1000 points so you score is now being recorded!" << endl;
			playerScores[playerIdNumber] += CountScore(diceRoll);
			cout << CountScore(diceRoll) << " Points added to your score giving you a total of " << playerScores[playerIdNumber] << " Points" << endl;

		}
		else
		{
			cout << "You havn't scored over 1000 Points in a single turn yet therefore your score hasn't been recorded" << endl;

		}
		NextPlayersTurn(playerIdNumber);//Go to the next player
	}
}
