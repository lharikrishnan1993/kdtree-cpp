/**
*    @author Harikrishnan Lakshmanan
*    @file main.cpp
*    @date 01/10/2017
*
*    @brief Uber Coding Assignment, Kd Tree Implementation in C++.
*/

#include "kdtree_utils.cpp"
#include <time.h>
#include <sys/time.h>

using namespace kdspace;
/**
*   @param The tree into which it is to be stored, and the data to be stored.
*   @return The root of the tree so that it can be used to manipulate the tree as and when required.
*   @brief The build_kdtree function as asked in the question paper.
*/
template <typename fd>
std::shared_ptr <node <fd>> build_kdtree(kdtree <fd> &tree, std::vector<std::vector<fd>> *dataset)
{
    std::shared_ptr <node <fd>> build_tree_root;

    if (dataset->size() == 0) return build_tree_root;

    std::shared_ptr <median_data <fd> > details = std::make_shared <median_data <fd>> ();
    try
    {
        get_median(details, dataset, 0.5);
    }
    catch (const std::invalid_argument& e)
    {
        std::cout<<e.what()<<std::endl;
    }
    build_tree_root = grow_kdtree(&tree, details->data);
    build_kdtree(tree, &details->data_left);
    build_kdtree(tree, &details->data_right);
    return build_tree_root;
}
/**
*   @param The file into which the nearest neighbors distance and the index in the sample input is to be stored, and the data to be queried.
*   @return None. The file is updated and hence can be accessed where it is required.
*   @brief The query_kdtree function as asked in the question paper. The output as required in the question paper has been created as .txt file.
*/
template <typename fd>
void query_kdtree(kdtree <fd> &tree, std::vector<std::vector<fd>> *dataset, std::ofstream *file)
{
    std::shared_ptr <node <fd>> nn;
    std::vector <fd> data;
    if (dataset->size() == 0) return;

    typename std::vector<std::vector<fd>>::iterator it;

    for(it = dataset->begin(); it < dataset->end(); it++)
    {
        (*it).erase((*it).begin());
        nn = tree.search_kdtree(*it);
        *file<<nn->get_index()<<", "<<kdspace::distance(nn->get_data(), *it)<<'\n';
    }
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::vector <double> data;
        std::vector<std::vector<double>> whole_data;
        std::vector<std::vector<double>> query_data;
        whole_data.clear();
        query_data.clear();
        kdtree <double> tree;
        std::shared_ptr <node <double>> root;
        kdtree <double> tree2;
        std::shared_ptr <node <double>> root2;

        /**
        Parsing from file to extract the data to be inserted in the Kd-tree.
        */
        try
        {
            std::ifstream file;
            file.open("../src/data/sample_data.csv");
            if (!file) throw std::runtime_error("Could not open file");
            else
            {
                std::cout<<"Parsing the 'sample_data.csv' file..."<<std::endl;
                parser <double> (&whole_data, &file);
                file.close();
            }
        }
        catch (const std::runtime_error& e)
        {
            std::cout<<e.what()<<std::endl;
            return 0;
        }

        /**
            Building the Kd-tree using the parsed data.
        */
            std::cout<<"Building the Kd tree..."<<std::endl;
            root = build_kdtree(tree, &whole_data);


        /**
            Storing the devoloped tree (serialization) into a file named tree.kd
        */
        try
        {
            std::ofstream fp;
            fp.open("../src/data/tree.kd", std::ios::binary);
            if (!fp) throw std::runtime_error("Could not open file");
            else
            {
                std::cout<<"Stroring the tree to disk as 'tree.kd'."<<std::endl;
                tree.serialize_tree(root, &fp);
            }
            fp << '/';
            fp.close();
        }
        catch (const std::runtime_error& e)
        {
            std::cout<<e.what()<<std::endl;
            return 0;
        }

        /**
            Retrieving the tree (deserialization) from the file stored on disk.
        */
        try
        {
            std::ifstream file_des;
            file_des.open("../src/data/tree.kd", std::ios::binary);
            if (!file_des) throw std::runtime_error("Could not open file");
            else
            {
                std::cout<<"Loading the tree from the serialized file 'tree.kd'."<<std::endl;
                root2 = tree2.deserialize_tree(&file_des);
            }
        }
        catch (const std::runtime_error& e)
        {
            std::cout<<e.what()<<std::endl;
            return 0;
        }

        /**
            Parsing from file to extract the data to be queried in the Kd-tree.
        */
        try
        {
            std::ifstream file_query;
            file_query.open("../src/data/query_data.csv");
            if (!file_query) throw std::runtime_error("Could not open file");
            else
            {
                std::cout<<"Parsing the 'query_data.csv' file..."<<std::endl;
                parser <double> (&query_data, &file_query);
                file_query.close();
            }
        }
        catch (const std::runtime_error& e)
        {
            std::cout<<e.what()<<std::endl;
            return 0;
        }

        /**
            Querying the tree for the given data
        */

        try
        {
            std::ofstream file_op;
            file_op.open("../src/data/output.txt", std::ios::binary);
            if (!file_op) std::runtime_error("Could not open file");
            else
            {
                try
                {
                    std::cout<<"Querying to get index and Euclidean distance and stroring the requested data to disk at 'output.txt'."<<std::endl;
                    query_kdtree(tree2, &query_data, &file_op);
                }
                catch (const std::invalid_argument &e)
                {
                    std::cout<<"Given data is of incompatible dimensions with provided tree/data dimensions..."<<std::endl;
                }
            }
            file_op.close();
        }
        catch (const std::runtime_error& e)
        {
            std::cout<<e.what()<<std::endl;
            return 0;
        }
    }
    else
    {
    /**
    User command based. Only two are currently available. '-b' to build and '-d' to deserialize a given .kd file.
    Both of them query and store the file in the requested format with euclidean distance and the index.
    */
        int i = 1;
        while (i<argc)
        {
            std::vector<std::vector<double>> whole_data;
            std::vector<std::vector<double>> query_data;
            whole_data.clear();
            query_data.clear();
            kdtree <double> tree;
            std::shared_ptr <node <double>> root;

            std::string file_folder = "../src/";
            if (std::string(argv[i]) == "-b")
            {
                /**
                Parsing from file to extract the data to be inserted in the Kd-tree.
                */
                try
                {
                    std::ifstream file;
                    file.open(file_folder+=argv[i+1]);
                    if (!file) throw std::runtime_error("Could not open file");
                    else
                    {
                        std::cout<<"Parsing the given file..."<<std::endl;
                        parser <double> (&whole_data, &file);
                        file.close();
                    }
                }
                catch (const std::runtime_error& e)
                {
                    std::cout<<e.what()<<std::endl;
                    return 0;
                }

                /**
                    Building the Kd-tree using the parsed data.
                */
                std::cout<<"Building the Kd tree..."<<std::endl;
                root = build_kdtree(tree, &whole_data);

                try
                {
                    std::ifstream file_query;
                    file_query.open("../src/data/query_data.csv");
                    if (!file_query) throw std::runtime_error("Could not open file");
                    else
                    {
                        std::cout<<"Parsing the 'query_data.csv' file..."<<std::endl;
                        parser <double> (&query_data, &file_query);
                        file_query.close();
                    }
                }
                catch (const std::runtime_error& e)
                {
                    std::cout<<e.what()<<std::endl;
                    return 0;
                }

                /**
                    Querying the tree for the given data
                */

                try
                {
                    std::ofstream file_op;
                    file_op.open("../src/data/output.txt", std::ios::binary);
                    if (!file_op) std::runtime_error("Could not open file");
                    else
                    {
                        try
                        {
                            std::cout<<"Querying to get index and Euclidean distance and stroring the requested data to disk at 'output.txt'."<<std::endl;
                            query_kdtree(tree, &query_data, &file_op);
                        }
                        catch (const std::invalid_argument &e)
                        {
                            std::cout<<"Given data is of incompatible dimensions with provided tree/data dimensions..."<<std::endl;
                        }
                    }
                    file_op.close();
                }
                catch (const std::runtime_error& e)
                {
                    std::cout<<e.what()<<std::endl;
                    return 0;
                }
            }
            else if (std::string(argv[i]) == "-d")
            {
                try
                {
                    std::ifstream file_des;
                    file_des.open(file_folder+=argv[i+1], std::ios::binary);
                    if (!file_des) throw std::runtime_error("Could not open file");
                    else
                    {
                        std::cout<<"Loading the tree from the given serialized file."<<std::endl;
                        root = tree.deserialize_tree(&file_des);
                    }
                }
                    catch (const std::runtime_error& e)
                {
                    std::cout<<e.what()<<std::endl;
                    return 0;
                }

                try
                {
                    std::ifstream file_query;
                    file_query.open("../src/data/query_data.csv");
                    if (!file_query) throw std::runtime_error("Could not open file");
                    else
                    {
                        std::cout<<"Parsing the 'query_data.csv' file..."<<std::endl;
                        parser <double> (&query_data, &file_query);
                        file_query.close();
                    }
                }
                catch (const std::runtime_error& e)
                {
                    std::cout<<e.what()<<std::endl;
                    return 0;
                }

                /**
                    Querying the tree for the given data
                */

                try
                {
                    std::ofstream file_op;
                    file_op.open("../src/data/output.txt", std::ios::binary);
                    if (!file_op) std::runtime_error("Could not open file");
                    else
                    {
                        try
                        {
                            std::cout<<"Querying to get index and Euclidean distance and stroring the requested data to disk at 'output.txt'."<<std::endl;
                            query_kdtree(tree, &query_data, &file_op);
                        }
                        catch (const std::invalid_argument &e)
                        {
                            std::cout<<"Given data is of incompatible dimensions with provided tree/data dimensions..."<<std::endl;
                        }
                    }
                    file_op.close();
                }
                catch (const std::runtime_error& e)
                {
                    std::cout<<e.what()<<std::endl;
                    return 0;
                }
            }
            i++;
        }
    }

/*
//    Searching the tree to get the nearest neighbor and the corresponding euclidean distance.

    std::cout<<std::endl<<"Searching Tree..."<<std::endl;
    data = {51,3,5};
    try
    {
        nn = tree2.search_kdtree(data);
        nn->print_data();
        std::cout<<": "<<distance(nn->get_data(), data)<<std::endl;
    }
    catch (const std::invalid_argument& e)
    {
        std::cout<<"Given data is of incompatible dimensions with provided tree/data dimensions..."<<std::endl;
    }


//    Checking if the given data exists in the tree.

    data.clear();
    data = {7,10,5};
    try
    {
        if (!tree2.check_kdtree(data)) std::cout<<"Given data is unavailable in the tree..."<<std::endl;
        else std::cout<<"Given data is present in the tree. Please use [search_kdtree(data)] to get the details pertaining to the corresponding node."<<std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cout<<"Given data is of incompatible dimensions with provided tree/data dimensions..."<<std::endl;
    }
*/
    std::cout<<"Done!"<<std::endl;
    return 0;
}
