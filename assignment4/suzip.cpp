#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>

using namespace std;

class huffman_node
{
public:
    huffman_node* left_child;
    huffman_node* right_child;
    int weight;
    char ch;
    bool is_leaf;

    huffman_node()
    {
        this->left_child = NULL;
        this->right_child = NULL;
        this->ch = 0;
        this->weight = 0;
        this->is_leaf = true;
    }

    huffman_node(huffman_node* lc, huffman_node* rc, char c, int w, bool _is_leaf)
    {
        this->left_child = lc;
        this->right_child = rc;
        this->ch = c;
        this->weight = w;
        this->is_leaf = _is_leaf;
    }

    ~huffman_node()
    {
        //delete the left and right child
        if (this->left_child != NULL) {
            delete this->left_child;
            this->left_child = NULL;
        }

        if (this->right_child != NULL) {
            delete this->right_child;
            this->right_child = NULL;
        }
    }
};

const int CODEBITS = 256;

//统计权重
map<char, int> get_weight_list(const char* filename) 
{
    //字符次数键值对
    map<char, int> weight_list;

    ifstream fin(filename);  //read the file
    while (fin.good()) {
        char tmpch = fin.get();
        while (fin.read(&tmpch, 1)) {
            weight_list[tmpch]++;   // If the element tmpch does not exist then it is created and 
                                    // initialized to zero. A reference to the internal value
                                    // is returned. so that the ++ operator can be applied.

                                    // If tmpch did not exist it now exist and is 1.
                                    // If tmpch had a value of 'n' it now has a value of 'n+1'
        }
    }

    return weight_list;
}

//HUffman tree
class huffman_tree
{
public:
    huffman_node* root;
    
    huffman_tree(map<char, int> weight_list)
    {
        if (this->root != NULL) {
            delete this->root;
        }
        this->root = build_huffman_tree(weight_list);
    }

    ~huffman_tree()
    {
        if (this->root != NULL) {
            delete this->root;
            this->root = NULL;
        }
    }

    huffman_node* build_huffman_tree(map<char, int> weight_list) 
    {
        huffman_node** 
    }

    static void gen_weight_list(const char* filename, int weight_list[]) 
    {
        
    }
};


class suzip
{
    
};

int main()
{
    map<char, int> wl = get_weight_list("in.txt");
    for (map<char, int>::iterator it = wl.begin(); it != wl.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }

    system("pause");
}