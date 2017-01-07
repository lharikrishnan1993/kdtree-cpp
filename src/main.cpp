/**
*    @author Harikrishnan Lakshmanan
*    @file main.cpp
*    @date 01/04/2017
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

int main()
{
    std::vector <double> data;
    kdtree <double> tree;
    node <double> *root;
    node <double> *nn;

    data = {50,0};
    root = grow_kdtree(&tree, data);

    data = {25,1};
    grow_kdtree(&tree, data);

    data = {75,1};
    grow_kdtree(&tree, data);

    data = {60,2};
    grow_kdtree(&tree, data);

    data = {100,5};
    grow_kdtree(&tree, data);

    data = {10,6};
    grow_kdtree(&tree, data);

    data = {49,3};
    grow_kdtree(&tree, data);

    std::cout<<std::endl<<"Printing Tree..."<<std::endl;
    tree.print_tree(root);

    std::cout<<std::endl<<"Searching Tree..."<<std::endl;
    data = {51,5};
    std::vector <double> dat = tree.search_kdtree(data);
    std::cout<<std::endl<<"Nearest Neigbor: "<<dat[0]<<" "<<dat[1]<<std::endl;
//    nn->check_point();

    return 0;
}
