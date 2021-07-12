#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// class for each individual set in input.txt
class Set
{
public:
    // relation functions
    void is_reflexive();
    void is_symmetric();
    void is_antisymmetric();
    void is_transitive();
    bool is_relation(string e1, string e2);

    // setter and getter for numOfRelations
    inline void set_numOfRelations(int number) { numOfRelations = number; }
    inline int  get_numOfRelations() const     { return numOfRelations;   }
    
    // setter and getter for elements
    inline void   set_elements(string element)   { elements.push_back(element); }
    inline string get_element (int index) const  { return elements[index];      }

    // getter for explanations
    inline string get_reflexive()     const { return reflexive; }
    inline string get_symmetric()     const { return symmetric; }
    inline string get_antisymmetric() const { return antisymmetric; }
    inline string get_transitive()    const { return transitive; }

    vector<vector<string>> relations; // ex: A = {1,2,3} ; relations[0] = {1,1}, relations[1] = {2,2}, relations[2] = {3,3}  
private:
    // these 4 string variables hold explanation part 
    string reflexive;
    string symmetric;
    string antisymmetric;
    string transitive;

    int numOfRelations;               // ex: A = {1,2,3} ; numOfRelations = n^2 = 3^2 = 9
    vector<string> elements;          // ex: A = {1,2,3} ; elements[0] = '1', elements[1] = '2', elements[2] = '3'
};

void read_input      (string filename, vector<Set> & sets);
void write_output    (string filename, vector<Set> & sets);
void rm_commas       (vector<string> & elements);
void print_relations (Set & s);

int main()  
{
    // initializing set object vector
    vector<Set> sets;

    // reading input file and filling every set object
    read_input("input.txt", sets);
    write_output("output.txt", sets);

    return 0;
}


// readds the given file and fills every individual set. 
void read_input(string filename, vector<Set> & sets)
{
    int    number; 
    int    index_element = 0;
    char   check_endl;
    string temp;
    vector<string> elements(1);
    vector<vector<string>> relations;

    // opening given file
    ifstream in;
    in.open(filename);

    // gets into the loop until EOF
    while( getline(in, temp) )
    {
        // breaks the loop if taken string from the txt is empty.
        if(temp.size() == 0)
            break;

        // first create a set
        sets.push_back(Set());

        // getting number of relations
        number = stoi(temp);
        // putting the relation number to the class object
        sets[index_element].set_numOfRelations(number);
        
        // getting set elements from input.txt
        getline(in, elements[0]);
        // removing commas between elements
        rm_commas(elements);

        // inserting elements to the class object
        for(int i=0; i<elements.size(); i++)
            sets[index_element].set_elements(elements[i]);

        // initializing relations array
        relations.resize(number);
        for(int i=0; i<number; i++)
            relations[i].resize(2);

        // initializing class relations array
        sets[index_element].relations.resize(number);
        for(int i=0; i<number; i++)
            sets[index_element].relations[i].resize(2);


        // gettin relations
        for(int i=0; i<number; i++)
        {
            getline(in, relations[i][0]);
            // removing commas between elements
            rm_commas(relations[i]);
            // inserting relations to class object
            sets[index_element].relations[i][0] = relations[i][0];
            sets[index_element].relations[i][1] = relations[i][1];
        }

        // incrementing index for Set class object 
        index_element++;
    }
    
    in.close();

    return;
}

// creates a file named filename. and writes every set and their relations.
void write_output(string filename, vector<Set> & sets)
{   
    int    numOfSet = sets.size(); // holds number of set
    string element;  // holds elements of an individual set
    bool   flag;     // will change for every relation check

    // opening a file with given name
    ofstream out;
    out.open(filename);

    // writing part
    for(int i=0; i<numOfSet; i++)
    {
        out << "n" << endl; // indicator for new set

        // writing relations to the file
        for(int j=0; j<sets[i].get_numOfRelations(); j++)
            out << '(' << sets[i].relations[j][0] << ',' << sets[i].relations[j][1] << ')';
        out << endl;

        //writing reflexivity
        sets[i].is_reflexive();
        out << sets[i].get_reflexive();

        //writing symmetry
        sets[i].is_symmetric();
        out << sets[i].get_symmetric();

        //writing antisymmetry
        sets[i].is_antisymmetric();
        out << sets[i].get_antisymmetric();

        //writing transivity
        sets[i].is_transitive();
        out << sets[i].get_transitive();
    }

    out.close();
}

// removes comma in the given element string and creates a string vector for every single element on the set
void rm_commas(vector<string> & elements)
{
    string temp = elements[0]; 

    // for ex: 1,2,3 has a 5 character if we divide it to 2; 5/2 = 2. if we add 1 to it we get 3, which is total element number of the set. 
    int size = (temp.size()/2) + 1; 

    // resizing element string vector before inserting individual elements
    elements[0].clear();
    elements.resize(size);

    // inserting elements to the string vector
    for(int i=0, j=0; i<temp.size(); i++)
        if(temp[i] != ',')
        {
            elements[j] = temp[i];
            j++;
        }

    return;    
}

// yes string is appended when number of (a,a) type relations is equal to total element number of the set
void Set::is_reflexive()
{
    int count = 0;

    // increases count by 1 every time it founds a (a,a) type relation
    for(int i=0; i<elements.size(); i++)
    {
        for(int j=0; j<numOfRelations; j++)
            if(elements[i] == relations[j][0] && elements[i] == relations[j][1])
                count++;
        
        //if count didn't increased then no explanation is appended to reflexive string
        if(count != i+1) 
        {
            reflexive.clear();
            reflexive.append("Reflexive: No. Because the relation (");
            reflexive.append(get_element(i));
            reflexive.append(",");
            reflexive.append(get_element(i));
            reflexive.append(") is missing.\n");
            return;
        }
        

    }   
    //if function reachs here, that means every element has a relation with itself. so we append this to the reflexive string
    reflexive.clear();
    reflexive.append("Reflexive: Yes. Every element has a relation with itself\n");
    return;
}

// for every relation checks to every other relation to find its symmetric one. 
// if it can't find even one, then appends 'No' explanation to the symmetric string.
void Set::is_symmetric()
{
    bool flag;

    for(int i=0; i<numOfRelations; i++)
    {
        flag = false;

        // flag assigned to true if there is a relation (a,b) and symmetric to this relation (b,a)
        for(int j=0; j<numOfRelations; j++)
            if(relations[i][0] == relations[j][1] && relations[i][1] == relations[j][0])
                flag = true;

        // writing explanation of why this relation set is not symmetric
        if(flag == false)
        {
            symmetric.clear();
            symmetric.append("Symmetric: No. Because it is missing relation (");
            symmetric.append(relations[i][1]);
            symmetric.append(",");
            symmetric.append(relations[i][0]);
            symmetric.append(") for (");
            symmetric.append(relations[i][0]);
            symmetric.append(",");
            symmetric.append(relations[i][1]);
            symmetric.append(").\n");
            return;
        }
    }
    //if function reachs here, that means condition holds for every relation.
    symmetric.clear();
    symmetric.append("Symmetric: Yes. Because every relation has a symmetric one\n");
    return;;
}

// checks every relation and if it finds a relation like (a,b) and (b,a), then appends 'No' explanation to antisymmetric string.
// it will search until it looks to every relation. if it doesn't find any relation like that, then appends 'Yes' explanation to antisymmetric string
void Set::is_antisymmetric()
{    
    for(int i=0; i<numOfRelations; i++)
        for(int j=0; j<numOfRelations; j++)
        {
            if(relations[i][0] == relations[j][1] && relations[i][1] == relations[j][0] && relations[i][0] != relations[i][1])
            {
                antisymmetric.clear();
                antisymmetric.append("Antisymmetric: No. Because there are two relations (");
                antisymmetric.append(relations[i][0]);
                antisymmetric.append(",");
                antisymmetric.append(relations[i][1]);
                antisymmetric.append(") - (");
                antisymmetric.append(relations[i][1]);
                antisymmetric.append(",");
                antisymmetric.append(relations[i][0]);
                antisymmetric.append(") and ");
                antisymmetric.append(relations[i][0]);
                antisymmetric.append("!=");
                antisymmetric.append(relations[i][1]);
                antisymmetric.append("\n");
                return;
            }

            // if function reachs here, that means it hasn't found any rule breaking relation. so we append 'yes' explanation to the string
            else if(relations[i][0] != relations[i][1])
            {
                antisymmetric.clear();
                antisymmetric.append("Antisymmetric: Yes. Because for the relation (");
                antisymmetric.append(relations[i][0]);
                antisymmetric.append(",");
                antisymmetric.append(relations[i][1]);
                antisymmetric.append("), (");
                antisymmetric.append(relations[i][1]);
                antisymmetric.append(",");
                antisymmetric.append(relations[i][0]);
                antisymmetric.append(") is not found\n");
            }
            else if(antisymmetric.size() < 1)
            {
                antisymmetric.clear();
                antisymmetric.append("Antisymmetric: Yes. Because there are two relations (");
                antisymmetric.append(relations[i][0]);
                antisymmetric.append(",");
                antisymmetric.append(relations[i][1]);
                antisymmetric.append(") - (");
                antisymmetric.append(relations[i][1]);
                antisymmetric.append(",");
                antisymmetric.append(relations[i][0]);
                antisymmetric.append(") and ");
                antisymmetric.append(relations[i][0]);
                antisymmetric.append("==");
                antisymmetric.append(relations[i][1]);
                antisymmetric.append("\n");
            }
            
        }

    return;
}

// for example we have relations (a,b) and (b,a).
// when function looks into first relation it will search for something starts with 'b'. 
// in this example it is (b,a). after function find this relation it will search for a relation (a,a)
// because of transitivity rule. if it finds one then its ok, it will jump into next relation. 
// but if it can't find then func returns false.
void Set::is_transitive()
{
    // relation elements
    int iofe = 0; // index of first element    
    int iose = 1; // index of second element
    // holds relation elements for validity check
    string firstElement;
    string secondElement;

    for(int i=0; i<numOfRelations; i++)
    {
        for(int j=0; j<numOfRelations; j++)
        {
            if(relations[i][iose] == relations[j][iofe])
            {
                // for ex: (a,b) and (b,c) ; firstElement = a , secondElement = c 
                firstElement = relations[i][iofe];
                secondElement = relations[j][iose];
                // if there is no relation like (a,c) for relation (a,b) and (b,c) then returns false
                if(is_relation(firstElement, secondElement) == false)
                {
                    transitive.clear();
                    transitive.append("Transivite: No. Because for the relations (");
                    transitive.append(relations[i][0]);
                    transitive.append(",");
                    transitive.append(relations[i][1]);
                    transitive.append(") and (");
                    transitive.append(relations[i][1]);
                    transitive.append(",");
                    transitive.append(secondElement);
                    transitive.append("), the relation (");
                    transitive.append(firstElement);
                    transitive.append(",");
                    transitive.append(secondElement);
                    transitive.append(") is not found.\n");
                    return; 
                }
                else if(is_relation(firstElement, secondElement) == true)
                {
                    transitive.clear();
                    transitive.append("Transitive: Yes. Because for the relations (");
                    transitive.append(relations[i][0]);
                    transitive.append(",");
                    transitive.append(relations[i][1]);
                    transitive.append(") and (");
                    transitive.append(relations[j][0]);
                    transitive.append(",");
                    transitive.append(relations[j][1]);
                    transitive.append("), the relation (");
                    transitive.append(firstElement);
                    transitive.append(",");
                    transitive.append(secondElement);
                    transitive.append(") is found.\n");
                    continue;
                }
            }
            // 
            else if(transitive.size() == 0)
            {
                transitive.clear();
                transitive.append("Transivite: Yes. Because for the relation (");
                transitive.append(relations[i][0]);
                transitive.append(",");
                transitive.append(relations[i][1]);
                transitive.append(") there are no relation starting with '");
                transitive.append(relations[i][1]);
                transitive.append("'.\n");
            }
        }
    }
    return;
    /*
    bool flag, flag2;
    string e1, e2; // for relation validty check

    for(int i=0; i<numOfRelations; i++)
    {
        flag = false;
        flag2 = true;
        for(int j=0; j<numOfRelations; j++)
        {
            if( ( relations[i][1] == relations[j][0] ) )
            {
                e1 = relations[i][0]; // if relation is (a,b) then e1 is 'a'
                e2 = relations[j][1]; // if relation is (b,c) then e2 is 'c'
                flag  = is_relation(e1, e2);
                flag2 = is_relation(e1, e2);  // checks if there is any relation like (a,c) according to this example
            }

            // if there is a relation like (a,c) we don't need to check for validty anymore and we can append 'yes' explanetion to the transitive string 
            if(flag == true)
            {
                transitive.clear();
                transitive.append("Transitive: Yes. Because for the relations (");
                transitive.append(relations[i][0]);
                transitive.append(",");
                transitive.append(relations[i][1]);
                transitive.append(") and (");
                transitive.append(relations[j][0]);
                transitive.append(",");
                transitive.append(relations[j][1]);
                transitive.append("), the relation (");
                transitive.append(e1);
                transitive.append(",");
                transitive.append(e2);
                transitive.append(") is found.\n");
                break;
            }

        }

        //if function reachs here, that means condition does not hold and we can append 'no' explanation to the transitive string.
        if(flag2 == false)
        {
            transitive.clear();
            transitive.append("Transivite: No. Because for the relations (");
            transitive.append(relations[i][0]);
            transitive.append(",");
            transitive.append(relations[i][1]);
            transitive.append(") and (");
            transitive.append(relations[i][1]);
            transitive.append(",");
            transitive.append(e2);
            transitive.append("), the relation (");
            transitive.append(e1);
            transitive.append(",");
            transitive.append(e2);
            transitive.append(") is not found.\n");
        }
        // extra explanation for 'yes'
        else if(flag == false && flag2 == true)
        {
            transitive.clear();
            transitive.append("Transivite: Yes. Because for the relation (");
            transitive.append(relations[i][0]);
            transitive.append(",");
            transitive.append(relations[i][1]);
            transitive.append(") there are no relation starting with '");
            transitive.append(relations[i][1]);
            transitive.append("'.\n");
        }
    }
    return;
    */
}

// if there is no relation with (e1, e2) in the relations vector then returns false
bool Set::is_relation(string e1, string e2)
{
    for(int i=0; i<numOfRelations; i++)
        if(relations[i][0] == e1 && relations[i][1] == e2)
            return true;
    
    return false;
}
