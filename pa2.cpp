//Author: Nathan Lucyk
//Date: 10/14/2012
//Version: 1.0
//Notes: This program is not written in a fashion that would lend itself to using it for something else or expanding upon in a very meaningful way.
//For instance the functions print to the console instead of returning a boolean.
//There is probably a way to increase efficiency by testing for multiple things at the same time instead of splitting them up by functions.
//This was not done for readability and because it was simply easier to focus on functionality instead of efficiency.

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;
//prototypes
static void functionCheck();
static void reflexiveCheck();
static void symetricChecks();
static void transitiveCheck();

int domain;
bool **pairs;//2-d array that holds the data

int main(int argc, char *argv[])
{
    const int NUM_OF_FILES = 2;
    const int TEXT_FILE_INDEX = 1;
    const int START_OF_DOMAIN = 1;
    const int INDEX_MAKER = -1;

    //Check for errors
    if(argc != NUM_OF_FILES)
    {//there should only be 2 files, the executable and the input file.
        cout<<"Error not the correct number of arguments";
        cin.ignore();
        return -1;
    }
    //open the file
    ifstream input;
    input.open(argv[TEXT_FILE_INDEX]);
    if(!input)
    {//check if the file is valid
        cout<<"Error opening file."<<endl;
        cin.ignore();
        return -1;//error
    }
    //get the domain and check it for errors
    domain = 0;
    input>>domain;
    if(domain < START_OF_DOMAIN)
    {
        cout<<"Error. Domain isn't valid."<<endl;
        cin.ignore();
        return -1;//error
    }
    //set up the 2-d array
    pairs = new bool*[domain];
    for(int i = 0; i < domain; i++)
        pairs[i] = new bool[domain];

    //initialize everything to false. There isn't any in yet.
    for(int row = 0; row < domain; row++)
    {
        for(int column = 0; column < domain; column++)
            pairs[row][column] = false;
    }

    //read the info in the file and put it into the struct arrays.
    int x;
    int y;
    int numPairs = 0;
    while(input>>x>>y)
    {//file in format "x y" for every line
        if( x > domain || x < START_OF_DOMAIN || y > domain || y < START_OF_DOMAIN)
        {
            cout<<"Error. Ordered pair is outside the domain."<<endl;
            cin.ignore();
            return -1;//error
        }
        pairs[x+INDEX_MAKER][y+INDEX_MAKER] = true;
        numPairs++;
    }

    if(numPairs > 0)
    {
        //check for what kind of relationship the data has.
        reflexiveCheck();
        symetricChecks();
        transitiveCheck();
        functionCheck();
    }
    else
        cout<<"The set is empty."<<endl<<"Therefore it has no relations and is a function of type neither"<<endl;

    //goodbye message
    cout<<endl<<"Press Enter to Exit"<<endl;
    cin.ignore();

    //de-allocate memory and end program
    for(int i = 0; i < domain; ++i)
        delete [] pairs[i];
    delete [] pairs;
    return 0;
}

//Checks if it is a function and what type of function it is. Displays this info the console.
static void functionCheck()
{//row is the x and column is the y
    bool functionCheck = true;
    bool isBothTypes = true; // the function will only ever be both or neither if it is a function.
    string columnsTaken = "";
    string currentColumn = "";
    int rowCounter;

    for(int row = 0; row < domain; row++)
    {
        rowCounter = 0;
        for(int column = 0; column < domain; column++)
        {
            if(pairs[row][column] == true)
            {
                rowCounter++;
                //Do the concatination in three steps to avoid c_strings messing with it.
                currentColumn = "{";
                currentColumn += column;
                currentColumn +="}";
                if(columnsTaken.find(currentColumn) != string::npos)
                    isBothTypes = false;//curly braces on string to avoid 1 being found in 10 and such.
                else
                    columnsTaken += currentColumn;
            }
        }
        if(rowCounter != 1)
        {// X value goes to multiple Y's
            functionCheck = false;
            cout<<"function: no";
            return;
        }
    }
    cout<<"function: yes"<<endl;
    if(isBothTypes)
        cout<<"type of function: one-to-one correspondence"<<endl;
    else
        cout<<"type of function: neither"<<endl;
}

//Checks if the relations are reflexive. Prints out whether it is to the console.
static void reflexiveCheck()
{
    for(int i=0; i < domain; i++)
    {
        if( pairs[i][i] != true)
        {
            cout<<"reflexive: no"<<endl;
            return;
        }
    }
    cout<<"reflexive: yes";
}

//Checks both anti-symetric and symetric. Prints both outcomes to the console.
static void symetricChecks()
{
    bool symetric = true;
    bool antisymetric = true;

    for(int row = 0; row < domain; row++)
    {
        for(int column = 0; column < domain; column++)
        {
            if(pairs[row][column] == true)
            {
                if( row != column && pairs[column][row] != true)
                    symetric = false;
                else if(row != column)//no need to check the second part.
                    antisymetric = false;
            }
        }
    }
    if(symetric)
        cout<<"symetric: yes"<<endl;
    else
        cout<<"symetric: no"<<endl;
    if(antisymetric)
        cout<<"antisymetric: yes"<<endl;
    else
        cout<<"antisymetric: no"<<endl;
}

//Checks if it is transitive. Displays to the console whether it is or not.
static void transitiveCheck()
{
    bool transitive = false;
    for(int row = 0; row < domain; row++)
    {
        for(int column = 0; column < domain; column++)
        {
            if(pairs[row][column] == true)
            {//if something is a match, go through the column as the row
                for(int columnCheck = 0; columnCheck < domain; columnCheck++)
                {
                    if(pairs[column][columnCheck] == true)
                    {//(B,C) was true so try (A,C)
                        if( pairs[row][columnCheck] != true)
                        {//(A,C) was false so it's not transitive
                            cout<<"transitive: no"<<endl;
                            return;
                        }
                        transitive = true;
                    }
                }
                if(!transitive)
                {//this if will occur if there were no objects in the row of "column"
                    cout<<"transitive: no"<<endl;
                    return;
                }
                transitive = false;
            }
        }
    }
    cout<<"transitive: yes"<<endl;
}
