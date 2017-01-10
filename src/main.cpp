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
    for(it = dataset->begin(); it != dataset->end(); it++)
    {
        (*it).erase((*it).begin());
        nn = tree.search_kdtree(*it);
        *file<<nn->get_index()<<", "<<kdspace::distance(nn->get_data(), *it)<<'\n';
    }
}

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
    try
    {
        std::ifstream file;
        file.open("../src/data/sample_data.csv");
        if (!file) throw std::runtime_error("Could not open file");
        else
        {
            parser <double> (&whole_data, &file);
            file.close();
        }
    }
    catch (const std::runtime_error& e)
    {
        std::cout<<e.what()<<std::endl;
        return 0;
    }
/*
    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();

    std::cout << "Wall Time = " << wall1 - wall0 << std::endl;
    std::cout << "CPU Time  = " << cpu1  - cpu0  << std::endl;
*/

//    whole_data = {{0,50,0},{1,25,1},{2,75,1},{3,10,6},{4,49,3},{5,60,2},{6,100,5}};
/**
    Building the Kd-tree using the parsed data.
*/
    root = build_kdtree(tree, &whole_data);

/**
    Printing the tree devoloped using the parsed data.
*/
    std::cout<<std::endl<<"Printing Tree..."<<std::endl;
    tree.print_tree(root);


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
            std::cout<<"Stroring the tree to disk"<<std::endl;
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

    kdtree <double> tree2;
    std::shared_ptr <node <double>> root2;
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
            std::cout<<"Loading the tree from disk"<<std::endl;
            root2 = tree2.deserialize_tree(&file_des);
        }
    }
    catch (const std::runtime_error& e)
    {
        std::cout<<e.what()<<std::endl;
        return 0;
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
        if (!tree2.check_kdtree(data)) std::cout<<"Given data is unavailable in the tree..."<<std::endl;
        else std::cout<<"Given data is present in the tree. Please use [search_kdtree(data)] to get the details pertaining to the corresponding node."<<std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cout<<"Given data is of incompatible dimensions with provided tree/data dimensions..."<<std::endl;
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
        file_op.open("../src/data/query_output.txt", std::ios::binary);
        if (!file_op) std::runtime_error("Could not open file");
        else
        {
            try
            {
                std::cout<<"Stroring the requested data to disk"<<std::endl;
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

    return 0;
}
