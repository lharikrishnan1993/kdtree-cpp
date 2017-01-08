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

int get_split_axis(std::vector <std::vector<double> > *dataset)
{
    std::map<double,int> ranges;
    std::vector <double> temp_vector;
    ranges.clear();

    for (int i = 0; i < 2; i++)
    {
        temp_vector.clear();
        for (int j = 0; j < dataset->size(); j++)
            temp_vector.push_back((*dataset)[j][i]);
        ranges[*std::max_element(temp_vector.begin(), temp_vector.end()) - *std::min_element(temp_vector.begin(), temp_vector.end())] = i;
    }
    return ranges.rbegin()->second;
}

node <double> *build_tree(kdtree <double> &tree, std::vector <std::vector<double> > *dataset)
{
    static node <double> *root = nullptr;

    if (dataset->size() == 0) return root;

// Once done with the reconstruction, the following block is not needed
    for (int i = 0; i < dataset->size(); i++)
    {
        root = grow_kdtree(&tree, (*dataset)[i]);
    }

    return root;
}

int main()
{
    std::vector <std::vector <double>> dataset;
    kdtree <double> tree;
    node <double> *nn;
    node <double> *root;
    std::vector <double> data;

    data = {50,0};
//    std::cout<<data.size()<<std::endl;
    dataset.push_back(data);
//    std::cout<<dataset.size()<<std::endl;
    data.clear();

    data = {25,1};
    dataset.push_back(data);
    data.clear();

    data = {75,1};
    dataset.push_back(data);
    data.clear();

    data = {60,2};
    dataset.push_back(data);
    data.clear();

    data = {100,5};
    dataset.push_back(data);
    data.clear();

    data = {200,6};
    dataset.push_back(data);
    data.clear();

    data = {49,3};
    dataset.push_back(data);
    data.clear();

    std::cout<<get_split_axis(&dataset)<<std::endl;
    root = build_tree(tree, &dataset);

    std::cout<<std::endl<<"Printing Tree..."<<std::endl;
    tree.print_tree(root);

    std::cout<<std::endl<<"Searching Tree..."<<std::endl;
    data = {51,5};
    std::vector <double> dat = tree.search_kdtree(data);
    std::cout<<std::endl<<"Nearest Neigbor: "<<dat[0]<<" "<<dat[1]<<std::endl;
//    nn->check_point();

    return 0;
}
