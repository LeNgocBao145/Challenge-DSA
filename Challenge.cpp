#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct Cities
{
    string name;
    double lat, lng;
    string country;
    int population;
};

void readFile(string fileName, vector<Cities>& list)
{
    ifstream file(fileName);
    string buffer;
    

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

            getline(ss, dumb.country, ',');
            ss >> dumb.population;

            list.push_back(dumb);
        }
    }else
    {
        cout << "Can't open file!" << endl;
    }

    file.close();
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

int main()
{
    vector<Cities> list;
    string fileName = "worldcities-20210313-population-50000+.txt";

    // for(int i = 0; i < list.size(); i++)
    // {
    //     cout << list[i].name << " " << list[i].lat << " " << list[i].lng << " " << list[i].country << " " << list[i].population << endl;
    // }

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
                readFile(fileName, list);
                break;
            // case 2:
            //     break;
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