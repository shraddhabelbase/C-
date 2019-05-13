#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>

#define PAGEWIDTH 90;
#define PAGEHEIGHT 70;
int LINESUSED = 0;

using namespace std;
struct Mailing {

    char firstName[13], lastName[13], address[21], city[21], state[3], zip[6];
    int data;
    int status;

    Mailing *next;
};
class List {
public:
    List();
    bool NodeExists(char[], char[]);

    void InsertNode(Mailing);

    void ChangeNode(char[], char[], int, char[]);
    void PrintList(ofstream &);
    void PrintListHead(ofstream &);
    void RecordError(char, char[], char[]);
    void PrintErrorLog(ofstream &);
    void ClearErrorLog();
    Mailing DeleteNode (char[], char[]);
private:
    Mailing *listStart;
    string errorLog;
    int errorCount;
};

List::List() {
    listStart = NULL;
}
bool List::NodeExists(char fname[], char lname[]) {

    Mailing *PreviousPtr, *CurrentPtr;
    PreviousPtr = NULL;
    CurrentPtr = listStart;

    while ( (CurrentPtr != NULL) &&
           !(( strcmp(CurrentPtr->lastName,lname)==0)&&(strcmp(CurrentPtr->firstName,fname)==0))
           ) {
        PreviousPtr = CurrentPtr;
        CurrentPtr = CurrentPtr -> next;
    }
    return (CurrentPtr != NULL);
}
void List::InsertNode(Mailing Person) {

    bool alphaTest = true;

    Mailing *newNode, *PreviousPtr, *CurrentPtr;
    newNode = new (Mailing);
    if ( newNode != NULL) {
        strcpy(newNode->firstName, Person.firstName);
        strcpy(newNode->lastName, Person.lastName);
        strcpy(newNode->address, Person.address);
        strcpy(newNode->city, Person.city);
        strcpy(newNode->state, Person.state);
        strcpy(newNode->zip, Person.zip);
        PreviousPtr = NULL;
        CurrentPtr = listStart;

        if (CurrentPtr == NULL) {
            newNode -> next = NULL;
            listStart = newNode;
        }
        else {

            while ( (CurrentPtr != NULL) && alphaTest )
            {

                if (strcmp(CurrentPtr->lastName, newNode->lastName) > 0) {
                    alphaTest = false;
                }

                else if ( (strcmp(CurrentPtr->lastName, newNode->lastName) == 0)
                       && (strcmp(CurrentPtr->firstName, newNode->firstName) > 0) )
                {
                    alphaTest = false;
                }
                else {
                    PreviousPtr = CurrentPtr;
                    CurrentPtr = CurrentPtr -> next;
                }
            }
            if (PreviousPtr == NULL) {
                newNode->next = listStart;
                listStart = newNode;
            }
            else {
                PreviousPtr -> next = newNode;
                newNode -> next  = CurrentPtr;
            }
        }
    }

    else
    {
        cout << "Error! Out of memory!";
    }
    return;
}

// --------------------------------------------------------------------------------------------------
void List::ChangeNode(char fname[], char lname[], int field, char Value[]) {

    Mailing movingNode;

    Mailing *PreviousPtr, *CurrentPtr;
    PreviousPtr = NULL;
    CurrentPtr = listStart;

    while ( (CurrentPtr != NULL)
           && ( strcmp(CurrentPtr->lastName, lname) != 0 )
           && ( strcmp(CurrentPtr->firstName, fname) != 0 ) ) {
        PreviousPtr = CurrentPtr;
        CurrentPtr = CurrentPtr -> next;
    }
    if (CurrentPtr != NULL) {

        switch (field) {
            case 1:
                strcpy(CurrentPtr->firstName, Value);
                movingNode = DeleteNode(Value, lname);
                InsertNode(movingNode);
                break;
            case 2:
                strcpy(CurrentPtr->lastName, Value);
                movingNode = DeleteNode(fname, Value);
                InsertNode(movingNode);
                break;
            case 3:
                strcpy(CurrentPtr->address, Value);
                break;
            case 4:
                strcpy(CurrentPtr->city, Value);
                break;
            case 5:
                strcpy(CurrentPtr->state, Value);
                break;
            case 6:
                strcpy(CurrentPtr->zip, Value);
                break;
            default:
                cout << "Error" << endl;
                break;
        }
    }
    else {
        cout << "Error" << endl;
    }
    return;
}

Mailing List::DeleteNode (char fname[], char lname[]) {



    Mailing *tempPtr, *PreviousPtr, *CurrentPtr;

    if ( (lname  == listStart->lastName) && (fname  == listStart->firstName) )
    {
        tempPtr = listStart;
        listStart = listStart -> next;

        return *tempPtr;
    }
    else
    {
        PreviousPtr = listStart;
        CurrentPtr = listStart -> next;

        while ( (CurrentPtr != NULL)
               && ( strcmp(CurrentPtr->lastName, lname) != 0 )
               && ( strcmp(CurrentPtr->firstName, fname) != 0 ) )
        {
            PreviousPtr = CurrentPtr;
            CurrentPtr = CurrentPtr -> next;
        }
        if  (CurrentPtr != NULL)
        {
            tempPtr = CurrentPtr;
            PreviousPtr->next = CurrentPtr->next;
            tempPtr->next = NULL;

            return *tempPtr;
        }
    }
    tempPtr->status = -1;
    return *tempPtr;
}

void List::PrintList(ofstream &outfile) {


    Mailing  *CurrentPtr;
    CurrentPtr = listStart;
    if ( CurrentPtr  == NULL )
    {
        outfile  << "The list is empty." << endl;
        return;
    }
    while ( CurrentPtr != NULL )
    {
        outfile << CurrentPtr->lastName << " ";
        outfile << CurrentPtr->firstName << " ";
        outfile << CurrentPtr->address << "  ";
        outfile << CurrentPtr->city << "  ";
        outfile << CurrentPtr->state << "     ";
        outfile << CurrentPtr->zip;
        outfile << endl;
        LINESUSED++;
        CurrentPtr = CurrentPtr->next;
    }
    outfile << endl;
    LINESUSED++;
    return;
}

// --------------------------------------------------------------------------------------------------
void List::PrintListHead(ofstream &outfile) {
    outfile << "                            ";
    outfile << "MAILING LIST" << endl;
    outfile << "Last Name        ";
    outfile << "First Name  ";
    outfile << "Address               ";
    outfile << "City        ";
    outfile << "State ";
    outfile << "Zip Code" << endl;
    outfile << endl;
    return;
}

// --------------------------------------------------------------------------------------------------
void List::RecordError(char mode, char fname[], char lname[]) {
    string newError = "\n";

    switch (mode) {
        case 'A':
            newError+="* ";
            newError+=fname;
            newError+=" ";
            newError+=lname;
            newError+=" is already in the list. \n";
            break;
        case 'C':
            newError+="* Record ";
            newError+=fname;
            newError+=lname;
            newError+=" not found!\n";
            break;
        case 'D':
            newError+="* Record of ";
            newError+=fname;
            newError+=lname;
            newError+=" not found. \n";
            break;
        default:
            cout << "Error: .";
            break;
    }
    errorLog+=newError;

    return;
}

void List::PrintErrorLog(ofstream &outfile) {

    outfile << errorLog;
    return;
}

void List::ClearErrorLog() {

    errorLog = "           ";
    return;
}

//********************************************************************************************

    // Main function prototypes
void Header(ofstream &);
void Footer(ofstream &Outfile);
void Input(ifstream &, ofstream &);

int main() {

    ifstream inputFile("data2.txt", ios::in);
    ofstream outputFile("output.txt", ios::out);

    Input(inputFile, outputFile);

    Footer(outputFile);

    inputFile.close();
    outputFile.close();

    return 0;
}
//****************************************************************************************************

void Input(ifstream &infile, ofstream &outfile)
{

    char    act;                        // action command
    char    fname[13];                  // first name
    char    lname[13];                  // last name
    char    nname[13];                  // new name
    char    address[21];                // address
    char    city[21], state[3], zip[6];
    int     field = 0;                  // field number

    int maxLines = PAGEHEIGHT;

    Mailing *newGuy;
    Mailing tempCustomer;

    List list;

    act = infile.get();
    infile >> ws;

    while (act != 'Q') {
        switch (act) {

            case 'A':
                newGuy = new (Mailing);

                infile >> ws;
                infile.get(fname, 13);
                infile.get(lname, 13);
                infile >> ws;
                infile.get(address, 21);
                infile >> ws;
                infile.get(city, 13);
                infile >> ws;
                infile.get(state, 3);
                infile >> ws;
                infile.get(zip, 6);
                strcpy(newGuy->firstName, fname);
                strcpy(newGuy->lastName, lname);
                strcpy(newGuy->address, address);
                strcpy(newGuy->city, city);
                strcpy(newGuy->state, state);
                strcpy(newGuy->zip, zip);

                if ( !(list.NodeExists(fname, lname)) ) {
                    list.InsertNode(*newGuy);
                }
                else {
                    list.RecordError(act, fname, lname);
                    LINESUSED+=2;
                }
                break;

            case 'D':
                infile >> ws;
                infile.get(fname, 13);
                infile >> ws;
                infile.get(lname, 13);

                if (list.NodeExists(fname, lname)) {
                    tempCustomer = list.DeleteNode(fname, lname);
                }
                else {
                    list.RecordError(act, fname, lname);
                    LINESUSED+=2;
                }
                break;

            case 'C':

                char f;
                infile >> ws;
                infile.get(fname, 13);
                infile >> ws;
                infile.get(lname, 13);

                infile.get(f);
                atoi(&f);
                field = (f - 48);

                if (list.NodeExists(fname, lname)) {
                    switch (field) {

                        case 1:
                        case 2:
                            infile.get(nname, 13);
                            list.ChangeNode(fname, lname, field, nname);
                            break;

                        case 3:
                            infile.get(address, 21);
                            list.ChangeNode(fname, lname, field, address);
                            break;

                        case 4:
                            infile.get(city, 13);
                            list.ChangeNode(fname, lname, field, city);
                            break;

                        case 5:
                            infile.get(state, 3);
                            list.ChangeNode(fname, lname, field, state);
                            break;

                        case 6:
                            infile.get(zip, 6);
                            list.ChangeNode(fname, lname, field, zip);
                            break;

                        default:
                            cout << "Error.";
                            break;

                    }
                }
                else {
                    list.RecordError(act, fname, lname);
                    LINESUSED+=2;
                    switch (field) {
                        case 1:
                        case 2: infile.get(nname, 13);
                            break;
                        case 3: infile.get(address, 21);
                            break;
                        case 4: infile.get(city, 13);
                            break;
                        case 5: infile.get(state, 3);
                            break;
                        case 6: infile.get(zip, 6);
                            break;
                        default:
                            cout << "Error.";
                            break;}}
                break;
            case 'P':

                Header(outfile);
                LINESUSED+=4;
                list.PrintListHead(outfile);
                LINESUSED+=3;
                list.PrintList(outfile);
                list.PrintErrorLog(outfile);
                list.ClearErrorLog();
                for (int i=0; i < (maxLines - LINESUSED); i++) {
                    outfile << endl;
                }
                LINESUSED = 0;
                break;
            default:
                cout << "Error." << endl;
                break;
        }
        infile >> ws;
        act = infile.get();
    }
    return;
}
void Header(ofstream &Outfile)
{
    Outfile << setw(35) << "-----------------------------------";
    Outfile << setw(35) << "-----------------------------------\n\n";

    return;
}

