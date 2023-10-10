//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: HashChaining.cpp
//Defines Chain table member functions
//-----------------------------------------------------------------------------

#include "HashChaining.h"
using std::cout;

/*
*HashChaining - constructs chaining hash table
*
*@params int size
*@return void
*/
HashChaining::HashChaining(int size)
{
    this->hashTableSize = size;
    hashTable = new Course*[size];
    for(int i = 0; i < size; i++){
        hashTable[i] = nullptr; 
    }
}

/*
*~HashChaining - deletes memory allocated by hash chaining member functions
*
*@param void
*@return void
*/
HashChaining::~HashChaining()
{
    Course *listTraverser = nullptr; //iterates over linked list
    Course *nextCourse = nullptr; //keeps reference to next course
    if(hashTableSize > 0){
        for(int i = 0; i < hashTableSize; i++){ //iterates over hash table
            listTraverser = hashTable[i];
                while(listTraverser){
                    nextCourse = listTraverser->next;
                    delete listTraverser; //deletes course, keeping reference to next
                    listTraverser = nextCourse;
            }
        }
        delete [] hashTable; //deletes array
    }
}

/*
*hash - returns hash of courseNumber
*
*@param courseNumber
*@return int
*/
int HashChaining::hash(int courseNumber)
{
    return (courseNumber % hashTableSize);
}

/*
*collisionResolutionChain - resolves collision using chaining 
*
*@params int index, Course pointer tempCourse, int tableSize, hashtable , vector<int> results
*@return vector<int> results
*/
vector<int> collisionResolutionChain(int index, Course *tempCourse, int tableSize, Course ** table, vector<int> results){
    int search = 1; int collision = 1; 
    Course *headCourse = table[index];
    while(headCourse->next){ //since chaining, course will be added to end of linked list
        headCourse = headCourse->next;
        search++; //search only incremented for amount of linked list elements
    }
    headCourse->next = tempCourse;
    tempCourse->previous = headCourse;
    search++;
    results[0] = results[0] + search;
    results[1] = results[1] + collision;
    return results; //sums and returns new search and collision values
}

/*
*bulkInsert - inserts courses into hash table, inserts professors into professor binary search tree
*
*@params string fileName
*@return void
*/
void HashChaining::bulkInsert(string filename)
{
    ifstream istream = ifstream(filename); //initializes variables used in loop
    int lineCount = 0;
    string line;
    string temp = "";
    vector<int> results = {0,0};
    int year,courseNum;
    string dep,courseName,profId,first,full; //end initializes variables used in loop
    while(getline(istream,line)){ //loops over file, counts number of commas per line to assign values
        if(lineCount == 0){
            lineCount++;
            continue;
        }
        stringstream stream(line);
        string holder;
        int counter = 0;
        while(getline(stream, holder, ',')) { //uses stringstream to fill variables, delimited by comma
            if (counter == 0)
                year = stoi(holder);
            else if (counter == 1)
                dep = holder;
            else if (counter == 2)
                courseNum = stoi(holder);
            else if (counter == 3)
                courseName = holder;
            else if(counter == 4){
                profId = holder;
            }
            else if(counter == 5)
                first = holder;
            else{
                full = first + " " + holder;
            }
            counter++;
        }
        Professor *tempProf = profDb.searchProfessor(profId); //looks for professor (returns null if not found)
        Course *tempCourse = new Course(year,dep,courseNum,courseName,tempProf); //makes new course object
        int index = hash(tempCourse->courseNum);
        if(hashTable[index] != nullptr){ //checks if first index is empty or not, resolves collision if not, inserts if is
            results = collisionResolutionChain(index,tempCourse,hashTableSize,hashTable,results);
            tempCourse->next = nullptr;
        }
        else{
            hashTable[index] = tempCourse;
            tempCourse->previous = nullptr;
            tempCourse->next = nullptr;
        }
        lineCount++;
        tempProf = profDb.searchProfessor(profId); //adds prof to BST or adds course to Prof's vector
        if(tempProf == nullptr){
            profDb.addProfessor(profId,full);
            tempProf = profDb.searchProfessor(profId);
        }
        if(tempProf){
            tempProf->coursesTaught.push_back(tempCourse);
            tempCourse->prof = tempProf;
        }
    } //different method used for this to compare (both end up working)
    cout << "========[CHAINING] Hash table complete========\nCollisions using open addressing: " << results[1] <<endl;
    cout << "Search operation used " << results[0] << " times." <<endl;
}

/*
*search-searches for course, displays info if found, displays error if not 
*
*@params int year, int courseNum, string profID
*@return void
*/
void HashChaining::search(int courseYear, int courseNumber, string  newProfId)
{
    int index = hash(courseNumber);
    Course *tempCourse = hashTable[index]; //Only have to check this index because chaining
    if(tempCourse == nullptr){ //empty index means failed search
        cout << "Also not found in chaining :(" <<endl;
        return;
    }
    int search = 1;
    //checks if course at index is desired course
    if((tempCourse->prof->profId == newProfId) && (tempCourse->year == courseYear) && (tempCourse->courseNum == courseNumber)){
        cout << "Course found in " << search << " searches." <<endl;
        displayCourseInfo(tempCourse); //displays info
        return;
    }
    else{
        Course *iter = tempCourse; //iterates over list 
        while(iter != nullptr){
            search++;
            if(iter->year == courseYear && iter->courseNum == courseNumber && iter->prof->profId == newProfId){
            cout << "Course found in " << search << " searches." <<endl;
            displayCourseInfo(iter); //displays info
            return;
            }
            if(iter->next != nullptr){ //if not found and next exists, iterate to next
                iter = iter->next;
            }
            else{
                cout << "Also not found in chaining :(" <<endl;
            }
        }
    }
}

/*
*displayAllCourses - displays every course in hash table
*
*@params void
*@return void
*/
void HashChaining::displayAllCourses()
{
    bool dupe;
    for(int i = 0; i < hashTableSize; i++){
        Course *tempCourse = hashTable[i];
        dupe = false;
        if(tempCourse){
            displayCourseInfo(tempCourse);
            while(tempCourse->next){
                dupe = true;
                tempCourse = tempCourse->next;
                displayCourseInfo(tempCourse);
            }
            if(dupe == true){
                displayCourseInfo(tempCourse);
            }
        }
    }
}

/*
*displayCourseInfo - displays info about passed course parameter
*
*@params Course pointer c
*@return void
*/
void HashChaining::displayCourseInfo(Course* c)
{
	if(c == nullptr){ //if course is null, print error
        cout << "Course not found (NULL VALUE)!" <<endl;
        return;
    }
    else{
        cout << "Hash Collision Method: Chaining" <<endl; //prints course info and method used if valid course
        cout << "Course year: " << c->year <<endl;
        cout << "Course number: " << c->courseNum <<endl;
        cout << "Course name: " << c->courseName <<endl; 
        cout << "Professor's name: " << c->prof->profName <<endl;
        cout << "--------------------------------" <<endl;
        return;
    }
}