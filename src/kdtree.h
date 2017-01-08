/**
*    @author Harikrishnan Lakshmanan
*    @file kdtree.h
*    @date 01/06/2017
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
#include <memory>
#include <stdio.h>

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
        //node(const node <fd> &old);
        void check_point() const;
    friend class kdtree <fd>;
};

template <class fd>
class kdtree
{
    private:
        node <fd> *root;
        const fd LEAF = -std::numeric_limits<fd>::infinity();
        const fd DATA = std::numeric_limits<fd>::infinity();
        //const fd SINGLE_DATA = -std::numeric_limits<fd>::infinity();

    public:
        kdtree();
        ~kdtree();
        void kill_tree(node <fd> *subtree);

        node <fd> *insert_kdtree(std::vector <fd> &data);
        node <fd> *insert_kdtree(std::vector <fd> &data, node <fd> *subtree, size_t depth=0, bool collsion_level=0);

        double distance(std::vector <fd> &data1, std::vector <fd> &data2);
        bool check_kdtree(std::vector <fd> &data);
        bool check_kdtree(std::vector <fd> &data, node <fd> *subtree, size_t depth=0);

        std::vector <fd> search_kdtree(std::vector <fd> &data);
        std::vector <fd> search_kdtree(std::vector <fd> &data, node <fd> *subtree, std::vector <fd> nearest, size_t depth=0, double best_dist=std::numeric_limits<fd>::infinity());

        node <fd> *serialize_tree(node <fd> *subtree, FILE *fp);
        node <fd> *deserialize_tree(node <fd> *subtree, FILE *fp);

        void print_tree(node <fd> *subtree) const;
};

#endif
