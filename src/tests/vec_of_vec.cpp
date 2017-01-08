#include <iostream>
#include <vector>
#include <algorithm>

//void check_function(std::vector <std::vector <int> > *dataset)
//{
//    std::cout<<(*dataset)[0].size()<<std::endl;
//}

//bool compare(const std::vector <int> &a, const std::vector <int> &b)
//{
//    static int count;
//    std::cout<<a[count]<<" "<<b[count]<<std::endl;
//    count += 1;
//    return a[count] < b[count];
//}

int main()
{
    std::vector <int> data;
    std::vector <int> data2;
    std::vector <std::vector <int> > dataset;
    std::vector <std::vector <int> > dataset2;

    data.push_back(200);
    data.push_back(1);
    dataset.push_back(data);

    data2.push_back(5);
    data2.push_back(56);
    dataset.push_back(data2);

    int val = 0;
//    check_function(&dataset);
    std::sort(dataset.begin(), dataset.end(), [val](const std::vector <int> &a, const std::vector <int> &b) { return  a.at(val) < b.at(val); });
    std::cout<<dataset[0][0]<<", "<<dataset[0][1]<<std::endl;

    return 0;
}
