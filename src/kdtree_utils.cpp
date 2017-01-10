/**
*    @author Harikrishnan Lakshmanan
*    @file kdtree_utils.cpp
*    @date 01/10/2017
*
*    @brief Uber Coding Assignment, Kd Tree Implementation in C++.
*
*/

#include "kdtree.h"
/**
*   @param Data and collision level
*   @return Does not return anything. Just upadtes the values.
*   @brief Parses the index from the data and updates both of them alonside the collision.
*       If collision is high, the value is disregarded as duplicate and is ignored.
*/
template <class fd>
node <fd>::node(std::vector <fd> &data, bool level)
{
    if (level) throw std::invalid_argument( "Error->Duplicate data" );
    else
    {
        index = data[0];
        data.erase(data.begin());
        data_point = data;
        collision = level;
    }
}
/**
*   @brief Destructor. All are smart pointers and hence don't require to explicitely delete them.
*/
template <class fd>
node <fd>::~node() {}
/**
*   @param None
*   @return data of the concerned node
*   @brief Returns the data so as to process it later.
*/
template <class fd>
std::vector <fd> node <fd>::get_data() const
{
    return this->data_point;
}
/**
*   @param None
*   @return index of the concerned node in the sample input
*   @brief Returns the index so as to process it later.
*/
template <class fd>
fd node <fd>::get_index() const
{
    return this->index;
}
/**
*   @param None
*   @return None
*   @brief Prints the data and pointers for verification, display and debugging purposes.
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
/**
*   @param None
*   @return None
*   @brief Prints the data and pointers for verification, display and debugging purposes.
*/
template <class fd>
void node <fd>::print_data() const
{
    typename std::vector<fd>::iterator itin;
    std::cout<<" (";
    for (int i=0 ; i < data_point.size();)
    {
        std::cout<<data_point[i];
        i++;
        if (i != data_point.size()) std::cout<<", ";
    }
    std::cout<<") ";
}
/**
*   @param None
*   @return None
*   @brief Constructor for the kd tree
*/
template <class fd>
kdtree <fd>::kdtree() {}
/**
*   @param None
*   @return None
*   @brief Destructor for the kd tree
*/
template <class fd>
kdtree <fd>::~kdtree() {}
/**
*   @param data to be inserted
*   @return pointer to the root of the tree so that, it can manipulate wherever required.
*   @brief Creates the root, if it is null, else, passes it to the another insert function
*       which performs the same function recursively.
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
/**
*   @param data to be inserted, layer at which it is to be inserted, the subtree and collision value
*   @return pointer to the root of the tree so that, it can manipulate wherever required.
*   @brief the fundamental logic of insertion in kd tree and the explaination can be found at
*       https://web.stanford.edu/class/cs106l/handouts/assignment-3-kdtree.pdf
*/
template <class fd>
std::shared_ptr <node <fd>> kdtree <fd>::insert_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, size_t depth, bool collision)
{
    if (subtree.get() == nullptr)
    {
        std::shared_ptr<node <fd>> subtree = std::make_shared <node <fd>> (data, collision);
        return subtree;
    }

    fd axis = fmod(depth, data.size());
    if (data[axis+1] < subtree->data_point[axis])
    {
        subtree->left = insert_kdtree(data, subtree->left, depth+1, collision);
    }
    else if (data[axis+1] > subtree->data_point[axis])
    {
        subtree->right = insert_kdtree(data, subtree->right, depth+1, collision);
    }
    else
    {
        size_t temp_counter = 0;
        for (int dim = 1; dim<data.size(); dim++)
        {
            if (data[dim] == subtree->data_point[dim-1])
            {
                temp_counter += 1;
            }
        }
        if (temp_counter == data.size()) collision += 1;
        subtree->right = insert_kdtree(data, subtree->right, depth+1, collision);
    }
    return subtree;
}
/**
*   @param Two vector data points, within which the euclidean distance is to be identified.
*   @return Returns the distance.
*/
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
/**
*   @param data to be checked for.
*   @return a boolean value. true if the data exists and false if not.
*   @brief checks if the root is null (or if not tree exists) and returns itself if so. Else, sends the
*       data to another function which performs the same recursively through layers.
*/
template <class fd>
bool kdtree <fd>::check_kdtree(std::vector <fd> &data) const
{
    if (root.get() == nullptr) return false;
    else return check_kdtree(data, root);
}
/**
*   @param data to be checked for.
*   @return a boolean value. true if the data exists and false if not.
*   @brief iteratively traverses the tree using preorder traversal technique and returns true if any node matches else returns false.
*/
template <class fd>
bool kdtree <fd>::check_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, size_t depth) const
{
    if (subtree.get() == nullptr) return false;

    if (distance(data, subtree->data_point) == 0) return true;

    fd axis = fmod(depth, data.size());
    if (data[axis] < subtree->data_point[axis])
    {
        return check_kdtree(data, subtree->left, depth+1);
    }
    return check_kdtree(data, subtree->right, depth+1);
}
/**
*   @param data to which the nearest neighbor is to be identified.
*   @return returns the pointer to the node which is the nearest neighbor thr given data.
*   @brief checks if the root is null (or if not tree exists) and returns itself if so. Else, sends the
*       data to another function which performs the same recursively through layers.
*/
template <class fd>
std::shared_ptr <node <fd>> kdtree <fd>::search_kdtree(std::vector <fd> &data) const
{
    if (root.get() == nullptr) return root;
    else return search_kdtree(data, root, root);
}
/**
*   @param data to which the nearest neighbor is to be identified.
*   @return returns the pointer to the node which is the nearest neighbor thr given data.
*   @brief Iterativelyruns through the tree to identify the nearest neighbor using pre order traversal,
*       but this can only solve the problem if the nearest neighbor is on the same brach. Else, it is hard and a
*       legitimate solution has been provided at https://web.stanford.edu/class/cs106l/handouts/assignment-3-kdtree.pdf
*       and the code was inspired from this.
*/
template <class fd>
std::shared_ptr <node <fd>> kdtree <fd>::search_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, std::shared_ptr <node <fd>> nearest, size_t depth, double best_dist) const
{
    if (subtree.get() == nullptr) return nearest;
    double temp_dist = distance(data, subtree->data_point);
    if (temp_dist < best_dist)
    {
        nearest = subtree;
        best_dist = temp_dist;
    }

    fd axis = fmod(depth, data.size());
    bool search_left = false;

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

    if (abs(data[axis] - subtree->data_point[axis]) <= sqrt(best_dist))
    {
        if (search_left) nearest = search_kdtree(data, subtree->right, nearest, depth+1, best_dist);
        else nearest = search_kdtree(data, subtree->left, nearest, depth+1, best_dist);
    }
    return nearest;
}
/**
*   @param file to which data is to be written and the required node of the tree(root by default).
*   @return returns the subtree if it reaches the end of the file. If null, will return the subtree as it is.
*   @brief stores the data into a file called 'tree.kd'
 */
template <class fd>
std::shared_ptr <node <fd>> kdtree <fd>::serialize_tree(std::shared_ptr <node <fd>> subtree, std::ofstream *file) const
{
    if (subtree.get() == nullptr)
    {
        return subtree;
    }

    typename std::vector<fd>::iterator it;
    *file << subtree->index << ',';
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
/**
*   @param file from which data is to be read and the required node of the tree(root by default).
*   @return returns the subtree if it reaches the end of the file. If null, will return the subtree as it is.
*   @brief parses the data in order from the file, thus saving a lot of time in creating the tree for usage.
 */
template <class fd>
std::shared_ptr <node <fd>> kdtree <fd>::deserialize_tree(std::ifstream *file)
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
/**
*   @param The required node of the tree(root by default).
*   @return None
*   @brief Prints the node using check point so as to see all the data (or) tree.
 */
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
