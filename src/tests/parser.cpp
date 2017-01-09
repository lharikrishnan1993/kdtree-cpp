#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

template <typename fd>
void parser(std::vector<std::vector<fd>> *whole_data, ifstream *file)
{
    vector <fd> data;
    fd num;

    whole_data->clear();
    data.clear();

    string value, word;

    while (file->good())
    {
        data.clear();
        getline(*file, value, '\n');
        stringstream stream(value);
        while(getline(stream, word, ','))
            {
                stringstream ss;
                ss << word;
                ss >> num;
                data.push_back(num);
            }
        whole_data->push_back(data);
    }
}

int main()
{
    vector<vector<float>> whole_data;

    ifstream file;
    file.open("sample.csv");

    parser <float> (&whole_data, &file);

    for (int i = 0; i < whole_data.size(); i++)
    {
        for (int j = 0; j < whole_data[i].size(); j++)
        {
            cout<<whole_data[i][j]<<",";
        }
        cout<<endl;
    }
    return 0;
}
