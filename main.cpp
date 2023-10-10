//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: main.cpp
// Main menu and function calls
//-----------------------------------------------------------------------------

#include "HashOpenAddressing.h"
#include "HashChaining.h"

#include <iostream>
#include <sstream>
#include <string>

using std::cout;

int main (int argc, char* argv[])
{
    if(argc != 3){ //checks for incorrect command line execution
        cout << "Invalid input! \n Valid: ./<program name> <csv file> <hash table size>";
        return -2;
    }
    int tableSize;
    try{ //checks that tableSize is an int greater than 0
        tableSize = stoi(argv[2]);
    if (tableSize <= 0) {throw -1;}
    }
    catch(...){
        cerr << "Invalid table size! Table size must be positive INT"<<endl;
        return -1;
    }
    string fileName = argv[1];
    HashOpenAddressing hashOpen(tableSize); //creates open addressing and chaining hash tables
    HashChaining hashChain(tableSize);
    string inputString = "";
    int input = 0;
    bool filled = false;
    while(input){ //loops menu
        cout << "==== Main Menu ==== \n1. Populate Hash Table (Only call this once!) \n2. Search for Course \n3. Search for Professor \n4. Display all Courses \n5. Exit" <<endl;
        getline(cin,inputString); //getline used for every input, standardized input stream
        try{input = stoi(inputString);} //handles stoi exceptions
        catch(...){
            cerr << "Input not INT"<<endl;
            return 1;
        }
        switch(input){ //switches input 
            case 1:
            {
                if(filled){ //checks that hash tables can only be filled once to prevent infinite loop
                    cout << "Hash table already filled!"<<endl;
                    break;
                }
                hashOpen.bulkInsert(fileName); //inserts into both hash tables
                hashChain.bulkInsert(fileName);
                filled = true;
                break;
            }
            case 2:
            {
                if(!filled){ //makes sure hash table is filled before any other functions are called (in every case)
                    cout << "WARNING: Hash table unfilled, please fill hash table." <<endl;
                    break;
                }
                int year, courseID; //vars to hold passed parameters for search
                string holder, profID;
                cout << "Enter course year: ";
                getline(cin,holder);
                try{year = stoi(holder);} //handles stoi exceptions
                catch(...){
                    cerr << "Input not INT"<<endl;
                    return 1;
                }
                cout << "Enter course number: ";
                getline(cin,holder);
                try{courseID = stoi(holder);}
                catch(...){
                    cerr << "Input not INT"<<endl;
                    return 1;
                }
                cout << "Enter professor ID: ";
                getline(cin,profID);
                hashOpen.search(year,courseID,profID); //searches both hash tables
                hashChain.search(year,courseID,profID);
                break;
            }
            case 3:
            {
                if(!filled){
                    cout << "WARNING: Hash table unfilled, please fill hash table." <<endl;
                    break;
                }
                cout << "Enter professor ID to search for: ";
                string profID;
                getline(cin,profID);
                hashOpen.profDb.publicSearchProfessor(profID); //searches both BST's
                hashChain.profDb.publicSearchProfessor(profID);
                break; 
            }
            case 4:
            {
                if(!filled){
                    cout << "WARNING: Hash table unfilled, please fill hash table." <<endl;
                    break;
                }
                cout << "Which hash table would you like to display?(O=Open Addressing, C=Chaining)" <<endl;
                string input;
                getline(cin,input); 
                if(input == "O"){ //displays all courses from hash table of user's choice
                    hashOpen.displayAllCourses();
                }
                else if(input == "C"){
                    hashChain.displayAllCourses();
                }
                else{
                    cout << "Invalid input" <<endl;
                }
                break;
            }
            case 5:
            {
                cout << "Are you sure? y/n" <<endl;
                getline(cin,inputString);
                if(inputString == "y"){ //exits program if user confirms
                    return 0;
                }
                else{
                    cout << "Invalid input or n entered, returning to program..."<<endl;
                    input = 0;
                    break;
                }
            }
            default:
            {
                cout << "Enter valid input (1-5)"; //default if user enters int not between 1-5
                break;
            }
        }
        
    }
    return 0;
}

//You guys have been great and made this course amazing <3