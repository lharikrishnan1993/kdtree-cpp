/**
*    @author Harikrishnan Lakshmanan
*    @file kdtree.h
*    @date 01/06/2017
*
*    @brief Uber Coding Assignment, Kd Tree Implementation in C++.
*
*    @section Updates to make
*
*    @section Optimization Issues
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
        std::vector <fd> data_point;
        std::shared_ptr <node <fd>> left;
        std::shared_ptr <node <fd>> right;
        bool collision;

    public:
        node(std::vector <fd> &data, bool level=0);
        ~node();
        std::vector <fd> get_data() const;
        void check_point() const;
        void print_data() const;
    friend class kdtree <fd>;
};

template <class fd>
class kdtree
{
    private:
        std::shared_ptr <node <fd>> root;

    public:
        kdtree();
        ~kdtree();

        std::shared_ptr <node <fd>> insert_kdtree(std::vector <fd> &data);
        std::shared_ptr <node <fd>> insert_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, size_t depth=0, bool collsion_level=0);

        double distance(std::vector <fd> &data1, std::vector <fd> &data2) const;

        bool check_kdtree(std::vector <fd> &data) const;
        bool check_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, size_t depth=0) const;

        std::shared_ptr <node <fd>> search_kdtree(std::vector <fd> &data) const;
        std::shared_ptr <node <fd>> search_kdtree(std::vector <fd> &data, std::shared_ptr <node <fd>> subtree, std::shared_ptr <node <fd>> nearest, size_t depth=0, double best_dist=std::numeric_limits<fd>::infinity()) const;

        std::shared_ptr <node <fd>> serialize_tree(std::shared_ptr <node <fd>> subtree, std::ofstream *file) const;
        std::shared_ptr <node <fd>> deserialize_tree(std::ifstream *file);

        void print_tree(std::shared_ptr <node <fd>> subtree) const;
};

namespace kdspace
{
    template <typename fd>
    void parser(std::vector<std::vector<fd>> *whole_data, std::ifstream *file)
    {
        std::vector <fd> data;
        fd num;

        whole_data->clear();
        data.clear();

        std::string value, word;

        while (file->good())
        {
            data.clear();
            getline(*file, value, '\n');

            std::stringstream stream(value);
            while(getline(stream, word, ','))
                {
                    std::stringstream ss;
                    ss << word;
                    ss >> num;
                    data.push_back(num);
                }
            whole_data->push_back(data);
        }
        whole_data->erase(whole_data->end());
        std::cout<<"Parsed"<<std::endl;
    }

    template <typename fd>
    std::shared_ptr <node <fd> > grow_kdtree(kdtree <fd> *tree, std::vector <fd> &data)
    {
        std::shared_ptr <node <fd>> head;
        static bool root_locater = 0;
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

    template <typename fd>
    void get_split_axis(int *axis, std::vector<std::vector<fd>> *dataset)
    {
        std::map<fd, int> ranges;
        std::vector <fd> temp_vector;
        ranges.clear();

        for (int i = 0; i < (*dataset)[0].size(); i++)
        {
            temp_vector.clear();
            for (int j = 0; j < dataset->size(); j++)
                temp_vector.push_back((*dataset)[j][i]);
            ranges[*std::max_element(temp_vector.begin(), temp_vector.end()) - *std::min_element(temp_vector.begin(), temp_vector.end())] = i;
        }
        *axis = ranges.rbegin()->second;
    }

    template <class fd>
    class median_data
    {
        public:
            std::vector<fd> data;
            std::vector<std::vector <fd>> data_left;
            std::vector<std::vector <fd>> data_right;
    };

    template <typename fd>
    void get_median(std::shared_ptr <median_data <fd> > median_details, std::vector<std::vector<fd>> *dataset)
    {
        int axis = 0;
        get_split_axis(&axis, dataset);
        median_details->data_left.clear();
        median_details->data_right.clear();
        median_details->data.clear();

        std::sort(dataset->begin(), dataset->end(), [axis](const std::vector <fd> &a, const std::vector <fd> &b)
            { return  a.at(axis) < b.at(axis); });

        int median_index = 0;
        if (dataset->size()%2 == 0) median_index = dataset->size()/2 - 1;
        else median_index = dataset->size()/2;

        typename std::vector<std::vector<fd>>::iterator it;
        int i = 0;

        for(it = dataset->begin(); it < dataset->end(); it++, i++ )
        {
            if (i < median_index) median_details->data_left.push_back(*it);
            else if (i > median_index) median_details->data_right.push_back(*it);
            else median_details->data = *it;
        }
    }

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

    template <typename fd>
    std::shared_ptr <node <fd>> build_tree(kdtree <fd> &tree, std::vector<std::vector<fd>> *dataset)
    {
        std::shared_ptr <node <fd>> build_tree_root;

        if (dataset->size() == 0) return build_tree_root;

        std::shared_ptr <median_data <fd> > details = std::make_shared <median_data <fd>> ();
        get_median(details, dataset);
        build_tree_root = grow_kdtree(&tree, details->data);
        build_tree(tree, &details->data_left);
        build_tree(tree, &details->data_right);
        return build_tree_root;
    }

    template <typename fd>
    void query_tree(kdtree <fd> &tree, std::vector<std::vector<fd>> *dataset)
    {
        std::shared_ptr <node <double>> nn;
        if (dataset->size() == 0) return;

        typename std::vector<std::vector<fd>>::iterator it;
        for(it = dataset->begin(); it < dataset->end(); it++)
        {
            nn = tree.search_kdtree(*it);
            typename std::vector<fd>::iterator itin;
            std::cout<<"Nearest Neighbor for (";
            itin = (*it).begin();
            while (itin != (*it).end())
            {
                std::cout<<*itin;
                itin++;
                if (itin != (*it).end()) std::cout<<", ";
            }
            std::cout<<") is \t";
            nn->print_data();
            std::cout<<": "<<kdspace::distance(nn->get_data(), *it)<<std::endl;
        }
    }
}

#endif
