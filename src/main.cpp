/**
*    @author Harikrishnan Lakshmanan
*    @file main.cpp
*    @date 01/06/2017
*
*    @brief Uber Coding Assignment, Kd Tree Implementation in C++.
*
*    @section Updates to make
*
*    @section Optimization Issues
*/

#include "kdtree_utils.cpp"
#include <time.h>
#include <sys/time.h>

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

using namespace kdspace;

int main()
{
    std::vector <double> data;
    std::vector<std::vector<double>> whole_data;
    std::vector<std::vector<double>> query_data;

    kdtree <double> tree;
    std::shared_ptr <node <double>> nn;
    std::shared_ptr <node <double>> root;
/*
    double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();
*/

/**
    Parsing from file to extract the data to be inserted in the Kd-tree.
*/
    std::ifstream file;
    file.open("sample.csv");
    parser <double> (&whole_data, &file);
    file.close();

/*
    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();

    std::cout << "Wall Time = " << wall1 - wall0 << std::endl;
    std::cout << "CPU Time  = " << cpu1  - cpu0  << std::endl;
*/

//    whole_data = {{50,0},{25,1},{75,1},{10,6},{49,3},{60,2},{100,5}};
/**
    Building the Kd-tree using the parsed data.
*/
    root = build_tree(tree, &whole_data);

/**
    Printing the tree devoloped using the parsed data.
*/
    std::cout<<std::endl<<"Printing Tree..."<<std::endl;
    tree.print_tree(root);

/**
    Storing the devoloped tree (serialization) into a file named tree.kd
*/
    std::ofstream fp;
    fp.open("tree.kd");
    if (fp.bad())
    {
        puts("Could not open file");
        return 0;
    }
    else
    {
        std::cout<<"Stroring the tree to disk"<<std::endl;
        tree.serialize_tree(root, &fp);
    }
    fp << '/';
    fp.close();

    kdtree <double> tree2;
    std::shared_ptr <node <double>> root2;
/**
    Retrieving the tree (deserialization) from the file stored on disk.
*/
    std::ifstream file_des;
    file_des.open("tree.kd");
    if (file_des.good())
    {
        std::cout<<"Loading the tree from disk"<<std::endl;
        root2 = tree2.deserialize_tree(&file_des);
    }

/**
    Printing the tree devoloped using the stored file, 'tree.kd'.
*/
    std::cout<<std::endl<<"Printing Tree2..."<<std::endl;
    tree2.print_tree(root2);

/**
    Searching the tree to get the nearest neighbor and the corresponding euclidean distance.
*/
    std::cout<<std::endl<<"Searching Tree..."<<std::endl;
    data = {51,3,5};
    try
    {
        nn = tree.search_kdtree(data);
        nn->print_data();
        std::cout<<": "<<distance(nn->get_data(), data)<<std::endl;
    }
    catch (const std::invalid_argument& e)
    {
        std::cout<<"Given data is of incompatible dimensions with provided tree/data dimensions..."<<std::endl;
    }

/**
    Checking if the given data exists in the tree.
*/
    data.clear();
    data = {7,10,5};
    try
    {
        if (tree2.check_kdtree(data)) std::cout<<"Given data is unavailable in the tree..."<<std::endl;
        else std::cout<<"Given data is present in the tree. Please use [search_kdtree(data)] to get the details pertaining to the corresponding node."<<std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cout<<"Given data is of incompatible dimensions with provided tree/data dimensions..."<<std::endl;
    }

/**
    Parsing from file to extract the data to be queried in the Kd-tree.
*/
    std::ifstream file_query;
    file_query.open("sample_query.csv");
    parser <double> (&query_data, &file_query);
    file_query.close();

/**
    Querying the tree for thr given data
*/
    try
    {
        query_tree(tree2, &query_data);
    }
    catch (const std::invalid_argument &e)
    {
        std::cout<<"Given data is of incompatible dimensions with provided tree/data dimensions..."<<std::endl;
    }

    return 0;
}
