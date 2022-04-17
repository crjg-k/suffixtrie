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
public:
    char *text{};
    int64_t root{}, position = -1,
            currentNode=0,
            needSuffixLink{},
            remainder=0,
            active_node{}, active_length{}, active_edge{}, size{};
    const static int64_t presetMax = INT64_MAX;
    struct SuffixNode {
        /*
            There is no need to create an "Edge" class.
            Information about the edge is stored right in the node.
            [start, end) int64_t interval specifies the edge,
            by which the node is connected to its parent node.
        */
        map<char, int64_t> next;
        SuffixNode()=default;
        SuffixNode(int64_t start, int64_t end, int64_t suffixIndex):start(start),end(end),suffixIndex(suffixIndex) {}
        int64_t edgeLength(int64_t position) const {
            return min(end, position + 1) - start;
        }
        int64_t start{}, end = presetMax, link=0, suffixIndex{};
    };
    SuffixNode *nodes= nullptr;

    void printLeaves(int64_t x);
    void printInternalNodes(int64_t x);
    void printEdges(int64_t x);
    void printSLinks(int64_t x);
    void dfs(int64_t now, vector<int64_t> *v) const;
    void addSuffixLink(int64_t node);
    char getActiveEdge() const;
    bool walkDown(int64_t next);
    int64_t newNode(int64_t start, int64_t end, int64_t suffixIndex=-1);
    void addChar(char c);
    string edgeString(int64_t node) const;
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
     * @return 本字符串和r的公共最长子串
     */
    string findLongestCommon(string &r){
        r.append("\2");
        auto tempNodes=new int64_t[(this->size+r.size()<<1)+2];

        auto tempText = new char[this->size+r.size()];
        remainder=0;
        root = active_node = newNode(-1, -1);
        for (char ch : r)   addChar(ch);

        delete []tempNodes;
    }
    /**
     * 2022-04-14 21:32:35 GMT+8
     * 打印构造的树的结构, 以graphviz的DOT格式
     */
    void showInfo();
    /**
     * 2022-04-17 11:20:01 GMT+8
     * @return this->size
     */
    int64_t getSize();
};

#endif //_SUFFIXTRIE_H
