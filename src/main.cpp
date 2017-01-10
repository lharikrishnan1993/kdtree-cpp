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
*
*/

#include "kdtree_utils.cpp"
#include <algorithm>
#include <map>
#include <time.h>
#include <sys/time.h>

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time(){
    return (double)clock() / CLOCKS_PER_SEC;
}

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

int main()
{
    std::vector<std::vector<double>> whole_data;
    kdtree <double> tree;
    std::shared_ptr <node <double>> nn;
    std::shared_ptr <node <double>> root;
    std::vector <double> data;
/*
    double wall0 = get_wall_time();
    double cpu0  = get_cpu_time();

    std::ifstream file;
    file.open("sample.csv");
    parser <double> (&whole_data, &file);
    file.close();

    double wall1 = get_wall_time();
    double cpu1  = get_cpu_time();

    std::cout << "Wall Time = " << wall1 - wall0 << std::endl;
    std::cout << "CPU Time  = " << cpu1  - cpu0  << std::endl;
*/

    whole_data = {{5,10},{6,11},{7,12},{8,13},{9,14},{10,15},{11,16}};
    root = build_tree(tree, &whole_data);

    std::cout<<std::endl<<"Printing Tree..."<<std::endl;
    tree.print_tree(root);

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

    std::ifstream fil;
    fil.open("tree.kd");
    if (fil.good())
    {
        std::cout<<"Loading the tree from disk"<<std::endl;
        root2 = tree2.deserialize_tree(root2, &fil);
        }

    std::cout<<std::endl<<"Searching Tree..."<<std::endl;
    data = {51,5};
    try
    {
            nn = tree2.search_kdtree(data);
        nn->print_data();
        std::cout<<": "<<distance(nn->get_data(), data)<<std::endl;
    }
    catch (const std::invalid_argument& e )
    {
        std::cout<<"Given data is of incompatible dimensions with provided tree/data dimensions..."<<std::endl;
    }

    return 0;
}
