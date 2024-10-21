// Author: Gavin Bish October 14th, 2024

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;

const int GLOBAL_SLEEP_TIME = 5000;//default time for sleep

/*
Struct that holds the course information
*/
struct Course {

    string courseId;
    string courseName;
    vector<string> preList;
};

class BinarySearchTree {

private:
    // Define structures to hold courses and creates new Nodes
    struct Node {
        Course course;
        Node* right;
        Node* left;

        // Default constructor
        Node() {
            left = nullptr;
            right = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;
    void inOrder(Node* node);
    int size = 0;

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course aCourse);
    void Remove(string courseId);
    Course Search(string courseId);
    int Size();
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    this->root = nullptr;
}

/**
 * Traverse the tree starting from Root
 */
void BinarySearchTree::InOrder() {
    inOrder(root);
}

/**
 * Insert a course in the Binary Search Tree
 */
void BinarySearchTree::Insert(Course aCourse) {

    Node* currentNode = root;

    if (root == NULL) {

        root = new Node(aCourse);

    }
    else {

        while (currentNode != NULL) {

            if (aCourse.courseId < currentNode->course.courseId) {

                if (currentNode->left == nullptr) {

                    currentNode->left = new Node(aCourse);
                    currentNode = NULL;
                }
                else {

                    currentNode = currentNode->left;
                }
            }
            else {

                if (currentNode->right == nullptr) {

                    currentNode->right = new Node(aCourse);
                    currentNode = NULL;
                }
                else {

                    currentNode = currentNode->right;
                }
            }
        }
    }
    size++;
}

/**
 * Removes a course from the Binary Search Tree
 */
void BinarySearchTree::Remove(string courseId) {
    Node* par = NULL;
    Node* curr = root;

    while (curr != NULL) {

        if (curr->course.courseId == courseId) {
            if (curr->left == NULL && curr->right == NULL) {
                if (par == NULL) {
                    root = nullptr;
                }
                else if (par->left == curr) {
                    par->left = NULL;
                }
                else {
                    par->right = NULL;
                }
            }
            else if (curr->right == NULL) {
                if (par == NULL) {
                    root = curr->left;
                }
                else if (par->left == curr) {
                    par->left = curr->left;
                }
                else {
                    par->right = curr->left;
                }
            }
            else if (curr->left == NULL) {
                if (par == NULL) {
                    root = curr->right;
                }
                else if (par->left == curr) {
                    par->left = curr->right;
                }
                else {
                    par->right = curr->right;
                }
            }
            else {
                Node* suc = curr->right;

                while (suc->left != NULL) {
                    suc = suc->left;

                }

                Node successorData = Node(suc->course);
                Remove(suc->course.courseId);
                curr->course = successorData.course;
            }
            return;
        }
        else if (curr->course.courseId < courseId) {
            par = curr;
            curr = curr->right;
        }
        else {
            par = curr;
            curr = curr->left;
        }
    }
    cout << "\nValue not found" << endl;
    return;
}

/**
 * Searches for a course in the Binary Search Tree
 */
Course BinarySearchTree::Search(string courseId) {

    Course aCourse;

    Node* currentNode = root;

    while (currentNode != NULL) {

        if (currentNode->course.courseId == courseId) {

            return currentNode->course;
        }
        else if (courseId < currentNode->course.courseId) {

            currentNode = currentNode->left;
        }
        else {

            currentNode = currentNode->right;
        }
    }
    return aCourse;
}

void BinarySearchTree::inOrder(Node* node) {

    if (node == NULL) {

        return;
    }
    inOrder(node->left);

    //Prints the courseID node
    cout << node->course.courseId << ", " << node->course.courseName << endl;

    inOrder(node->right);
}

int BinarySearchTree::Size() {

    return size;
}

/*
Split tree by delimiter, found on StackOverFlow
*/
vector<string> Split(string lineFeed) {

    char delim = ',';

    lineFeed += delim; //includes a delimiter at the end so last word is also read
    vector<string> lineTokens;
    string temp = "";
    for (int i = 0; i < lineFeed.length(); i++)
    {
        if (lineFeed[i] == delim)
        {
            lineTokens.push_back(temp); //store words in token vector
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}

/*
This function loads the courses
*/
void loadCourses(string csvPath, BinarySearchTree* courseList) {
    // Creates the data structure and loads the course data into it

    ifstream inFS; //insteam to read file
    string line; //line feed 
    vector<string> stringTokens;

    //Open files to read based on csvPath
    inFS.open(csvPath);

    //Handler for open file errors
    if (!inFS.is_open()) { 
        cout << "Could not open file. Please check inputs. " << endl;
        return;
    }

    while (!inFS.eof()) {

        Course aCourse;//create a new struct for each "line"

        getline(inFS, line);
        stringTokens = Split(line); //split the line into tokens via the delimiter

        if (stringTokens.size() < 2) { //if there aren't 2 tokens the line is misformatted

            cout << "\nError. Skipping line." << endl;
        }

        else {

            aCourse.courseId = stringTokens.at(0);
            aCourse.courseName = stringTokens.at(1);

            for (unsigned int i = 2; i < stringTokens.size(); i++) {

                aCourse.preList.push_back(stringTokens.at(i));
            }

            // push this course to the end
            courseList->Insert(aCourse);
        }
    }

    inFS.close(); //close the file
}

/*
Course information is passed in for display to console
*/
void displayCourse(Course aCourse) {

    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";

    //if the list is empty then prerequisites are none
    if (aCourse.preList.empty()) {

        cout << "none" << endl;
    }
    else {

        for (unsigned int i = 0; i < aCourse.preList.size(); i++) {

            cout << aCourse.preList.at(i);

            //put a comma for any elements greater than 1
            if (aCourse.preList.size() > 1 && i < aCourse.preList.size() - 1) {

                cout << ", ";
            }
        }
    }

    cout << endl;
}

/*
Force the case of any string to uppercase
Pass the string by reference and change the case of any alpha to upper
*/
void convertCase(string& toConvert) {

    for (unsigned int i = 0; i < toConvert.length(); i++) {

        if (isalpha(toConvert[i])) {

            toConvert[i] = toupper(toConvert[i]);
        }
    }
}

int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, aCourseKey;

    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        aCourseKey = argv[2];
        break;
    default:
        csvPath = "FinalProject_Sample_input.csv";
    }

    // defines the table to hold all the course information
    BinarySearchTree* courseList = new BinarySearchTree();

    Course course;
    bool goodInput;
    int choice = 0;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Remove Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        //clear the string 
        aCourseKey = "";       
        string anyKey = " ";
        choice = 0;

        try {
            cin >> choice;

            // Limites the user inptu for the menu to good values
            if ((choice > 0 && choice < 5) || (choice == 9)) {
                goodInput = true;
            }
            //Throws an error for a catch
            else {
                goodInput = false;
                throw 1;
            }

            switch (choice) {
            case 1:

                // Completes the load course method
                loadCourses(csvPath, courseList);
                cout << courseList->Size() << " courses read" << endl;

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 2:
                courseList->InOrder();

                cout << "\nEnter \'y\' to continue..." << endl;

                cin >> anyKey;

                break;

            case 3:

                cout << "\nWhat course do you want to know about? " << endl;
                cin >> aCourseKey;

                //Converts the case for user input
                convertCase(aCourseKey); 

                course = courseList->Search(aCourseKey);

                if (!course.courseId.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "\nCourse ID " << aCourseKey << " not found." << endl;
                }

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 4:

                cout << "\nWhat course do you want delete? " << endl;
                cin >> aCourseKey;

                //Converts the case for user input
                convertCase(aCourseKey);

                courseList->Remove(aCourseKey);

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 9:
                exit;
                break;

            default:

                throw 2;
            }
        }

        catch (int err) {

            std::cout << "\nPlease check your input." << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }

        //need to clear the cin operator of any extra input
        cin.clear();
        cin.ignore();

        //Clears console to redraw a new console
        system("cls");
    }

    cout << "Good bye." << endl;

    Sleep(GLOBAL_SLEEP_TIME);

    return 0;
}