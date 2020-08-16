## Welcome to Hunter Krech's ePortfolio
This ePortfolio is a compilation of a self-assessment, code review, artifacts of work, and narratives.

### Self-Assessment


### Farkle Dice Game

This is a c++ console game that uses Software design, engineering, Algorithms and a custom data structure.

```markdown
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
```
### Narrative
The first artifact is an example of the multiplayer dice game Farkle that uses random numbers to roll dice and score a player's roll. It was created in my last term in my advanced C++  programming and data structures class for my final. I selected the item because it was created totally from scratch with no existing code and was methodically planned out with pseudocode that discussed the best options for every piece of functionality. I believe this showcases that I can plan well to systematically solve problems as they arise as I did when developing this artifact originally. This artifact was improved by some original errors I missed. For example, when examining the code below only one of these statements can be evaluated to true.
 
Even though only one can be true were still checking all 3 of them when we don't need to or should be.
```markdown
if (isPlaying[playerIdNumber] == 1)//if player already is playing
{
	playerScores[playerIdNumber] += CountScore(diceRoll);
	cout << CountScore(diceRoll) << " Points added to your score giving you a total of " << playerScores[playerIdNumber] << " Points" << endl;
}
if (isPlaying[playerIdNumber] == false && CountScore(diceRoll) >= 1000)//hadn't scored over 100 but did this turn
{
	cout << "Congrats you scored at least 1000 points so you score is now being recorded!" << endl;
	playerScores[playerIdNumber] += CountScore(diceRoll);
	cout << CountScore(diceRoll) << " Points added to your score giving you a total of " << playerScores[playerIdNumber] << " Points" << endl;

}
if
{
	cout << "You havn't scored over 1000 Points in a single turn yet therefore your score hasn't been recorded" << endl;
}
```
I improved this to the following.
```markdown
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
```
Here we only test parameters until on is true then stop testing the rest. Also if the first two don’t apply, we have an else to catch the other case. Another problem I hadn’t previously anticipated was the possibility of breaking the application if incorrect input was entered. For example, in the screenshot below we can encase the whole user input in a do-while that checks the input. If cin.fail() evaluates to true or the number entered is less then two we ask the user for input again.
 
The only other adjustment I made was to the function DiceRoll where I condensed this function with the AdditionalRoll function. In the game Farkle you can choose to keep the point you have earned or roll for a double or nothing. There is no need for an additional function for this and honestly just complicates the program because I need to pass the old dice that were scored as well as the dice that were re-rolling and just isn't needed in this game. I strategically made a lot of functions for different functionality that way it would be easy to follow what was going on, but this was overkill. 

### Database Handler

This is a database handler written in Java for a android application.
```markdown
package hunterkrech.cs360.campsiteloacator;

import android.content.ContentValues;
import android.content.Context;
import android.database.ContentObservable;
import android.database.Cursor;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteDatabase;

public class DogDBHandler extends SQLiteOpenHelper{
    // database name and version
    private static final int DB_VER = 1;
    private static final String DB_NAME = "dogDB.db";
    // table
    public static final String TABLE_DOGS = "dogs";
    // columns
    public static final String COLUMN_ID = "id";
    public static final String COLUMN_NAME = "name";
    public static final String COLUMN_AGE = "age";
    // constructor
    public DogDBHandler(Context context, String name,
                        SQLiteDatabase.CursorFactory factory, int version)
    {
        super(context, DB_NAME, factory, DB_VER);
    }
    // This method creates the Dogs table when the DB is initialized.
    @Override
    public void onCreate(SQLiteDatabase db) {
        String CREATE_DOGS_TABLE = "CREATE TABLE " +
                TABLE_DOGS + "(" +
                COLUMN_ID + " INTEGER PRIMARY KEY," +
                COLUMN_NAME + " TEXT," +
                COLUMN_AGE + " INTEGER" + ")";
        db.execSQL(CREATE_DOGS_TABLE);
    }
    // This method closes an open DB if a new one is created.
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_DOGS);
        onCreate(db);
    }
    // This method is used to add a Dog record to the database.
    public void addDog(Dog dog) {
        ContentValues values = new ContentValues();
        values.put(COLUMN_NAME, dog.getName());
        values.put(COLUMN_AGE, dog.getAge());
        SQLiteDatabase db = this.getWritableDatabase();
        db.insert(TABLE_DOGS, null, values);
        db.close();
    }
    // implements the search/find functionality
    public Dog searchDog(String dogName) {
        String query = "SELECT * FROM " +
                TABLE_DOGS + " WHERE " + COLUMN_NAME +
                " = \"" + dogName + "\"";
        SQLiteDatabase db = this.getWritableDatabase();
        Cursor cursor = db.rawQuery(query, null);
        Dog dog = new Dog();
        if (cursor.moveToFirst()) {
            cursor.moveToFirst();
            dog.setID(Integer.parseInt(cursor.getString(0)));
            dog.setName(cursor.getString(1));
            dog.setAge(Integer.parseInt(cursor.getString(2)));
            cursor.close();
        } else {
            dog = null;
        }
        db.close();
        return dog;
    }
    // implements the delete dog functionality
    public boolean deleteDog(String dogName) {
        boolean result = false;
        String query = "SELECT * FROM " + TABLE_DOGS +
                " WHERE " + COLUMN_NAME + " = \"" + dogName + "\"";
        SQLiteDatabase db = this.getWritableDatabase();
        Cursor cursor = db.rawQuery(query, null);
        Dog dog = new Dog();
        if (cursor.moveToFirst()) {
            dog.setID(Integer.parseInt(cursor.getString(0)));
            db.delete(TABLE_DOGS, COLUMN_ID + " = ?",
                    new String[] { String.valueOf(dog.getID())});
            cursor.close();
            result = true;
        }
        db.close();
        return result;
    }
}
```
For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).


