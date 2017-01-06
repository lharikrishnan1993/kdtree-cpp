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

    data = {6};
    root = grow_kdtree(&tree, data);

    data = {5};
    grow_kdtree(&tree, data);

    data = {8};
    grow_kdtree(&tree, data);

    data = {7};
    grow_kdtree(&tree, data);

    data = {9};
    grow_kdtree(&tree, data);

    data = {3};
    grow_kdtree(&tree, data);

    data = {2};
    grow_kdtree(&tree, data);

    data = {4};
    grow_kdtree(&tree, data);

    std::cout<<std::endl<<"Printing Tree..."<<std::endl;
    tree.print_tree(root);

//    data = {50};
//    node <double> *test1 = new node <double> (data);
//    test1->check_point();
//    std::shared_ptr <node <double> > test2(test1);
//    test1->data_point = {75};
//    test2->check_point();
//    test1->check_point();

    std::cout<<std::endl<<"Searching Tree..."<<std::endl;
    data = {3.4};
    nn = tree.search_kdtree(data);
    std::cout<<std::endl<<"Nearest Neigbor: ";//<<hn<<std::endl;
    nn->check_point();

    return 0;
}
