//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Shirl Laeway
// Version     : 1.0
// Description : CS300 : Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Course structure to hold course information.
struct Course
{
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Node structure for the binary search tree.
struct Node
{
    Course course;
    Node *left;
    Node *right;

    Node()
    {
        left = nullptr;
        right = nullptr;
    }

    Node(Course aCourse)
    {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

// Binary search tree class to manage courses.
class BinarySearchTree
{
private:
    Node *root;

    void addNode(Node *node, Course course)
    {
        if (course.courseNumber < node->course.courseNumber)
        {
            if (node->left == nullptr)
            {
                node->left = new Node(course);
            }
            else
            {
                addNode(node->left, course);
            }
        }
        else
        {
            if (node->right == nullptr)
            {
                node->right = new Node(course);
            }
            else
            {
                addNode(node->right, course);
            }
        }
    }
    // In-order traversal to print the course list.
    void inOrder(Node *node)
    {
        if (node != nullptr)
        {
            inOrder(node->left);
            cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
            inOrder(node->right);
        }
    }

    void destroyTree(Node *node)
    {
        if (node != nullptr)
        {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    // Public interface for the binary search tree.
public:
    BinarySearchTree()
    {
        root = nullptr;
    }

    ~BinarySearchTree()
    {
        destroyTree(root);
    }

    void Insert(Course course)
    {
        if (root == nullptr)
        {
            root = new Node(course);
        }
        else
        {
            addNode(root, course);
        }
    }
    // Search for a course by its course number.
    Course Search(string courseNumber)
    {
        Node *currentNode = root;

        while (currentNode != nullptr)
        {
            if (currentNode->course.courseNumber == courseNumber)
            {
                return currentNode->course;
            }
            if (courseNumber < currentNode->course.courseNumber)
            {
                currentNode = currentNode->left;
            }
            else
            {
                currentNode = currentNode->right;
            }
        }
        return Course();
    }

    void PrintCourseList()
    {
        inOrder(root);
    }
};

// Trim whitespace from the beginning and end of a string.
string trim(const string &str)
{
    size_t first = str.find_first_not_of(" \t\r\n");

    if (first == string::npos)
    {
        return "";
    }

    size_t last = str.find_last_not_of(" \t\r\n");

    return str.substr(first, last - first + 1);
}

// Load courses from the CSV file.
void LoadCourses(string fileName, BinarySearchTree &bst)
{

    ifstream file(fileName);

    if (!file.is_open())
    {
        cout << "Unable to open file " << fileName << "." << endl;
        return;
    }

    string line;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }

        stringstream ss(line);

        vector<string> fields;
        string token;

        while (getline(ss, token, ','))
        {
            fields.push_back(trim(token));
        }
        if (fields.size() < 2)
        {
            continue;
        }

        Course course;

        course.courseNumber = fields[0];
        course.courseTitle = fields[1];

        for (unsigned int i = 2; i < fields.size(); i++)
        {
            course.prerequisites.push_back(fields[i]);
        }
        bst.Insert(course);
    }
    file.close();
}

// Search for and display a course.
void PrintCourse(BinarySearchTree &bst, string courseNumber)
{

    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
    Course course = bst.Search(courseNumber);

    if (course.courseNumber.empty())
    {
        cout << "Course " << courseNumber << " not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;
    // Display prerequisites if they exist.
    if (!course.prerequisites.empty())
    {
        cout << "Prerequisites: ";
        for (unsigned int i = 0; i < course.prerequisites.size(); i++)
        {
            if (course.prerequisites[i].empty())
            {
                cout << "None";
                break;
            }
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

int main()
{
    BinarySearchTree bst;

    int choice = 0;
    string fileName;
    string courseNumber;
    bool dataLoaded = false;

    cout << "####################################" << endl;
    cout << "##             Welcome            ##" << endl;
    cout << "##             to the             ##" << endl;
    cout << "##       ABCU Course Planner      ##" << endl;
    cout << "####################################" << endl;

    while (choice != 9)
    {
        cout << endl;
        cout << "Please select an option from the menu below:" << endl;
        cout << "1. Load Courses" << endl;
        cout << "2. Search for a Course" << endl;
        cout << "3. Print All Courses Sorted" << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl;
            cout << "Invalid input!" << endl;
            continue;
        }

        switch (choice)
        {

        case 1:
            cout << endl;
            cout << "Please enter the file name to load: ";
            cin.ignore(1000, '\n');
            getline(cin, fileName);
            LoadCourses(fileName, bst);
            dataLoaded = true;
            break;

        case 2:
            cout << endl;
            if (!dataLoaded)
            {
                cout << "ERROR: No course data loaded." << endl;
                cout << "Please load course data first!" << endl;
                break;
            }

            cout << "What course do you want to know about? ";
            cin >> courseNumber;
            cout << endl;
            PrintCourse(bst, courseNumber);
            break;

        case 3:
            cout << endl;
            if (!dataLoaded)
            {
                cout << "ERROR: No course data loaded." << endl;
                cout << "Please load course data first!" << endl;
                break;
            }

            cout << "Here is a sample schedule:" << endl;
            bst.PrintCourseList();
            break;

        case 9:
            cout << endl;
            cout << "####################################" << endl;
            cout << "##            Thank you           ##" << endl;
            cout << "##          for using the         ##" << endl;
            cout << "##       ABCU Course Planner!     ##" << endl;
            cout << "####################################" << endl;
            cout << endl;
            break;
        default:
            cout << choice << " is not a valid option!" << endl;
        }
    }
    return 0;
}