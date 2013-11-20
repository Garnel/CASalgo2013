#include <iostream>
#include <algorithm>
#include <map>
#include <fstream>
#include <string>
#include <queue>
#include <cassert>

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

//统计权重
map<char, int> get_weight_list(const char* filename)
{
    //字符次数键值对
    map<char, int> weight_list;

    ifstream fin(filename);  //read the file
    while (fin.good()) {
        char tmpch = fin.get();
        while (fin.read(&tmpch, 1)) {
            weight_list[tmpch]++;   // map的下标操作符(就是[])在元素不存在的时候会自动新建一个元素
                                    // 当tmpch已经在map里面的时候，个数加1,否则会自动新建一个键是
                                    // tmpch的项，调用int的默认构造函数，也就是初始化为0，然后自增1
        }
    }

    fin.close();
    return weight_list;
}

//HUffman tree
class huffman_tree
{
public:
    huffman_node* root;

    huffman_tree()
    {
        this->root = NULL;
    }

    huffman_tree(map<char, int>& weight_list)
    {
        this->build_from_weight_list(weight_list);
    }

    ~huffman_tree()
    {
        if (this->root != NULL) {
            delete this->root;
            this->root = NULL;
        }
    }

    inline void build_from_weight_list(map<char, int>& weight_list)
    {
        this->root = build_huffman_tree(weight_list);
    }

private:
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
};

class suzip
{
public:
    map<char, string> huffman_code;
    map<char, string> search_table;
    string to_write;

    void zip(const char* fin, const char* fout)
    {
        this->init();
        map<char, int> weight_list = get_weight_list(fin);
        huffman_tree t(weight_list);
        this->get_huffman_code_table(t);

        ifstream tozip(fin, ios::binary);  //read the file
        ofstream zipped(fout, ios::binary);

        if (tozip.fail() || zipped.fail()) {
            cerr << "ERROR!" << endl;
            return;
        }

        string code = "";
        while (tozip.good()) {
            char cur_ch = 0;
            tozip.read(&cur_ch, 1); //读取一个字符，并压缩
            code = this->huffman_code[cur_ch];
            this->write_bits(zipped, code);
        }

        size_t diff8 = 8 - this->to_write.length();
        if (diff8 < 8 && diff8 > 0) {  //补齐一个字节
            string align = string(diff8, '1');
            this->write_bits(zipped, align);
        }

        tozip.close();
        zipped.close();
    }

private:
    void init()
    {
        if (!this->huffman_code.empty()) {
            this->huffman_code.erase(this->huffman_code.begin(), this->huffman_code.end());
        }

        if (!this->search_table.empty()) {
            this->search_table.erase(this->search_table.begin(), this->search_table.end());
        }
        this->to_write = "";
    }

    void get_huffman_code_table(huffman_tree& t)
    {
        this->huffman_code.erase(this->huffman_code.begin(), this->huffman_code.end());
        this->search_table.erase(this->search_table.begin(), this->search_table.end());

        //用队列实现层次遍历
        queue<huffman_node*> nodes;
        queue<string> codes;
        nodes.push(t.root);
        codes.push("0");
        while (!nodes.empty()) {
            huffman_node* cur_node = nodes.front();
            string cur_code = codes.front();

            //叶子节点，也就是记录有效字符的节点，可以得出这个字符的huffman编码
            if (cur_node->left_child == NULL && cur_node->right_child == NULL) {
                this->huffman_code[cur_node->ch] = cur_code;
                this->search_table[cur_code] = cur_node->ch;
                //cout << cur_node->ch << " " << (int)(cur_node->ch) << " " << cur_code << endl;
            } else {  //huffman树要么左右孩子都为空，要么都不为空
                nodes.push(cur_node->left_child);  codes.push(cur_code + '0');
                nodes.push(cur_node->right_child); codes.push(cur_code + '1');
            }

            //弹出刚刚处理过的节点
            nodes.pop();
            codes.pop();
        }
    }

    void write_bits(ofstream& output, string& bits_str)
    {
        this->to_write += bits_str;
        while (this->to_write.length() > 8) {
            string bits = this->to_write.substr(0, 8);
            char ch = this->bits2char(bits);
            to_write.assign(this->to_write.begin() + 8, this->to_write.end());
            output.write(&ch, 1);
        }
    }

    char bits2char(string& bits)
    {
        assert(bits.length() == 8);
        char ch = 0, flag = 1;
        for (int i = 0; i < 8; ++i) {
            if (bits[i] == '1') {
                ch |= flag << (7 - i);
            }
        }

        return ch;
    }

    //把huffman编码放到
    void write_header(ofstream& output)
    {
        const char* file_type = "SUZ\n";
        output << file_type;


    }
};

int main()
{
    suzip worker;
    worker.zip("in", "out");
#ifdef _WIN32_
    system("pause");
#endif
}
