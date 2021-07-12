#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>

using namespace std;

class Set
{
public:
    void makePOSET();
    void makeHasse();

    friend ofstream & operator<<(ofstream & out, const Set & s);

    int numOfRelations;
    vector<string> relations;
    vector<string> hasseRelations;
    string elements;
private:
    void removeReflexives();
    void removeTransitives();
    int  findRelation(char firstElement, char secondElement);
    int  findHasseRelation(char firstElement, char secondElement);

    void makeReflexive();
    void makeAntisymmetric();
    void makeTransitive();

    bool isReflexive();
    bool isSymmetric();
    bool isAntisymmetric();
    bool isTransitive();
    bool isRelation     (char firstElement, char secondElement);
    bool isHasseRelation(char firstElement, char secondElement);
};

void readFile(string filename, vector<Set> & set);
void writeFile(string filename, vector<Set> & set);
int  getNOS  (string filename); // returns the number of set in the given file

int main()
{
    srand(time(NULL));
    // initializing i-o file names
    string inputFile  = "input.txt";
    string outputFile = "output.txt";

    // initializing set size
    int numofSet = getNOS(inputFile);
    vector<Set> set(numofSet);

    // reading file
    readFile(inputFile, set);
    // turning sets into poset and then hasse diagrams
    for(int i=0; i<numofSet; i++)
    {
        set[i].makePOSET();
        set[i].makeHasse();
    }
    // writing final sets into the output file
    writeFile(outputFile, set);

    return 0;
}

void readFile(string filename, vector<Set> & set)
{
    ifstream in;
    in.open(filename);

    int size = set.size(); // total number of set
    int nor;               // number of relations
    string relation;       // temp relation holder
    string elements;       // temp elements holder

    for(int i=0; i<size; i++)
    {
        // getting number of relation
        in >> nor;
        // inserting it into set
        set[i].numOfRelations = nor;
        
        // getting elements
        getline(in, elements); // get \n
        getline(in, elements);
        // inserting them into set
        set[i].elements = elements;

        for(int j=0; j<nor; j++)
        {
            //getting relation
            getline(in, relation);
            //inserting it into set
            set[i].relations.push_back(relation);
        }
    }

    in.close();
}

void writeFile(string filename, vector<Set> & set)
{
    // opening file with given name
    ofstream out;
    out.open(filename);
    // initializing set size
    const int size = set.size();

    // writing every set to the file
    for(int i=0; i<size; i++)
        out << set[i];

    out.close();
}

int getNOS(string filename)
{
    ifstream in;
    in.open(filename);

    string temp;
    int numOfSet = 0;

    while(getline(in, temp))
        if(temp.size() == 1 || temp.size() == 2)
            numOfSet++;

    in.close();

    return numOfSet;
}

ofstream & operator<<(ofstream & out, const Set & s)
{

    out << "n" << endl;
    out << "POSET" << ": ("<< s.relations[0][0] << s.relations[0][1] << ' ' << s.relations[0][2] << ")";
    for(int i=1; i<s.numOfRelations; i++)
        out << ", ("<< s.relations[i][0] << s.relations[i][1] << ' ' << s.relations[i][2] << ")";

    out << endl;

    for(int i=0; i<s.hasseRelations.size(); i++)
        out << s.hasseRelations[i] << endl;

    return out;
}

bool Set::isReflexive()
{
    const int elementNumber = (elements.size()/2)+1 ; // for ex: a,b,c have 5 chars but only have 3 letters. 5/2 + 1 = 3
    int count = 0;
    // relation elements
    int iofe  = 0; // index of first element
    int iose  = 2; // index of second element

    for(int i=0; i<numOfRelations; i++)
        if(relations[i][iofe] == relations[i][iose])
            count++;

    if(count == elementNumber)
        return true;
    else
        return false;
}

bool Set::isSymmetric()
{
    bool flag;
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element

    for(int i=0; i<numOfRelations; i++)
    {
        flag = false;
        for(int j=0; j<numOfRelations; j++)
            if(relations[i][iofe] == relations[j][iose] && relations[j][iofe] == relations[i][iose])
                flag = true;

        if(flag == false)
            return false;
    }

    return true;
}

bool Set::isAntisymmetric()
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element

    // returns false if there are two relations (a,b) and (b,a) which a!=b
    for(int i=0; i<numOfRelations; i++)
        for(int j=0; j<numOfRelations; j++)
            if(relations[i][iofe] == relations[j][iose] && relations[j][iofe] == relations[i][iose] && relations[i][iofe] != relations[i][iose])
                return false;

    return true;
}

bool Set::isTransitive()
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element
    // holds relation elements for validity check
    char firstElement;
    char secondElement;

    for(int i=0; i<numOfRelations; i++)
    {
        for(int j=0; j<numOfRelations; j++)
            if(relations[i][iose] == relations[j][iofe])
            {
                // for ex: (a,b) and (b,c) ; firstElement = a , secondElement = c 
                firstElement = relations[i][iofe];
                secondElement = relations[j][iose];
                // if there is no relation like (a,c) for relation (a,b) and (b,c) then returns false
                if(isRelation(firstElement, secondElement) == false)
                    return false;
            }
    }
    return true;
}

bool Set::isRelation(char firstElement, char secondElement)
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element
    
    // if there is a relation (firstElement, secondElement) returns true 
    for(int i=0; i<numOfRelations; i++)
        if(relations[i][iofe] == firstElement && relations[i][iose] == secondElement)
            return true;

    return false;
}

bool Set::isHasseRelation(char firstElement, char secondElement)
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element
    
    // if there is a relation (firstElement, secondElement) returns true 
    for(int i=0; i<hasseRelations.size(); i++)
        if(hasseRelations[i][iofe] == firstElement && hasseRelations[i][iose] == secondElement)
            return true;

    return false;
}

void Set::makePOSET()
{
    // first we execute every relation handler once before checking
    makeReflexive();
    makeAntisymmetric();
    makeTransitive();

    // loops turns until every condition is hold
    int count = 0;
    while(count != 3)
    {
        if(isReflexive() == false)
            makeReflexive();
        else
            count++;

        if(isAntisymmetric() == false)
            makeAntisymmetric();
        else
            count++;

        if(isTransitive() == false)
            makeTransitive();
        else
            count++;
    }

}

void Set::makeHasse()
{
    // getting size from relations
    int size = relations.size();
    // resizing hasse relations with that size
    hasseRelations.resize(size);

    // inserting every relations to hasseRelations
    for(int i=0; i<size; i++)
        hasseRelations[i] = relations[i];


    // removing reflexive relations from hasseRelations
    removeReflexives();
    // removing transitive relations from hasseRelations
    removeTransitives();
}

void Set::makeReflexive()
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element
    // number of elements
    int noe = (elements.size()/2)+1; // for ex:(a,b,c) have 5 char and 3 elements , (5/2) + 1 = 3
    // creating a char array for elements
    char * tmpElements = new char[noe];

    // inserting elements to the char array
    for(int i=0, j=0; i<noe; i++, j+=2)
        tmpElements[i] = elements[j];

    // holds relation elements temporarly
    string firstElement;
    string secondElement;
    string relation;
    
    // check every element x for relation (x,x), if there is no relation like that, then creates one.
    for(int i=0; i<noe; i++)
    {
        firstElement  = tmpElements[i]; 
        secondElement = tmpElements[i];

        if(isRelation(firstElement[0], secondElement[0]) == false)
        {
            // creating string for relation x,x
            relation.clear();
            relation.append(firstElement);    
            relation.append(",");
            relation.append(secondElement);    
            // inserting relation x,x into the relations
            relations.push_back(relation);
            // increasing number of relation by one
            numOfRelations++;
        }    
    }

    // deleting dynamically allocated char array
    delete []tmpElements;
}

void Set::makeAntisymmetric()
{
    // holds index of chosen relation that will be removed
    int index;
    // holds either 0 or 1. for random removal process
    int random = 0;
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element

    // holds relation elements for validity check
    char firstElement;
    char secondElement;
    // holds the index of relation which we will remove
    auto iterator = relations.begin();
    // works same is isAntisymmetric() function but instead of returnin false this time removes the first relation.
    for(int i=0; i<numOfRelations; i++)
        for(int j=0; j<numOfRelations; j++)
            if(relations[i][iofe] == relations[j][iose] && relations[j][iofe] == relations[i][iose] && relations[i][iofe] != relations[i][iose])
            {
                // choosing relation which will be removed randomly
                random = rand() % 2;
                if(random == 0)
                {
                    firstElement  = relations[i][iofe];
                    secondElement = relations[i][iose];
                }
                else if(random == 1)
                {
                    firstElement  = relations[j][iofe];
                    secondElement = relations[j][iose];
                }
                
                //finding index of chosen relation
                index = findRelation(firstElement, secondElement);
                //increasing iterator of relation vector as much as index
                for(int i=0; i<index; i++)
                    iterator++;

                //removing that relation
                relations.erase(iterator);

                //decreasing number of relations, i by one since we removed one relation
                numOfRelations--;
                i--;
                
                //initializing iterator again for next removal process
                iterator = relations.begin();

                //breaking loop since loop will check this loop again (i decreased by one)
                break;
            }    
}

void Set::makeTransitive()
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element

    // holds relation elements for validity check
    string firstElement;
    string secondElement;
    string relation;

    for(int i=0; i<numOfRelations; i++)
        for(int j=0; j<numOfRelations; j++)
            if(relations[i][iose] == relations[j][iofe]) // checks for relations for ex: (x,y) and (m,n), if y and m are equal
            {
                // for ex: (a,b) and (b,c) ; firstElement = a , secondElement = c 
                firstElement = relations[i][iofe];
                secondElement = relations[j][iose];
                
                // if there is no relation like (a,c) for relation (a,b) and (b,c) then we create relation (a,c)
                if(isRelation(firstElement[0], secondElement[0]) == false)
                {
                    // creating string for relation x,x
                    relation.clear();
                    relation.append(firstElement);    
                    relation.append(",");
                    relation.append(secondElement);    
                    // inserting relation x,x into the relations
                    relations.push_back(relation);
                    // increasing number of relation by one
                    numOfRelations++;
                }  
            }
    
}

void Set::removeReflexives()
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element
    
    // an iterator to keep track of relation which will be removed
    auto iterator = hasseRelations.begin();

    //checks every relation
    for(int i=0; i<hasseRelations.size(); i++, iterator++)
        if(hasseRelations[i][iofe] == hasseRelations[i][iose])
        {
            // if any (x,x) is found, remove it
            hasseRelations.erase(iterator);
            // decrease index operators by one 
            i--;
            iterator--;
        }
}

void Set::removeTransitives()
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element
    
    // holds relation elements for validity check
    string firstElement;
    string secondElement;

    // an iterator and int index to keep track of relation which will be removed
    auto iterator = hasseRelations.begin();
    int index = 0;

    for(int i=0; i<relations.size(); i++)
        for(int j=0; j<relations.size(); j++)
        {
            // for ex: (a,b) and (b,c) ; firstElement = a , secondElement = c 
            firstElement = relations[i];
            secondElement = relations[j];
                
            // checks for relations for ex: (x,y) and (m,n), if y and m are equal (also x!=y and m!=n)
            if(firstElement[iose] == secondElement[iofe] && firstElement[iofe] != firstElement[iose] && secondElement[iofe] != secondElement[iose]) 
            {
                // if there is a relation like (a,c) for relation (a,b) and (b,c) then we remove relation (a,c)
                if(isHasseRelation(firstElement[iofe], secondElement[iose]) == true)
                {
                    // finding index and iteration of relation we will remove
                    index = findHasseRelation(firstElement[iofe], secondElement[iose]);
                    for(int m=0; m<index; m++)
                        iterator++;
                        
                    // removing that relation
                    hasseRelations.erase(iterator);
                    
                    // initializing iterator again 
                    iterator = hasseRelations.begin();
                }
            }
        }
}

int Set::findRelation(char firstElement, char secondElement)
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element

    // finding relation with given elements then returning that relation's index
    for(int i=0; i<relations.size(); i++)
        if(relations[i][iofe] == firstElement && relations[i][iose] == secondElement) 
            return i;

    return -1;
}

int Set::findHasseRelation(char firstElement, char secondElement)
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 2; // index of second element

    // finding relation with given elements then returning that relation's index
    for(int i=0; i<hasseRelations.size(); i++)
        if(hasseRelations[i][iofe] == firstElement && hasseRelations[i][iose] == secondElement) 
            return i;

    return -1;
}   