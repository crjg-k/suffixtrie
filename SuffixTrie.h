//
// Created by admin on 22-3-31 031.
//

#ifndef _SUFFIXTRIE_H
#define _SUFFIXTRIE_H
#include <cstring>
#include <map>
#include <vector>
#include <iostream>
using namespace std;

class SuffixTrie {
    char *text{};
    int64_t root{}, size{}, position = -1,
            currentNode=0,  //当前到数组中哪个位置了, 后面插入新的节点就在该位置插入
            needSuffixLink{},
            remainder=0,    //当前剩余隐式包含的后缀的数量
            active_node{}, active_length{}, active_edge{};  //三元组
    const static int64_t presetMax = INT64_MAX;
    struct SuffixNode {
        /*
            不需要创建 Edge 类.
            有关边的信息存储在节点中.
            [start, end) int64_t 的区间指定边, 节点通过它连接到其父节点.
        */
        map<char, int64_t> next;
        SuffixNode()=default;
        SuffixNode(int64_t start, int64_t end, int64_t suffixIndex):start(start),end(end),suffixIndex(suffixIndex) {}
        int64_t edgeLength(int64_t pos) const {
            return min(end, pos + 1) - start;
        }
        int64_t start{}, end = presetMax,
                link=0, //该节点是否有后缀链接
                suffixIndex{};  //该叶子节点所代表的后缀是从原串中哪个位置开始的, 从0开始
    };
    SuffixNode *nodes= nullptr;

    void printLeaves(int64_t x, fstream &f);
    void printInternalNodes(int64_t x, fstream &f);
    void printEdges(int64_t x, fstream &f);
    void printSLinks(int64_t x, fstream &f);
    void dfs(int64_t now, vector<int64_t> *v) const;
    void addSuffixLink(int64_t node);
    char getActiveEdge() const;
    bool walkDown(int64_t next);
    int64_t newNode(int64_t start, int64_t end, int64_t suffixIndex=-1);
    void addChar(char c);
    string edgeString(int64_t node) const;
    string findLongestCommon(int64_t now=0,int64_t cnt=0,int64_t flag=0);
public:
    explicit SuffixTrie()=default;
    /**
     * 2022-04-03 11:59:22 GMT+8
     * @param s 要构建后缀树的字符串
     * @param final 结尾字符, 在对两个不同字符串查询公共部分时, 用来区分不同的字符串
     */
    explicit SuffixTrie(std::string s, const string& final="\1"):size(s.size()) {
        s.append(final);
        nodes = new SuffixNode[(s.size()<<1)+2];
        text = new char[s.size()];
        remainder=0;
        root = active_node = newNode(-1, -1);
        for (char ch : s)   addChar(ch);
    }
    /**
     * 2022-03-31 18:07:04 GMT+8
     * @param t 查找的子串
     * @return 子串t对应的开始索引
     */
    vector<int64_t>* findSubstring(const std::string &t) const;
    /**
     * 2022-03-31 18:07:06 GMT+8
     * @param t 统计的子串
     * @return 子串t的出现次数
     */
    int64_t statisticSubstring(const string &t) const;
    /**
     * 2022-03-31 18:07:25 GMT+8
     * @return 重复出现的最长子串
     */
    string findMostRepeatSubstring(int64_t now=0,int64_t cnt=0,int64_t flag=0);
    /**
     * 2022-03-31 18:07:44 GMT+8
     * @param q 第一个串
     * @param r 第二个串
     * @return 公共最长子串
     */
    static string findLongestCommon(string &q,string &r){
        r.append("\2");
        SuffixTrie st(q);
        auto tempNodes=new SuffixNode[((q.size()+1+r.size())<<1)+2];    //节点个数不会超过2n+2个
        copy(st.nodes, st.nodes+st.currentNode, tempNodes);
        st.nodes=tempNodes;
        auto tempText=new char[q.size()+1+r.size()];
        copy(st.text, st.text+st.size+1, tempText);
        st.text=tempText;
        st.size=q.size()+1+r.size();
        st.root=st.remainder=st.active_node=st.active_length=st.active_edge=0;
        for (char ch : r)   st.addChar(ch);
        st.displaySuffixTrie();
        delete []tempNodes;
        delete []tempText;
        return st.findLongestCommon();
    }
    /**
     * 2022-04-14 21:32:35 GMT+8
     * 以图形化方式输出树的结构, 以graphviz的DOT格式
     */
    void displaySuffixTrie();
    /**
     * 2022-04-19 18:32:35 GMT+8
     * 输出节点信息
     */
    void outputNodeInfo() const;
    /**
     * 2022-04-17 11:20:01 GMT+8
     * @return this->size
     */
    int64_t getSize() const;
};

#endif //_SUFFIXTRIE_H
