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

int main()
{
    vector<Cities> list;
    string fileName = "worldcities-20210313-population-50000+.txt";
    readFile(fileName, list);

    for(int i = 0; i < list.size(); i++)
    {
        cout << list[i].name << " " << list[i].lat << " " << list[i].lng << " " << list[i].country << " " << list[i].population << endl;
    }

    return 0;
}