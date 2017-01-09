/**
*    @author Harikrishnan Lakshmanan
*    @file kdtree_utils.cpp
*    @date 01/06/2017
*
*    @brief Uber Coding Assignment, Kd Tree Implementation in C++.
*
*    @section Updates to make
*       1) Provide facility to change the axis split and position split in run time.
*
*    @section Optimization Issues
*
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
        collision = level;
    }
}

template <class fd>
node <fd>::~node() {}
template <class fd>
std::vector <fd> node <fd>::get_data() const
{
    return this->data_point;
}

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
void node <fd>::print_data() const
{
    for (int dim=0; dim<this->data_point.size(); dim++)
    {
        std::cout<<data_point[dim]<<" ";
    }
}


template <class fd>
kdtree <fd>::kdtree() {}

template <class fd>
kdtree <fd>::~kdtree()
{
//    std::cout<<std::endl;
//    std::cout<<"Tree destroyed."<<std::endl;
}

// Else case is a Leaf node with nullptr and were allocated in stack
// (left, right, root & head(main.cpp)). Deletes automatically when out of scope.
/*
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
*/
template <class fd>
std::shared_ptr <node <fd>> kdtree <fd>::insert_kdtree(std::vector <fd> &data)
{
    if (root.get() == nullptr)
    {
        this->root = std::make_shared<node <fd>> (data);
        return root;
    }
    else
    {
        return insert_kdtree(data, root);
    }
}

template <class fd>
std::shared_ptr <node <fd>> kdtree <fd>::insert_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, size_t depth, bool collision)
{
    if (subtree.get() == nullptr)
    {
        std::shared_ptr<node <fd>> subtree = std::make_shared<node <fd>> (data, collision);
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
double kdtree <fd>::distance(std::vector <fd> &data1, std::vector <fd> &data2) const
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
/*
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
*/
template <class fd>
std::shared_ptr <node <fd>> kdtree <fd>::search_kdtree(std::vector <fd> &data)
{
    if (root.get() == nullptr) return root;
    else return search_kdtree(data, root, root);
}

template <class fd>
std::shared_ptr <node <fd>> kdtree <fd>::search_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, std::shared_ptr <node <fd>> nearest, size_t depth, double best_dist)
{
    if (subtree.get() == nullptr) return nearest;

    double temp_dist = distance(data, subtree->data_point);
    if (temp_dist < best_dist)
    {
        nearest = subtree;
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
std::shared_ptr <node <fd>> kdtree <fd>::serialize_tree(std::shared_ptr <node <fd>> subtree, std::ofstream *file)
{
    if (subtree.get() == nullptr)
    {
        return subtree;
    }

    typename std::vector<fd>::iterator it;
    for (it=subtree->data_point.begin(); it != subtree->data_point.end();)
    {
        *file << *it;
        it++;
        if (it != subtree->data_point.end()) *file << ',';
    }
    *file << "\n";
    serialize_tree(subtree->left, file);
    serialize_tree(subtree->right, file);
    return subtree;
}

template <class fd>
std::shared_ptr <node <fd>> kdtree <fd>::deserialize_tree(std::shared_ptr <node <fd>> subtree, std::ifstream *file)
{
    std::vector<fd> data;
    std::string value, word, whole_data;
    data.clear();
    fd num;
    bool root_locater = 0;
    std::shared_ptr <node <fd>> head;

    while (file->good())
    {
        getline(*file, whole_data, '/');
        std::stringstream dataset(whole_data);
        while (getline(dataset, value, '\n'))
        {
            data.clear();
            std::stringstream stream(value);
            while(getline(stream, word, ','))
            {
                std::stringstream ss;
                ss << word;
                ss >> num;
                data.push_back(num);
            }
            if (!root_locater) head = this->insert_kdtree(data);
            else this->insert_kdtree(data);
        }
    }
    return head;
}

template <class fd>
void kdtree <fd>::print_tree(std::shared_ptr <node <fd>> subtree) const
{
    if (subtree.get() != nullptr)
    {
        subtree->check_point();
        print_tree(subtree->left);
        print_tree(subtree->right);
    }
}
