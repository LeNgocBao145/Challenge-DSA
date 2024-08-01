#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

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
};

Node* createNode(Cities data)
{
    Node* newNode = new Node;
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

void Insert(Node* &root, Cities newCity, int depth)
{
    if(root == NULL)
    {
        root = createNode(newCity);
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

    cout << "0. Exit Program" << endl;
    cout << endl;

    cout << "Type your choice (1 -> 5): ";
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

int main()
{
    vector<Cities> list;
    string fileName = "worldcities-20210313-population-50000+.txt";

    Node* root = NULL;


    int choice;

    while(true)
    {
        choice = showMenu();

        switch(choice)
        {
            case 0:
                system("cls");
                cout << "Good bye! See you again!" << endl;
                return 0;
            case 1:
                //Read already existed file and add items into KD tree
                root = readFile(fileName, list);
                NLR(root);
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
                string file;
                cin.ignore();
                getline(cin, file);
                readFile(file, list);
                break;
            // case 4:
            //     break;
            // case 5:
            //     break;
            // default:
            //     cout << "Invalid Choice!" << endl;
            //     break;
        }
    }

    return 0;
}