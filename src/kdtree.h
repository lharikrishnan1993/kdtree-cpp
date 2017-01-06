/**
*    @author Harikrishnan Lakshmanan
*    @file kdtree.h
*    @date 01/04/2017
*
*    @brief Uber Coding Assignment, Kd Tree Implementation in C++.
*
*    @section Updates to make
*       1) Write copy constructor for class node as it is required in search_kdtree(...)
*
*    @section Optimization Issues
*/

#ifndef KDTREE_H
#define KDTREE_H

#include <math.h>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <limits>

template <class fd> class kdtree;

template <class fd>
class node
{
    private:
        std::vector <fd> data_point;
        node* left;
        node* right;
        bool collision;

    public:

        node(std::vector <fd> &data, bool level=0);
        ~node();
        void check_point();
    friend class kdtree <fd>;
};

template <class fd>
class kdtree
{
    private:
        node <fd> *root;

    public:
        kdtree();
        ~kdtree();
        void kill_tree(node <fd> *subtree);

        node <fd> *insert_kdtree(std::vector <fd> &data);
        node <fd> *insert_kdtree(std::vector <fd> &data, node <fd> *subtree, size_t depth=0, bool collsion_level=0);

        double distance(std::vector <fd> &data1, std::vector <fd> &data2);
        node <fd> *search_kdtree(std::vector <fd> &data);
        node <fd> *search_kdtree(std::vector <fd> &data, node <fd> *subtree, node <fd> *nearest, double best_dist=std::numeric_limits<double>::max(), size_t depth=0);

        void print_tree(node <fd> *subtree);
};

#endif
