/**
*    @author Harikrishnan Lakshmanan
*    @file main.cpp
*    @date 01/06/2017
*
*    @brief Uber Coding Assignment, Kd Tree Implementation in C++.
*
*    @section Updates to make
*
*    @section Optimization Issues
*
*/

#include "kdtree_utils.cpp"
#include <algorithm>
#include <map>

node <double> *grow_kdtree(kdtree <double> *tree, std::vector <double> &data)
{
    static node <double> *head = nullptr;
    static size_t root_locater = 0;
    if (!root_locater)
    {
        head = tree->insert_kdtree(data);
        data.clear();
    }
    else
    {
        try
        {
            tree->insert_kdtree(data);
            data.clear();
        }
        catch (const std::invalid_argument& e )
        {
            std::cout<<"Ignoring due to duplication..."<<std::endl;
        }
    }
    root_locater += 1;
    return head;
}

void get_split_axis(int *axis, std::vector<std::vector<double>> *dataset)
{
    std::map<double,int> ranges;
    std::vector <double> temp_vector;
    ranges.clear();

    for (int i = 0; i < (*dataset)[0].size(); i++)
    {
        temp_vector.clear();
        for (int j = 0; j < dataset->size(); j++)
            temp_vector.push_back((*dataset)[j][i]);
        ranges[*std::max_element(temp_vector.begin(), temp_vector.end()) - *std::min_element(temp_vector.begin(), temp_vector.end())] = i;
    }
    *axis = ranges.rbegin()->second;
}

class median_data
{
    public:
        std::vector<double> data;
        std::vector<std::vector <double>> data_left;
        std::vector<std::vector <double>> data_right;
};

void get_median(median_data *median_details, std::vector<std::vector<double>> *dataset)
{
    int axis = 0;
    get_split_axis(&axis, dataset);
    median_details->data_left.clear();
    median_details->data_right.clear();
    median_details->data.clear();

    std::sort(dataset->begin(), dataset->end(), [axis](const std::vector <double> &a, const std::vector <double> &b)
        { return  a.at(axis) < b.at(axis); });

    int median_index = 0;
    if (dataset->size()%2 == 0) median_index = dataset->size()/2 - 1;
    else median_index = dataset->size()/2;

    std::vector<std::vector<double>>::iterator it;
    int i = 0;

    for(it = dataset->begin(); it < dataset->end(); it++, i++ )
    {
        if (i < median_index) median_details->data_left.push_back(*it);
        else if (i > median_index) median_details->data_right.push_back(*it);
        else median_details->data = *it;
    }
}

node <double> *build_tree(kdtree <double> &tree, std::vector<std::vector<double>> *dataset)
{
    static node <double> *root = nullptr;

    if (dataset->size() == 0) return root;

    median_data *details = new median_data;
    get_median(details, dataset);

    root = grow_kdtree(&tree, details->data);
    build_tree(tree, &details->data_left);
    build_tree(tree, &details->data_right);

    return root;
}

int main()
{
    std::vector<std::vector<double>> whole_data;
    kdtree <double> tree;
    node <double> *nn;
    node <double> *root;
    std::vector <double> data;

    whole_data = {{50,0}, {25,1}, {75,1}, {60,2}, {100,5}, {10,6}, {49,3}};

    root = build_tree(tree, &whole_data);

    std::cout<<std::endl<<"Printing Tree..."<<std::endl;
    tree.print_tree(root);

    std::cout<<std::endl<<"Searching Tree..."<<std::endl;
    data = {51,5};
    std::vector <double> dat = tree.search_kdtree(data);
    std::cout<<std::endl<<"Nearest Neigbor: "<<dat[0]<<" "<<dat[1]<<std::endl;
//    nn->check_point();

    return 0;
}
