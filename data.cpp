/***************************************************************************
//    Programmer: Paul Juneau        CSC100 Programming Assignment 7
//    Date: 07/15/2013		     Course number: CSC100AB (Section: 15768)
//    Description: Reads a text file, creates an array containing one structured
//    element for each unique word found in the file, as well as the number of
//    times that word appears in the file regardless of case. This array is used
//    to calculate and display statistics.
//    Input:  User enters name of file.txt that they want processed.
//    Output: Struct array is generated containing 500 unique words.
//    Repeated words are incremented. Words are printed out in alphabetical
//    order and numbered. The number of time the words show up are displayed.
//    Statistics are displayed after.
****************************************************************************/
#include<iostream>
#include<iomanip>
#include<string>
#include<cstring>
#include<cctype>
#include<fstream>

using namespace std;

//struct to hold word and index the amout of times the word is encountered in file.
struct WordStruct
{
	char term[21];
	int index;
};

//function prototypes
void setString(char []);
void extractWord (char [], WordStruct []);
void sortStruct (WordStruct [], int);
int findSmallestLoc (WordStruct [], int, int);
int findLargestLoc (WordStruct[], int , int);
void sortIndex (WordStruct [], int);

int main()
{
	int const MAXWORDS = 500;
	WordStruct structArray [MAXWORDS];
	int count = 0;

	char inputName[60];
	char outputName[60];

	int const MAXWORD = 81;
	char buffer[MAXWORD];						     //unformatted lines

	int ch = 0; //counter for each line read in file
	int n; //length of char array in inputName
	int pos; //defines what position '.' is in inputName char array

	bool error = false;

	int wordsRead = 0;
	double avgWordLength = 0;

    cout << "Program title: data.cpp " << endl << endl;
	cout << "Enter filename ";
	cin >> inputName;

	n = strlen(inputName);
	for (int i = 0; i < n; i++)
	{
		if (inputName[i] == '.')
			pos = i;
	}

	strncpy(outputName, inputName, pos);
	strcat(outputName, ".out");

	cout << "Creating " << outputName << endl << endl;

	ifstream infile;
	infile.open(inputName);

    ofstream outfile;
    outfile.open(outputName);

    cout << "Processing lines and extracting words to struct array" << endl << endl;

	while ((ch = infile.peek()) != EOF) //goes until end of file
	{
	    wordsRead++;
	    infile >> buffer; // >> delimits spaces
	    setString(buffer); //removes punctuation and makes lower case

	    int pos = 0;

		while (pos < count && strcmp(buffer, structArray[pos].term) != 0)
			pos++;

        //the if statement says that the word is unique and increments the count. It also adds the word to the struct array.
		if (pos == count && count < MAXWORDS)
        {

			strcpy(structArray[pos].term,buffer); //these two lines add word to struct array, sets the count of that word to one.
			structArray[pos].index = 1;

			count++; //Keeps track of number of valid words in array

        }
		else if (pos < count)
        {

			structArray[pos].index++;
		}
		else if (!error)
        {
            cout << "There are more than 500 UNIQUE words." << endl;
            error = true;
        }
	} // end while loop

    cout << "Words have been extracted and are now being processed." << endl << endl;
	infile.close (); //close input file b/c done processing words


    sortStruct(structArray, count); //alphabetizes and numbers words 1 to 500.

	outfile << "  words in             number of" << endl;
    outfile << " sorted order        occurrences" << endl;
    outfile << "---------------------------------" << endl;

		for(int i = 0; i < count; i++)
		{
			outfile << right << setw(3)  << i+1 << ") ";
			outfile << left  << setw(20) <<  structArray[i].term <<structArray[i].index;
			outfile << endl;
		}
		outfile << endl;


    //determines average word length by making a running sum and then dividing it by the count.
    for (int i = 0; i< count; i++)
    {
        avgWordLength += strlen(structArray[i].term);
    }
    avgWordLength /= count;

    cout << "Word statistics are being generated." << endl;

    //Print out statistics
	outfile << "Statistics:" << endl;
    outfile << "\tTotal number of word read: " << wordsRead << endl;
    outfile << "\tTotal number of unique word read: " << count << endl;
    outfile << "\tAverage length of a word: " << avgWordLength << endl;
    outfile << "\tAverage occurance of a word: " << wordsRead/((float)count) << endl;

    sortIndex(structArray, count); //Determines what words occur the most and sorts from most to least.

    //Display the four most commonly occurning words.
    outfile << "\tMost commonly occurring word(s): " << endl;
    outfile << "\t\t" << structArray[0].term << "\t" <<structArray[0].index << endl;
    outfile << "\t\t" << structArray[1].term << "\t" <<structArray[1].index << endl;
    outfile << "\t\t" << structArray[2].term << "\t" <<structArray[2].index << endl;
    outfile << "\t\t" << structArray[3].term << "\t" <<structArray[3].index << endl;

    outfile.close(); //close the output file.

    cout << "Done. File " << outputName << " has been prepared." << endl;

	return 0;
} // end main

 void setString(char buffer[]) //alters buffer by removing punctuation and decapitalizing
	{
		char str2[81]; //dummy word C-string
		int pos2 = 0; //initialize subscript position in str2
		int length;

		length = strlen(buffer);
		for (int pos = 0; pos < length; pos++) //itterate until end of buffer array
		{
			if(isalpha(buffer[pos])) //if it's a letter, lower case it and send it to str2, then increment position of str2.
			{
				str2[pos2] = tolower(buffer[pos]);
				pos2++;
			}
			else if(isspace(buffer[pos])) //if it's a space, add it to str2 and then increment position of str2.
			{
				str2[pos2] = buffer[pos];
				pos2++;
			}
			else if(ispunct(buffer[pos]))
			{
				if (isalpha(buffer[pos-1]) && isalpha(buffer[pos+1])) //if the punctuation is surrounded by letters, then send it to str2 and increment str2.
				{
					str2[pos2] = buffer[pos];
					pos2++;
				}
				else //if the punctuation is not surrounded by letters, copy nothing over.
					str2[pos2] = str2[pos2];
			}
		}
		str2[pos2] = '\0'; // to make str2 a valid c-string
		strncpy(buffer, str2, sizeof(str2)); //strncpy was the problem before
	}


int findSmallestLoc (WordStruct structArray [], int start, int stop) //finds least word value and returns its position
{
    int loc = start;
    for (int pos = start + 1; pos <= stop; pos++)
        if (strcmp(structArray[pos].term, structArray[loc].term) < 0)
            loc = pos;

    return loc;
}

// Sorts terms alphabetically
void sortStruct (WordStruct structArray [], int count)
{
    WordStruct temp; //creates dummy struct to allow swapping to occur
    int position;

    for (int loop = 0; loop < count; loop++) {
        position = findSmallestLoc (structArray, loop, count - 1);

        if (position != loop)
        {
            temp = structArray[position];
            structArray[position] = structArray[loop];
            structArray[loop] = temp;
        }
    }
}

int findLargestLoc (WordStruct structArray [], int start, int stop) //finds index with highest value in struct array
{
    int loc = start;
    for (int pos = start + 1; pos <= stop; pos++)
        if (structArray[pos].index > structArray[loc].index)
            loc = pos;

    return loc;
}

void sortIndex (WordStruct structArray [], int count) //sorts struct array to have index value decreasing
{
    WordStruct temp;
    int position;

    for (int loop = 0; loop < count; loop++) {
        position = findLargestLoc (structArray, loop, count - 1);

        if (position != loop)
        {
            temp = structArray[position];
            structArray[position] = structArray[loop];
            structArray[loop] = temp;
        }
    }
}
