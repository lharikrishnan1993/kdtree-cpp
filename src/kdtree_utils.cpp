/**
*    @author Harikrishnan Lakshmanan
*    @file kdtree_utils.cpp
*    @date 01/06/2017
*
*    @brief Uber Coding Assignment, Kd Tree Implementation in C++.
*
*    @section Updates to make
*       1) Add dimensionality check and exception in the node depending on the root dimensions.
*       2) Add insertion strategy by changing split axis and split position
*       3) Provide facility to change the axis split and position split in run time.
*
*    @section Optimization Issues
*/

#include "kdtree.h"
bool search_left = true;

template <class fd>
node <fd>::node(std::vector <fd> &data, bool level)
{
    if (level) throw std::invalid_argument( "Error->Duplicate data" );
    else
    {
        data_point = data;
        left = nullptr;
        right = nullptr;
        collision = level;
    }
}

template <class fd>
node <fd>::~node() {}
/*
template <class fd>
node <fd>::node(const node <fd> &old)
{
    std::cout<<"Copy constructor called"<<std::endl;
}
*/
template <class fd>
void node <fd>::check_point() const
{
    if (this == nullptr)
    {
        std::cout<<"It's a leaf node with None"<<std::endl;
    }
    else
    {
        for (int dim=0; dim<this->data_point.size(); dim++)
        {
            std::cout<<data_point[dim]<<" ";
        }
        std::cout<<std::endl;
        if (this->left == nullptr) std::cout<<"Left is nullptr"<<std::endl;
        if (this->right == nullptr) std::cout<<"Right is nullptr"<<std::endl;
    }
}

template <class fd>
kdtree <fd>::kdtree()
{
    root = nullptr;
    std::cout<<"Building tree..."<<std::endl;
}

template <class fd>
kdtree <fd>::~kdtree()
{
    kill_tree(root);
    std::cout<<std::endl;
    std::cout<<"Tree destroyed."<<std::endl;
}


// Else case is a Leaf node with nullptr and were allocated in stack
// (left, right, root & head(main.cpp)). Deletes automatically when out of scope.


template <class fd>
void kdtree <fd>::kill_tree(node <fd> *subtree)
{
    if (subtree != nullptr)
    {
        delete subtree;
        kill_tree(subtree->left);
        kill_tree(subtree->right);
    }
}

template <class fd>
node <fd> *kdtree <fd>::insert_kdtree(std::vector <fd> &data)
{
    if (root == nullptr)
    {
        root = new node <fd> (data);
        return root;
    }
    else
    {
        return insert_kdtree(data, root);
    }
}

template <class fd>
node <fd> *kdtree <fd>::insert_kdtree(std::vector <fd> &data, node <fd> *subtree, size_t depth, bool collision)
{
    if (subtree == nullptr)
    {
        subtree = new node <fd> (data, collision);
        return subtree;
    }

    fd axis = fmod(depth, data.size());
    if (data[axis] < subtree->data_point[axis])
    {
        subtree->left = insert_kdtree(data, subtree->left, depth+1, collision);
    }
    else if (data[axis] > subtree->data_point[axis])
    {
        subtree->right = insert_kdtree(data, subtree->right, depth+1, collision);
    }
    else
    {
        size_t temp_counter = 0;
        for (int dim = 0; dim<data.size(); dim++)
        {
            if (data[dim] == subtree->data_point[dim])
            {
                temp_counter += 1;
            }
        }
        if (temp_counter == data.size()) collision += 1;
        subtree->right = insert_kdtree(data, subtree->right, depth+1, collision);
    }
    return subtree;
}

template <class fd>
double kdtree <fd>::distance(std::vector <fd> &data1, std::vector <fd> &data2)
{
    if (data1.size() != data2.size())
    {
        throw std::invalid_argument("Error->Incompatible dimensions. Wrong data.");
    }
    else
    {
        double sum = 0;
        for (int dim=0; dim<data1.size(); dim++)
        {
            sum += pow(data1[dim]-data2[dim],2);
        }
        return sum;
    }
}

template <class fd>
bool kdtree <fd>::check_kdtree(std::vector <fd> &data)
{
    if (root == nullptr) return root;
    else return check_kdtree(data, root);
}

template <class fd>
bool kdtree <fd>::check_kdtree(std::vector <fd> &data, node <fd> *subtree, size_t depth)
{
    if (subtree == nullptr) return false;

    if (distance(data, subtree->data_point) == 0) return true;

    fd axis = fmod(depth, data.size());
    if (data[axis] < subtree->data_point[axis])
    {
        return check_kdtree(data, subtree->left, depth+1);
    }
    return check_kdtree(data, subtree->right, depth+1);
}

template <class fd>
std::vector <fd> kdtree <fd>::search_kdtree(std::vector <fd> &data)
{
    if (root == nullptr) return root->data_point;
    else return search_kdtree(data, root, root->data_point);
}

template <class fd>
std::vector <fd> kdtree <fd>::search_kdtree(std::vector <fd> &data, node <fd> *subtree, std::vector <fd> nearest, size_t depth, double best_dist)
{
    if (subtree == nullptr)
    {
        return nearest;
    }

    double temp_dist = distance(data, subtree->data_point);
    if (temp_dist < best_dist)
    {
        nearest = subtree->data_point;
        best_dist = temp_dist;
    }

    fd axis = fmod(depth, data.size());

    if (data[axis] < subtree->data_point[axis])
    {
        search_left = true;
        nearest = search_kdtree(data, subtree->left, nearest, depth+1, best_dist);
    }
    else
    {
        search_left = false;
        nearest = search_kdtree(data, subtree->right, nearest, depth+1, best_dist);
    }

    if (abs(data[axis] - subtree->data_point[axis]) < best_dist)
    {
        if (search_left) nearest = search_kdtree(data, subtree->right, nearest, depth+1, best_dist);
        else nearest = search_kdtree(data, subtree->left, nearest, depth+1, best_dist);
    }
    return nearest;
}

template <class fd>
node <fd> *kdtree <fd>::serialize_tree(node <fd> *subtree, FILE *fp)
{
    if (subtree == nullptr)
    {
        fprintf(fp, "%lf", this->LEAF);
        return subtree;
    }

    for (int i=0; i < subtree->data_point.size(); i++)
    {
        fprintf(fp, "%lf", (subtree->data_point)[i]);
    }
    fprintf(fp, "%lf", this->DATA);

    serialize_tree(subtree->left, fp);
    serialize_tree(subtree->right, fp);
}

template <class fd>
node <fd> *kdtree <fd>::deserialize_tree(node <fd> *subtree, FILE *fp)
{
    static std::vector<fd> data;
    static fd one_dim_data;
    static node <double> *head = nullptr;
    static size_t root_locater = 0;
    data.clear();
    fscanf(fp, "%lf", &one_dim_data);
//    std::cout<<one_dim_data<<std::endl;

    if (one_dim_data == this->LEAF)
    {
        return subtree;
    }

    while (one_dim_data != this->DATA)
    {
        std::cout<<one_dim_data<<std::endl;
        data.push_back(one_dim_data);
        fscanf(fp, "%lf", &one_dim_data);
    }
//    std::cout<<data[0]<<", "<<data[1]<<std::endl;
    if (!root_locater)
    {
        subtree = this->insert_kdtree(data);
//        head = this->insert_kdtree(data);
    }
    else subtree = this->insert_kdtree(data);

    root_locater += 1;
    subtree->left = deserialize_tree(subtree->left, fp);
    subtree->right = deserialize_tree(subtree->right, fp);
//    return head;
}

template <class fd>
void kdtree <fd>::print_tree(node <fd> *subtree) const
{
    if (subtree != nullptr)
    {
        subtree->check_point();
        print_tree(subtree->left);
        print_tree(subtree->right);
    }
}
