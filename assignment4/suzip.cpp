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

    huffman_node(huffman_node* lc, huffman_node* rc, char c, int w)
    {
        this->left_child = lc;
        this->right_child = rc;
        this->ch = c;
        this->weight = w;
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

    huffman_tree(map<char, int>& weight_list)
    {
        this->root = build_huffman_tree(weight_list);
    }

    ~huffman_tree()
    {
        if (this->root != NULL) {
            delete this->root;
            this->root = NULL;
        }
    }

    huffman_node* build_huffman_tree(map<char, int>& weight_list)
    {
        huffman_node* root = NULL;
        if (weight_list.empty()) {
            return root;
        }

        multimap<int, huffman_node*> cur_list;
        for (map<char, int>::iterator it = weight_list.begin();
                it != weight_list.end(); ++it) {
            huffman_node* node = new huffman_node(NULL, NULL, it->first, it->second);
            cur_list.insert(pair<int, huffman_node*>(node->weight, node));
        }

        while (cur_list.size() > 1) {
            //get the min 2 elements
            multimap<int, huffman_node*>::iterator it_min = cur_list.begin(),
                                                   it_min2 = it_min;
            it_min2++;

            //new node whose weight is the sum of the 2 elements above
            huffman_node* node = new huffman_node(it_min->second, it_min2->second,
                    0, it_min->first + it_min2->first);
            cur_list.erase(cur_list.begin());
            cur_list.erase(cur_list.begin());
            cur_list.insert(pair<int, huffman_node*>(node->weight, node));
        }

        root = cur_list.begin()->second;

        return root;
    }

    map<char, string> get_huffman_code_table()
    {
        map<char, string> table;
        return table;
    }

};


class suzip
{
public:
    huffman_tree t;
};

int main()
{
    map<char, int> wl = get_weight_list("in.txt");

    huffman_tree t(wl);

#ifdef _WIN32_
    system("pause");
#endif
}
