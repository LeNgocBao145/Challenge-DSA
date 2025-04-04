#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include "json.hpp"
#define _USE_MATH_DEFINES // for pi constant

using namespace std;
using json = nlohmann::json;

//K Dimension
const int k = 2;

struct Cities
{
    string name;
    double lat, lng;
    string country;
    int population;
    
};

struct Node{
    Cities data;
    Node* left;
    Node* right;
    int depth = 0;
};


Node* createNode(Cities data, int depth)
{
    Node* newNode = new Node;
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->depth = depth;

    return newNode;
}

void Insert(Node* &root, Cities newCity, int depth)
{
    if(root == NULL)
    {
        root = createNode(newCity, depth);
        return;
    }

    int align = depth % k;

    if(align == 1)
    {
        if(newCity.lat < root->data.lat)
        {
            Insert(root->left, newCity, depth + 1);
        }else
        {
            Insert(root->right, newCity, depth + 1);
        }
    }else if(align == 0)
    {
        if(newCity.lng < root->data.lng)
        {
            Insert(root->left, newCity, depth + 1);
        }else
        {
            Insert(root->right, newCity, depth + 1);
        }
    }
}



Node* readFile(string fileName, vector<Cities>& list)
{
    ifstream file(fileName);
    string buffer;
    
    Node* root = NULL;
    if(file.is_open())
    {
        getline(file, buffer);
        while(!file.eof())
        {
            getline(file, buffer);
            istringstream ss(buffer);
            
            Cities dumb;
            getline(ss, dumb.name, ',');

            char s = ',';
            ss >> dumb.lat >> s >> dumb.lng >> s;

            string tmp;
            getline(ss, tmp, ',');
            
            //To check if name of country is more than 2 words 
            if(tmp[0] == '"')
            {
                tmp.erase(0, 1);
                string tmp2;
                getline(ss, tmp2, ',');
                tmp2.erase(tmp2.length() - 1, 1);
                tmp += tmp2;
                dumb.country = tmp;
            }else{
                dumb.country = tmp;
            }
            
            ss >> dumb.population;

            Insert(root, dumb, 1);
            //list.push_back(dumb);
        }
    }else
    {
        cout << "Can't open file!" << endl;
    }

    file.close();

    return root;
}

int showMenu()
{
    int choice;

    cout << "1. Load the list of cities from a CSV file" << endl;
    cout << endl;

    cout << "2. Insert a new city into the KD-Tree" << endl;
    cout << endl;

    cout << "3. Insert multiple new cities into the KD-Tree from a specified CSV file path" << endl;
    cout << endl;

    cout << "4. Conduct a nearest neighbor search by providing latitude and longitude coordinates" << endl;
    cout << endl;

    cout << "5. Query all cities within a specified rectangular region defined by its geographical boundaries" << endl;
    cout << endl;

    cout << "6. Save KD-Tree to file (Serialize)" << endl;
    cout << endl;

    cout << "7. Load KD-Tree from file (Deserialize)" << endl;
    cout << endl;

    cout << "0. Exit Program" << endl;
    cout << endl;

    cout << "Type your choice (1 -> 7): ";
    cin >> choice;

    return choice;
}

void NLR(Node* pRoot)
{
    if(pRoot == NULL)
    {
        return;
    }

    cout << pRoot->data.name << " " << pRoot->data.lat << " " << pRoot->data.lng << " " << pRoot->data.country << " " << pRoot->data.population << endl;

    NLR(pRoot->left);
    NLR(pRoot->right);
}

int Height(Node* node) {
    if (!node) return 0;

    int left_height = Height(node->left);
    int right_height = Height(node->right);

    if (left_height > right_height)
        return 1 + left_height;
    else
        return 1 + right_height;
}


void rangeSearch(Node* pRoot, vector <Cities> &res, double x1, double y1, double x2, double y2) {
    // x1, y1 is the top left corner
    // x2, y2 is the bottom right corner
    if (pRoot == NULL)
    {
        return;
    }

    // verify

    double x = pRoot->data.lng;
    double y = pRoot->data.lat;

    if (x1 <= x && x <= x2 && y2 <= y && y <= y1) {
        res.push_back(pRoot->data);
    }

    rangeSearch(pRoot->left, res, x1, y1, x2, y2);
    rangeSearch(pRoot->right, res, x1, y1, x2, y2);
}

void printCities(vector <Cities> res) {
    for (Cities item : res) {
        cout << "City name: " << item.name << endl;
        cout << "Longitude: " << item.lng << endl;
        cout << "Latitude: " << item.lat << endl;
        cout << "Country: " << item.country << endl;
        cout << "Population: " << item.population << endl;
        // margin
        cout << endl;

    }
}


double calculateDistance(double x1, double y1, double x2, double y2) {
    // based on Harvesine formula
    double pi = 2 * acos(0.0);
    y1 = y1 * pi / 180.0;
    y2 = y2 * pi / 180.0;
    double delta_x = (x2 - x1) * pi / 180.0;
    double delta_y = (y2 - y1);
    double a = sin((delta_y) / 2) * sin((delta_y) / 2) + cos(y1) * cos(y2) * sin((delta_x) / 2) * sin((delta_x) / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double r = 6371e3;

    double dist = r * c;
    return dist;
}

bool isSameCities(Cities a, Cities b) {
    if (a.name == b.name &&
        a.lat == b.lat &&
        a.lng == b.lng &&
        a.population == b.population &&
        a.country == b.country) return true;
    return false;
}

int findDepth(Node* root, Cities x)
{
    // Base case
    if (root == NULL)
        return -1;

    // Initialize distance as -1
    int dist = -1;

    // Check if x is current node=
    if (isSameCities(root->data, x)

        // Otherwise, check if x is
        // present in the left subtree
        || (dist = findDepth(root->left, x)) >= 0

        // Otherwise, check if x is
        // present in the right subtree
        || (dist = findDepth(root->right, x)) >= 0)

        // Return depth of the node
        return dist + 1;

    return dist;
}

void findNearestNeighbors(Node* pRoot, Cities& res, double& dist, double x, double y) {
    // Using BST find key
    if (pRoot == NULL)
    {
        return;
    }
    int depth = pRoot->depth;
    int align = depth % k;
    double node_dist = calculateDistance(x, y, pRoot->data.lng, pRoot->data.lat);
    if (node_dist < dist) {
        dist = node_dist;
        res = pRoot->data;
    }

    if (align == 1)
    {
        if (y < pRoot->data.lat)
        {
            findNearestNeighbors(pRoot->left, res, dist, x, y);
        }
        else
        {
            findNearestNeighbors(pRoot->right, res, dist, x, y);
        }
    }
    else if (align == 0)
    {
        if (x < pRoot->data.lng)
        {
            findNearestNeighbors(pRoot->left, res, dist, x, y);
        }
        else
        {
            findNearestNeighbors(pRoot->right, res, dist, x, y);
        }
    }

}

void outputToCSV(string filename, vector <Cities> cities) {
    ofstream fileOut;
    fileOut.open(filename);

    fileOut << "city, lat, lng, country, population" << endl;

    for (Cities item : cities) {
        fileOut << item.name << "," << item.lat << "," << item.lng << "," << item.country << "," << item.population << endl;
    }

    fileOut.close();
}


// Part 4: Bảo Phạm.
// a. json format.
/*Advantage: 
    +, Human-Readable: Easy to read and debug.
    +, Language-Independent: Widely supported across various programming languages.
    +, Easier to Extend: Adding new fields or nodes can be managed without changing 
    the existing structure much.*/
/*Disadvantage:
    +, Larger Size: Takes more space than binary formats due to text representation.
    +, Slower Parsing: Text processing is generally slower than binary.*/


// Setting Deserialization.
json serializeNode(Node* root) {
    if (!root) return nullptr;

    json j;
    j["name"] = root->data.name;
    j["lat"] = root->data.lat;
    j["lng"] = root->data.lng;
    j["country"] = root->data.country;
    j["population"] = root->data.population;

    j["left"] = serializeNode(root->left);
    j["right"] = serializeNode(root->right);
    j["depth"] = root->depth;

    return j;
}

void serializeTree(Node* root, string filename) {
    filename += ".json";
    json j = serializeNode(root);
    ofstream out(filename);
    if (out.is_open()) {
        out << j.dump(4);
        out.close();
        cout << "Tree serialized successfully to " << filename << "\n";
    } else {
        cout << "Unable to open file for serialization!.\n";
    }
}

// Setting Deserialization.
Node* deserializeNode(json &j) {
    if (j.is_null()) return nullptr;

    Node* node = new Node;
    node->data.name = j["name"];
    node->data.lat = j["lat"];
    node->data.lng = j["lng"];
    node->data.country = j["country"];
    node->data.population = j["population"];

    node->left = deserializeNode(j["left"]);
    node->right = deserializeNode(j["right"]);
    node->depth = j["depth"];

    return node;
}

Node* deserializeTree(string filename) {
    ifstream file(filename);
    json j;
    if (file.is_open()) {
        cout << "Load KD-Tree successfully.\n";
        file >> j;
        file.close();
        return deserializeNode(j);
    } else {
        cout << "Unable to open file for deserialization.\n" << endl;
        return nullptr;
    }
}


void printTree(Node* root, int &count) {
    if (!root) return;
    cout << ++count << ". City: " << root->data.name << ", Latitude: " << root->data.lat
         << ", Longitude: " << root->data.lng << ", Country: " << root->data.country
         << ", Population: " << root->data.population << endl;

    printTree(root->left, count);
    printTree(root->right, count);
}

void outputCSVHandler(vector <Cities> cities) {
    char choice;
    cout << "Save to csv file? (Y/N): ";
    bool validChoice = false;
    do {
        cin >> choice;
        if (choice == 'y' ||
            choice == 'Y' ||
            choice == 'N' ||
            choice == 'n') {
            validChoice = true;
        }
    } while (!validChoice);
    
    if (choice == 'N' ||
        choice == 'n') return;
    string filename;
    cout << "CSV file name: ";
    cin >> filename;

    filename += ".csv";
    outputToCSV(filename, cities);

    cout << "Saved to " << filename << endl;
}

int main()
{
    vector<Cities> list;
    string fileName = "worldcities-20210313-population-50000+.txt";

    Node* root = NULL;


    int choice;

    while(true)
    {
        choice = showMenu();

        switch (choice)
        {
        case 0:
            system("cls");
            cout << "Good bye! See you again!" << endl;
            return 0;
        case 1:
            //Read already existed file and add items into KD tree
            root = readFile(fileName, list);
            //NLR(root);
            break;
        case 2:
            //Insert new city into KD tree
        {
            Cities newCity;
            cin.ignore();
            cout << "Name of the city: ";
            getline(cin, newCity.name);
            cout << "Latitude: ";
            cin >> newCity.lat;
            cout << "Longtitude: ";
            cin >> newCity.lng;
            cout << "Country: ";
            cin >> newCity.country;
            cout << "Population: ";
            cin >> newCity.country;
            Insert(root, newCity, 1);
        }

        break;
        case 3:
            //Read the input CSV file path and insert items from new file into existed tree
        {
            string file;
            cin.ignore();
            getline(cin, file);
            readFile(file, list);  
        }
                
            break;

        case 4:
        {
            Cities res;
            double distance = INT_MAX;
            double x, y;
            cout << "Input the coordinates (x y): ";
            cin >> x >> y;

            if (!root) {
                cout << "Current KD-Tree is empty" << endl;
            }
            else {
                findNearestNeighbors(root, res, distance, x, y);
                vector <Cities> output = {res};
                printCities(output);
                cout << "Distance: " << distance << "m" << endl;
                
                outputCSVHandler(output);
            }
        }
            break;
        case 5:
        // search cities in range based on TL - BR
        {
            double x1, y1, x2, y2;
            cout << "Input the coordinates of top left corner (x1 y1): ";
            cin >> x1 >> y1;
            cout << "Input the coordinates of top bottom right (x2 y2): ";
            cin >> x2 >> y2;

            vector <Cities> res = {};
            if (!root) {
                cout << "Current KD-Tree is empty" << endl;
            }
            else {
                rangeSearch(root, res, x1, y1, x2, y2);
                printCities(res);
                outputCSVHandler(res);
            }
            
        }
            break;
        
        case 6:
        {
            string Filename;
            cout << "Enter filename to save KD-Tree: ";
            cin >> Filename;
            serializeTree(root, Filename);
        }
            break;
        case 7:
        {
            int count = 0;
            string Filename;
            cout << "Enter filename to load KD-Tree: ";
            cin >> Filename;
            root = deserializeTree(Filename);
            printTree(root, count);
        }
            break;
        default:
            cout << "Invalid Choice!" << endl;
            break;
        }
    }

    return 0;
}