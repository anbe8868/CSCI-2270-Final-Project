//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: ProfBST.cpp
// Defines Professor Binary Search Tree member functions
//-----------------------------------------------------------------------------

#include "ProfBST.h"
#include <iostream>

using std::cout;

/*
*ProfBST - constructs professor object Binary Search Tree
*
*@param courseNumber
*@return int
*/
ProfBST::ProfBST()
{
    root = nullptr;
}

/*
*destroy node (helper function) - recursively deletes BST 
*
*@param Professor pointer current
*@return void
*/
void destroyNode(Professor *current)
{
    if (current != nullptr)
    {
        destroyNode(current->left);
        destroyNode(current->right);
        if(current != nullptr)
        {
            delete current;
        }
        current = nullptr;
    }
}

/*
*~ProfBST - destructor called to destroy BST and delete professor allocated memory
*
*@param void
*@return void
*/
ProfBST::~ProfBST()
{
    destroyNode(root);
}

/*
*addNodeHelper (Helper function) - adds new professor node
*
*@param Professor pointer current, Professor pointer newNode
*@return current professor pointer (will recursively return root)
*/
Professor *addNodeHelper(Professor *current, Professor *newNode)
{
    if (current == nullptr) //if node is empty, adds 
    {
        return newNode;
    }
    if (current->profId > newNode->profId) //if less, check left (alphabetical by profID)
    {
        current->left = addNodeHelper(current->left, newNode);
    }
    else if (current->profId < newNode->profId) //if more, check right (alphabetical by profID)
    {
        current->right = addNodeHelper(current->right, newNode);
    }

    return current;
}

/*
*addProfessor - Calls helper to add new professor node
*
*@param string profId, string ProfName
*@return void
*/
void ProfBST::addProfessor(string profId, string profName)
{
    Professor *newProf = new Professor(profId,profName); //makes new professor object
    root = addNodeHelper(root, newProf); //inserts
}

/*
*searchHelper (Helper function) - returns node if found, returns null if not
*
*@param string profId, Professor pointer ptr
*@return Professor node pointer if found, null if not
*/
Professor* searchHelper(string profId, Professor* ptr){
    if(ptr == nullptr || ptr->profId == profId){ //returns reference if found or leaf node is found (search failed)
        return ptr;
    }
    if(ptr->profId < profId){ //checks right tree
        return searchHelper(profId, ptr->right);
    }
    return searchHelper(profId,ptr->left); //checks left tree
}

/*
*searchProfessor - calls helper to search, returns node
*
*@param string profId
*@return Professor pointer or null
*/
Professor* ProfBST::searchProfessor(string profId)
{
    return searchHelper(profId,root); //returns pointer to professor searched or null if not found
}

/*
*publicSearchProfessor - prints professor's courses taught
*
*@param string profId
*@return void
*/
void ProfBST::publicSearchProfessor(string profId) 
{
    Professor *prof = searchProfessor(profId); //gets reference to professor object
    if(!prof){
        cout << "Professor not found, please try again." <<endl;
    }
    else{
        displayProfessorInfo(prof); //displays professor info
    }
}

/*
*displayProfessorInfo - prints professor name, id, and courses taught
*
*@param string profId
*@return void
*/
void ProfBST::displayProfessorInfo(Professor* p)
{
    if(p == nullptr){
        cout << "Error displaying info (null pointer)" <<endl; //error if null pointer passed
    }
    cout << "Professor name: " << p->profName <<endl;
    cout << "Professor ID: " << p->profId << endl;
    cout << "Courses taught(" << p->coursesTaught.size() << "): " <<endl;
        for(int i = 0; i < int(p->coursesTaught.size());i++){ //iterates over course vector, displays course department, number, and name
            cout << p->coursesTaught[i]->department << p->coursesTaught[i]->courseNum << " " << p->coursesTaught[i]->courseName << " " << p->coursesTaught[i]->year <<endl;
        }
    cout << "------------------------"<<endl;
}
