//-----------------------------------------------------------------------------
// CSCI2270 Course Project
//
// Identification: HashOpenAddressing.cpp
// Defines Open Addressing table member functions
//-----------------------------------------------------------------------------

#include "HashOpenAddressing.h"
using std::cout;

/*
*HashOpenAddressing - constructs open addressing hash table
*
*@param int size
*@return void
*/
HashOpenAddressing::HashOpenAddressing(int size)
{
    this->hashTableSize = size; 
    hashTable = new Course*[size]; //array of Course pointers made (hash table)
    for(int i = 0; i < size; i++){
        hashTable[i] = nullptr; //pointers set to null at every index
    }
}

/*
*~HashOpenAddressing - frees memory allocated by HashOpenAddressing member functions
*
*@param void
*@return void
*/
HashOpenAddressing::~HashOpenAddressing()
{
    for(int i = 0; i < hashTableSize; i++){
        delete hashTable[i]; //deletes every index
        hashTable[i] = nullptr;
    }
    delete [] hashTable; //deletes table
}

/*
*hash - returns hash of courseNumber
*
*@param int courseNumber
*@return int hashed courseNumber
*/
int HashOpenAddressing::hash(int courseNumber)
{
    return (courseNumber % hashTableSize);
}

/*
*collisionResolutionOpen - resolves collision using quadratic probing method
*
*@params int index, Course pointer tempCourse, int tableSize, hashtable, vector<int> results
*@return vector<int> results
*/
vector<int> collisionResolutionOpen(int index, Course* tempCourse, int tableSize, Course ** table, vector<int> results){
    int i = 0;
    int search = 1; int collision = 1;
    bool inserted = false;
    int newIndex; //int initialized outside of loop, defined in loop
    while(!inserted){
        newIndex = (index + i*i) % tableSize;  //use circular array method for this as to not leave array bounds (this has to have crazy runtime)
        search++;
        if(table[newIndex] == nullptr){ //checks inital index 
            table[newIndex] = tempCourse;
            inserted = true;
        }
        i++;
        if(i >= 46340){ //i*i > INT_MAX (this caused so many issues)
            while(!inserted){
                newIndex = newIndex + 1; //since i*i is too big and causing undefined behavior, first open slot is filled (caused memory leaks)
                if(newIndex >= tableSize){
                    newIndex = 0;
                }
                search++; //increases search counter for every non-open bucket found
                if(table[newIndex] == nullptr){
                table[newIndex] = tempCourse; //inserts in first found open bucket
                inserted = true;
                }
            }
        }
    }
    results[0] = results[0] + search;
    results[1] = results[1] + collision;
    return results; //adds new search and collision count to totals and returns
}


/*
*bulkInsert - inserts courses into hash table, inserts professors into professor binary search tree
*
*@params string fileName
*@return void
*/
void HashOpenAddressing::bulkInsert(string filename)
{
    ifstream istream = ifstream(filename); //initializes variables used in loop
    int lineCount = 0;
    string line = "";
    string temp = "";
    vector<int> results = {0,0}; //not sure why I did not use an array here, would be more resource efficient 
    int year,courseNum;
    string dep,courseName,profId,first,full; //end initializes variables used in loop

    while(getline(istream,line)){ //loops over file, counts number of commas per line to assign values
        if(lineCount == 0){
            lineCount++;
            continue;
        }
        stringstream ss(line); 
        string field;
        int counter = 0;
        while(getline(ss, field, ',')) { //uses stringstream to fill variables, delimited by comma (accidentally used two loops)
            if (counter == 0)
                year = stoi(field);
            else if (counter == 1)
                dep = field;
            else if (counter == 2)
                courseNum = stoi(field);
            else if (counter == 3)
                courseName = field;
            else if(counter == 4){
                profId = field;
            }
            else if(counter == 5)
                first = field;
            else{
                full = first + " " + field;
            }
            counter++;
        }
        Course *tempCourse = new Course(year,dep,courseNum,courseName,nullptr); //makes new course object
        tempCourse->previous = nullptr; tempCourse->next = nullptr; //open addressing version, set new and prev to null
        int index = hash(tempCourse->courseNum);
        if(hashTable[index] != nullptr){ //checks if first index is empty or not, resolves collision if not, inserts if is
            results = collisionResolutionOpen(index,tempCourse,hashTableSize,hashTable,results);
        }
        else{
            hashTable[index] = tempCourse;
        }
        lineCount++;
        Professor *tempProf = nullptr;
        tempProf = profDb.searchProfessor(profId); //sets tempProf to professor object, returns null if not found (could have been done on one line)
        if(tempProf == nullptr){
            profDb.addProfessor(profId,full); //adds prof to ProfBST if not found
            tempProf = profDb.searchProfessor(profId); //sets tempProf to object
        }
        if(tempProf){
            tempProf->coursesTaught.push_back(tempCourse); 
        }
        tempCourse->prof = tempProf; //sets course->professor 
    }
    cout << "========[OPEN ADDRESSING] Hash table complete========\nCollisions using open addressing: " << results[1] <<endl;
    cout << "Search operation used " << results[0] << " times." <<endl;
}

/*
*search-searches for course, displays info if found, displays error if not 
*
*@params int year, int courseNum, string profID
*@return void
*/
void HashOpenAddressing::search(int newCourseYear, int newCourseNumber, string newProfId)
{
    bool found = false;
    int index = hash(newCourseNumber); //checks initial index
    int newIndex;
    Course *tempCourse = hashTable[index];
    int search = 0;
    //checks if course at index is desired course
    if(tempCourse->prof->profId == newProfId && tempCourse->year == newCourseYear && tempCourse->courseNum == newCourseNumber){
        cout << "-----------------------------" <<endl;
        cout << "Course found in " << search << " searches." <<endl;
        displayCourseInfo(tempCourse); //displays info
        found = true;
    }
    else{
        for(int i = 0; i < hashTableSize; i++){ //checks other indexes using quadratic probing
            search++;
            newIndex = (index + i*i) % hashTableSize; //calcs new index
            if(hashTable[newIndex]->prof->profId == newProfId && hashTable[newIndex]->year == newCourseYear && hashTable[newIndex]->courseNum == newCourseNumber){
                cout << "-----------------------------" <<endl;
                cout << "Course found in " << search << " searches." <<endl;
                displayCourseInfo(hashTable[newIndex]);
                found = true;
                break; //multiple numbers can be % to same index, so once found once, break is used
            }
        }
    }
    if(!found){ //prints if course is not found
        cout << "Course not found :(" <<endl;
        return;
    }
}

/*
*displayAllCourses - displays every course in hash table
*
*@params void
*@return void
*/
void HashOpenAddressing::displayAllCourses()
{
    for(int i = 0; i < hashTableSize; i++){ //i iterates over table, displays course info
        Course *tempCourse = hashTable[i];
        displayCourseInfo(tempCourse);
    }
}

/*
*displayCourseInfo - displays info about passed course parameter
*
*@params Course pointer c
*@return void
*/
void HashOpenAddressing::displayCourseInfo(Course* c)
{
	if(c == nullptr){ //if course is null, print error
        cout << "Courses not found (NULL VALUE)" <<endl;
        return;
    }
    else{
        cout << "Hash Collision Method: Open Addressing" <<endl; //prints course info and method used if valid course
        cout << "Course year: " << c->year <<endl;
        cout << "Course number: " << c->courseNum <<endl;
        cout << "Course name: " << c->courseName <<endl;
        cout << "Professor's name: " << c->prof->profName <<endl;
        cout << "--------------------------------" <<endl;
        return;
    }
}