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

    string value, word, delimiter;

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
                std::cout<<num<<std::endl;
            }
        whole_data->push_back(data);
    }
    whole_data->erase(whole_data->end());
}

template <typename fd>
void saver(std::vector<std::vector<fd>> *whole_data, ofstream *file)
{
    if (file->is_open())
    {
        typename std::vector<std::vector<fd>>::iterator it;
        for ( it = whole_data->begin(); it != whole_data->end(); it++)
        {
            typename std::vector <fd>::iterator itin;
            for (itin = (*it).begin(); itin != (*it).end();)
            {
                *file << *itin;
                itin++;
                if (itin != (*it).end()) *file << ',';
            }
            *file << "\n";
        }
    }
}

int main()
{
    vector<vector<float>> whole_data;

    ifstream file;
    file.open("tree.csv");
    parser <float> (&whole_data, &file);

    for (int i = 0; i < whole_data.size(); i++)
    {
        for (int j = 0; j < whole_data[i].size(); j++) cout<<whole_data[i][j]<<",";
        cout<<endl;
    }
    file.close();

    ofstream file_save;
    file_save.open("sample.csv");
    saver <float> (&whole_data, &file_save);
    file_save.close();

    return 0;
}
