#include <vector>
#include <iostream>
#include <typeinfo>
#include <memory>

template <class fd>
class node
{
    public:
//    private:
        std::vector <fd> data_point;
        node *left;
        node *right;
        bool collision;

//    public:
        node(std::vector <fd> &data, bool level=0);
        ~node();
        node(const node <fd> &old);
        void check_point();
};

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

template <class fd>
node <fd>::node(const node <fd> &old)
{
    std::cout<<"Copy constructor called"<<std::endl;
    this->data_point = old.data_point;
    this->collision = old.collision;
//    this->left = new < node <fd> > (&(old.left))
}

template <class fd>
void node <fd>::check_point()
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

int main()
{
    std::vector <double> data;
    data = {50};
    std::shared_ptr <node <double> > test1 = std::make_shared <node <double> > (data);
    std::cout<<"Test1: ";
    test1->check_point();

    std::shared_ptr <node <double> > test2(std::make_shared <node <double> >(*test1));
    test1->data_point = {75};
    std::cout<<"Test2: ";
    test2->check_point();
    std::cout<<"Test1: ";
    test1->check_point();
    return 0;
}
