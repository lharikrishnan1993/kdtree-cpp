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
#include <typeinfo>

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

node <double> *build_tree(kdtree <double> &tree, std::vector <std::vector<double>> &dataset)
{
    node <double> *root;
    for (int i = 0; i < dataset.size(); i++ )
    {
        root = grow_kdtree(&tree, dataset[i]);
    }
    return root;
}

int main()
{
    std::vector <std::vector <double>> dataset;
    kdtree <double> tree;
    node <double> *nn;
    node <double> *root;

    dataset = {{50,0}, {25,1}, {75,1}, {60,2}, {100,5}, {10,6}, {49,3}};
    root = build_tree(tree, dataset);

    std::cout<<std::endl<<"Printing Tree..."<<std::endl;
    tree.print_tree(root);

    std::cout<<std::endl<<"Searching Tree..."<<std::endl;
    std::vector <double> data = {51,5};
    std::vector <double> dat = tree.search_kdtree(data);
    std::cout<<std::endl<<"Nearest Neigbor: "<<dat[0]<<" "<<dat[1]<<std::endl;
//    nn->check_point();

    return 0;
}
