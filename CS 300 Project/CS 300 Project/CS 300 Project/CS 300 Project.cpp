
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


using namespace std;

//Structure / class for course objects. Holds the course number, the course title, and the course prereqs.
struct Course {
    string courseNum; //Represents the course's course num or ID. Ex. CSCI300
    string courseTitle; //Represents the course's course title. Ex. Introduction to Computer Science
    vector<string> preReqs; //Vector containing prereqs of courses. May be empty or contain a few items.
};

//Structure for node objects which are used to create the binary search tree. Each node has a left and right pointer to represent a tree.
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course courseObj) : Node() {
        this->course = courseObj;
    }
};

//Start of the Binary Search Tree class
class BSTree {

public:
    BSTree();
    void InOrder(); 
    void Insert(Course courseObj); 
    void Search(string courseNum); 
    virtual ~BSTree(); 
    void Destruct(Node* node); 
    Node* root;
    void addNode(Node* node, Course courseObj);
    void inOrder(Node* node);
};

//This is the basic constructor function of the binary search tree object
BSTree::BSTree() {
    root = nullptr; //creates a root node pointing to nothing yet.
}

BSTree::~BSTree() {
    Destruct(root);
}

//Used to delete binary search tree if need be. Is not used currently inside program.
void BSTree::Destruct(Node* node) {
    if (node != nullptr) {
        Destruct(node->left);
        Destruct(node->right);
        delete node;
    }
}

//This function works alongside inOrder to print out the entire binary search tree in order of the nodes. This function calls inOrder to start at the root node of the tree.
void BSTree::InOrder() {
    inOrder(root);
}
//This function prints out the entire binary search tree, node by node. Each course object at each node is printed including the course num and course title.
void BSTree::inOrder(Node* node) {
    //If the node passed into the function is not null, then a recursive call is made to inOrder left, then the data is output and a recursive call to inOrder right is made.
    if (node != nullptr) {
        inOrder(node->left);

        //Out puts the current node's course number and course title.
        cout << node->course.courseNum << ", " << node->course.courseTitle << endl;

        inOrder(node->right);
    }
    return;
}

//This function works alongside the insert function to add a new node to the binary search tree.
void BSTree::addNode(Node* node, Course courseObj) {
    //If the current node's course number is greater than the user entered course number, then the new course object will be added to the left of the current node if said left is nullptr.
    if (node->course.courseNum.compare(courseObj.courseNum) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(courseObj);
        }
        //If the node to the left of the current node is not nullptr, then the current node is set to the left node through a recursive call and the function runs another check.
        else {
            addNode(node->left, courseObj);
        }
    }
    //If the current node's course number is less than the user entered course number, then the new course object will be added to the right of the current node if said right node is nullptr.
    else {
        if (node->right == nullptr) {
            node->right = new Node(courseObj);
        }
        //If the node to the right of the current node is not nullptr, then the current node is set to the right node through a recursive call and the function runs another check.
        else {
            addNode(node->right, courseObj);
        }
    }
}

//This function works alongside the addNode function to add new nodes to the binary search tree. This function calls addNode depending on whether the root is empty or not.
void BSTree::Insert(Course courseObj) {
    //If the root node of the tree is null, then a new node is created for the root node in order to be added into the tree.
    if (root == nullptr) {
        root = new Node(courseObj);
    }
    //Otherwise, the current root node is passed into the addNode function in order to add a node beyond the root node in the correct position.
    else {
        addNode(root, courseObj);
    }
}

//This function searches the binary search tree for the user specified course. If the use specified course is found inside the tree, then the course objects num, title, and prereqs will be printed.
void BSTree::Search(string courseNum) {

    //New node object to represent the current node that will be checked. Starts from the root node in the BSTree.
    Node* currNode = root;

    //While the current node being checked does no equal a null pointer, the loop will run until a match is found or until the end of the tree is reached.
    while (currNode != nullptr) {
        //If the current node being checked matches the user entered course number, then the current node's course object(course num and course title) are o
        if (currNode->course.courseNum.compare(courseNum) == 0) {
            //Outputs the current node's course num and course title
            cout << currNode->course.courseNum << ", " << currNode->course.courseTitle << ", PreReqs: ";

            int i = 0; //Loop variable

            //This loop will run for as long as the size of the prereqs vector for the current node course object.
            for (i = 0; i < currNode->course.preReqs.size(); i++) {
                //Outputs the prereq at the current position in the course objects prereq vector.
                cout << currNode->course.preReqs.at(i);

                //If the current prerreq is not the last element in the course object's preReqs vector, then a comma and space is added to the end.
                if (i != currNode->course.preReqs.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;

            //If there are no prereq elements in the course object's preReqs vector, then no preReqs is printed.
            if (i == 0) {
                cout << "No Prereqs found." << endl;
            }
            return;
        }
        //If the user entered course num is less than the current node's course num, then the current node is reassigned to the current node's left
        if (courseNum.compare(currNode->course.courseNum) < 0) {
            currNode = currNode->left;
        }
        //If the user entered course num is greater than the current node's course num, then the current node is reassigned to the current node's right
        else {
            currNode = currNode->right;
        }
    }
    //If the course object that was searched for does not exist, then this is printed.
    cout << "The course you searched for was not found in the loaded data file." << endl << endl;
}

//This function opens the file that the user inputs as long as the file is found. It then stores the data from the file into course objects which are then placed into a binary search tree.
void Open_Read_Load(string fileToOpen, BSTree* bst) {
    ifstream inFS; //Allows for the file to be opened.
    string line; //Holds the current line in the file.

    //Opens the user entered file.
    inFS.open(fileToOpen);

    //If the file opens properly, then this branch will execute. The lines of the file are read, parsed, and stored inside course objects, which are then inserted into the BSTree.
    if (inFS.is_open()) {
        int num; //Represents what part of the file data is being looked at. Ex. 0 and 1 represent the courseNum and courseTitle respectively.
        string holder; //Used to hold the current line up until the first comma found.

        //This loop will run while there are still lines for the getline command to get. Basically, the loop runs until the end of the file.
        while (getline(inFS, line)) {
            num = 0;
            Node* node = new Node(); //New node object.
            stringstream str(line); //Allows the code to read data from the string line as if it were a stream like cin. str command gets/sets the data in the string object.
            //The stringstream basically allows for the line to be separated into substrings.

            //This loop will run until the line has reached the third part of the line, the preReqs, or until there are no more parts of the line to read.
            while (num < 2) {
                getline(str, holder, ','); //Gets the current line up until the first comma.
                //If num equals 0 or the first part of the line (the courseNum), then the new node object's courseNum is assigned to holder, which holds the first part before the first comma from the current file line.
                if (num == 0) {
                    node->course.courseNum = holder;
                }
                //Otherwise, the new node's courseTitle is assigned to holder. (Basically, when this branch is reached, the loop is reading the courseTitle from the file into the new node.)
                else {
                    node->course.courseTitle = holder;
                }
                num++; //Increments the num variable so it gets the data for the next part of the line past the most recent comma.
            }
            //This loop runs until there are no more preReqs to read into the new node.
            while (getline(str, holder, ',')) {
                //Reads the current preReq up until the most recent comma into the node's course preReqs vector.
                node->course.preReqs.push_back(holder);
            }

            Course newCourse; //Creates a new course object to hold the data on the current line.
            newCourse.courseNum = node->course.courseNum; //Sets the new course object's courseNum to the correct data from the file.
            newCourse.courseTitle = node->course.courseTitle; //Sets the new course object's courseTitle to the correct data from the file.
            newCourse.preReqs = node->course.preReqs; //Sets the new course object's PreReqs vector to the correct data from the file.

            //Inserts the new course object for the current line into the binary search tree by calling the Insert function.
            bst->Insert(newCourse);
        }
        //Lets the user know that the file has been loaded properly.
        cout << endl << "File has been loaded into data structure properly." << endl;
    }
    //If the file did not open, then this will be printed. Basically, if the user inputted file name did not match a correct file name, then this branch is executed.
    else {
        cout << endl << "File was not found, so it was not loaded into data structure properly." << endl;
        cout << "Remember to use all capital letters for the class you are searching for." << endl;
        //Closes the opened file.
        inFS.close();
        //Returns after the file was not found.
        return;
    }
    //Closes the opened file.
    inFS.close();

}

//This function displays the Menu with options for the user to execute.
void DisplayMenu() {
    cout << "  Menu:" << endl;
    cout << "  1. Load Data Structure." << endl;
    cout << "  2. Print Course List." << endl;
    cout << "  3. Print Specific Course." << endl;
    cout << "  9. Exit" << endl << endl;
}


int main(int argc, char* argv[]) {
    string fileToLoad; //Holds the user entered file to be loaded/read.
    string courseNum; //holds the user entered courseNum to be searched for.

    switch (argc) {

    case 2:
        fileToLoad = argv[1];
        break;
    case 3:
        fileToLoad = argv[1];
        courseNum = argv[2];
        break;
    default:
        fileToLoad = "";
        break;
    }

    //Creates the new Binary search tree object.
    BSTree* bst = new BSTree();

    //Basic welcome statement.
    cout << "Welcome to the course planner!" << endl << endl;

    int choice = 0; //Holds the user entered choice for which menu option they wish to select.

    //This loop runs until the user enters the number 9. 
    while (choice != 9) {
        try {
            //Calls the DisplayMenu function. Will continue to display until the user enters 9 to exit the program.
            DisplayMenu();

            //Asks the user to enter one of the listed commands.
            cout << "Please enter the number of the command you wish to execute: ";
 
            cin >> choice; //Holds the user entered command to execute.

            //If the user enters something that is not of an int type, then this branch executes.
            if (cin.fail()) {
                cin.clear(); //Clears the input cache.
                cin.ignore(); //Ignores the rest of the input cache so that the loop can continue normally and not infinitely repeat.
                throw runtime_error("Invalid Input Type."); //Throws a runtime error that the input is invalid and has failed.
            }

            //If the user enters a number that is not 1, 2, 3, or 9, then this branch executes.
            if (choice > 9 || choice < 1 || choice == 4 || choice == 5 || choice == 6 || choice == 7 || choice == 8) {
                throw runtime_error("Invalid Input Value."); //Throws a runtime error that the input value is not valid.
            }

            switch (choice) {

            //If the user entered 1, then this case executes.
            case 1:

                cout << endl << "You have selected to load data in from a file." << endl;
                cout << "Please enter the name of the file you wish to load: (ex. CoursesToLoad.csv)" << endl;

                cin >> fileToLoad; //Gets user input for the file they wish to open.

                Open_Read_Load(fileToLoad, bst); //Function call to Open_Read_Load which opens the file and reads the data from it if possible.
                cout << endl;
    
                break;
            //If the user entered 2, then this case executes.
            case 2:

                cout << endl << "You have selected to print the loaded course list." << endl << endl;

                bst->InOrder(); //Prints out the binary search tree starting from the root. Will print out the data loaded from the file as long as the file has been loaded.
                cout << endl;
                break;
            //If the user entered 3, then this case executes.
            case 3:

                cout << endl << "You have selected to print a specific course in the course list." << endl;
                cout << "Please enter the course number you wish to print out: (ex. MATH201 or CSCI300)" << endl;

                cin >> courseNum; //Gets user input for the course they want to search for.

                cout << endl;

                bst->Search(courseNum); //Searches the binary search tree for the correct course and prints the course and related info if the course is found.
                cout << endl;

                break;
            }
        }
        //Catches any errors that are thrown if the user enter invalid input.
        catch (runtime_error& excpt) {
            //Outputs the error message associated with the specific thrown error.
            cout << excpt.what() << endl << endl;
        }
    }
    cout << "Thank you for using the course planner!";
}
