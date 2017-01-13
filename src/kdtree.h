/**
*    @author Harikrishnan Lakshmanan
*    @file kdtree.h
*    @date 01/10/2017
*
*    @brief Uber Coding Assignment, Kd Tree Implementation in C++.
*/

#ifndef KDTREE_H
#define KDTREE_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>

template <class fd> class kdtree;

template <class fd>
class node
{
    private:
        std::vector <fd> data_point;        /*!< Storage for data */
        std::shared_ptr <node <fd>> left;   /*!< Pointer to the left node */
        std::shared_ptr <node <fd>> right;  /*!< Pointer to the right node */
        bool collision;                     /*!< Collision count of a node */
        fd index;                           /*!< Index of the data (associated with this node) in the input file */

    public:
        node(std::vector <fd> &data, bool level=0);     /*!< Constructor. Identifies the index from data and updates both of them. If collision is true, it is rejected as duplicate data. */
        ~node();                            /*!< Destructor. All are smart pointers or stack variables and hence will get destroyed automatically */
        fd get_index() const;               /*!< Returns the index in the input file for the concerned node data */
        std::vector <fd> get_data() const;  /*!< Returns the data so as to process it later. */
        void check_point() const;           /*!< Prints data and the nodes values. Just to check if they are pointing to null or not. */
        void print_data() const;            /*!< Prints the data alone. Basically, useful for debugging, checking purposes.*/

    friend class kdtree <fd>;               /*!< Class 'kdtree' is a friend of the class 'node', so as to access the private members. */
};

template <class fd>
class kdtree
{
    private:
        std::shared_ptr <node <fd>> root;   /*!< Root (or) Head of the tree*/

    public:
        kdtree();                           /*!< Constructor. Creates a tree. */
        ~kdtree();                          /*!< Destructor. All are smart pointers and hence will get destroyed automatically */

        void insert_kdtree(std::vector <fd> &data);
        std::shared_ptr <node <fd>> insert_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, size_t depth=0, bool collsion_level=0);

        double distance(std::vector <fd> &data1, std::vector <fd> &data2) const;

        bool check_kdtree(std::vector <fd> &data) const;
        bool check_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, size_t depth=0) const;

        std::shared_ptr <node <fd>> search_kdtree(std::vector <fd> &data) const;
        std::shared_ptr <node <fd>> search_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, std::shared_ptr <node <fd>> nearest, size_t depth=0, double best_dist=std::numeric_limits<fd>::infinity()) const;

        std::shared_ptr <node <fd>> serialize_tree(std::ofstream *file, std::shared_ptr <node <fd>> subtree=nullptr) const;
        void deserialize_tree(std::ifstream *file);

        void print_tree(std::shared_ptr <node <fd>> subtree=nullptr) const;
};
/**
* A namespace to hold some of the important functions pertaining to the functioning and interfacing to the classes
* to the required functions.
*/
namespace kdspace
{
    bool serialization_flag = false;     /*!< Flag to avoid seg faults when repeatedly printed and to actovate calls from root with no parameters*/
    bool print_flag = false;            /*!< Flag to avoid seg faults when repeatedly printed and to actovate calls from root with no parameters*/
/**
*   @param A holder of type vector of vector into which the parsed data will be filled. A file which holds the data to be parsed.
*   @return Values were filled into the concerned containersa and hence does not return anything.
*   @brief Clears the containers, and later, using stringstream the data in the file is parsed and then pushed back into the vector.
*/
    template <typename fd>
    void parser(std::vector<std::vector<fd>> *whole_data, std::ifstream *file)
    {
        std::vector <fd> data;
        fd num;
        fd counter = 0;

        whole_data->clear();
        data.clear();

        std::string value, word;

        while (file->good())
        {
            data.clear();
            getline(*file, value, '\n');

            data.push_back(counter);
            std::stringstream stream(value);
            while(getline(stream, word, ','))
                {
                    std::stringstream ss;
                    ss << word;
                    ss >> num;
                    data.push_back(num);
                }
            whole_data->push_back(data);
            counter += 1;
        }
        whole_data->erase(whole_data->end());
        std::cout<<"Parsed"<<std::endl;
    }

/**
*   @param The tree into which the provided data is to be inserted.
*   @return None. Inserts the given given data into the tree.
*   @brief This is an interface function and hence takes care of the necessary functionalities in order to use the 'insert_tree' function.
*/

    template <typename fd>
    void grow_kdtree(kdtree <fd> *tree, std::vector <fd> &data)
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

/**
*   @param A holder of type vector of vector which holds the complete data. Container to fill in the split axis,
*           and values that determine which splitting strategy is to be used. By default, both the vaues, remain 0,
*           that is, it figures out the dimension which has the greatest variance. Parameter 'val' can be changed to
*           1 to get the dimension with least variance leading to the worst heuristic possible. If it is neither 0 nor 1,
*           code expects the depth value and will return it as same and this can be utilized to determine as=ny user
*           defined strategy.
*   @return Values were filled into the concerned containers and hence does not return anything.
*   @brief This decides the axis on which the data is to split. This value maintains the balance of the tree.
*/

    template <typename fd>
    void get_split_axis(int *axis, std::vector<std::vector<fd>> *dataset, const size_t val=0, const size_t depth=0)
    {
        std::map<fd, int> ranges;
        std::vector <fd> temp_vector;
        ranges.clear();

        for (int i = 1; i < (*dataset)[0].size(); i++)
        {
            temp_vector.clear();
            for (int j = 0; j < dataset->size(); j++)
                temp_vector.push_back((*dataset)[j][i]);
            ranges[*std::max_element(temp_vector.begin(), temp_vector.end()) - *std::min_element(temp_vector.begin(), temp_vector.end())] = i;
        }
        if (val == 0) *axis = ranges.rbegin()->second;
        else if (val == 1) *axis = ranges.begin()->second;
        else *axis = depth;
    }

/**
*   @brief Median_Data: This class serves to be a holder of a bunch of data, namely, the point at
*           which the data was split and the data to the left and the data to the right.
*/

    template <class fd>
    class median_data
    {
        public:
            std::vector<fd> data;
            std::vector<std::vector <fd>> data_left;
            std::vector<std::vector <fd>> data_right;
    };

/**
*   @param The pointer to the class containerto hold the median details. The complete dataset to recursively,
*           split it and the hyperparameter to choose and move the split position in the decided dimension.
*   @return The split data is inserted to the class object and it reflects in the called location. Hence, does not return anything.
*   @brief This chooses the split position given a dimension to split on from the 'get_split_axis' function.
*           This too determines the balance of the tree. The value can range anywhere between 0 and 1 (both not
*           included). If not will return exception.
*/

    template <typename fd>
    void get_median(std::shared_ptr <median_data <fd> > median_details, std::vector<std::vector<fd>> *dataset, const double val=0.5)
    {
        if (val < 1)
        {
            int axis = 0;

            get_split_axis(&axis, dataset);
            median_details->data_left.clear();
            median_details->data_right.clear();
            median_details->data.clear();

            std::sort(dataset->begin(), dataset->end(), [axis](const std::vector <fd> &a, const std::vector <fd> &b)
                { return  a.at(axis) < b.at(axis); });

            int median_index = 0;
            if (dataset->size()%2 == 0) median_index = dataset->size()*val - 1;
            else median_index = dataset->size()*val;

            typename std::vector<std::vector<fd>>::iterator it;
            int i = 0;

            for(it = dataset->begin(); it < dataset->end(); it++, i++ )
            {
                if (i < median_index) median_details->data_left.push_back(*it);
                else if (i > median_index) median_details->data_right.push_back(*it);
                else median_details->data = *it;
            }
        }
        else
        {
            throw std::invalid_argument("Error-> Split Position value (instead of median) must be less than 1.0");
        }
    }

/**
*   @param Two vector data points, within which the euclidean distance is to be identified.
*   @return Returns the distance.
*/

    template <typename fd>
    double distance(const std::vector <fd> &data1, std::vector <fd> &data2)
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
            return sqrt(sum);
        }
    }
/*

    template <typename fd>
    void query_tree(kdtree <fd> &tree, std::vector<std::vector<fd>> *dataset, std::ofstream *file)
    {
        std::shared_ptr <node <fd>> nn;
        std::vector <fd> data;
        if (dataset->size() == 0) return;
        *file << "Query Data \t\t\t\t Sample Data \t\t\t Sample Data Index \t Euclidean Distance\n";

        typename std::vector<std::vector<fd>>::iterator it;
        for(it = dataset->begin(); it != dataset->end(); it++)
        {
            (*it).erase((*it).begin());
            nn = tree.search_kdtree(*it);
            typename std::vector<fd>::iterator itin;
            *file<<"(";
            itin = (*it).begin();
            while (itin != (*it).end())
            {
                *file<<*itin;
                itin++;
                if (itin != (*it).end()) *file<<", ";
            }
            *file<<")\t";

            data.clear();
            data = nn->get_data();
            *file <<"\t(";
            itin = data.begin();
            while (itin != data.end())
            {
                *file<<*itin;
                itin++;
                if (itin != data.end()) *file<<", ";
            }
            *file <<")\t";

            *file<<nn->get_index()<<", "<<kdspace::distance(nn->get_data(), *it)<<'\n';
        }
    }
*/
}

#endif
