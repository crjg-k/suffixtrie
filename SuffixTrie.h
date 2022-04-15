//
// Created by admin on 22-3-31 031.
//

#ifndef _SUFFIXTRIE_H
#define _SUFFIXTRIE_H
#include <string>
#include <map>
#include <vector>
#include <iostream>
using namespace std;

class SuffixTrie {
public:
    string text;
    int64_t root, position = -1,
            currentNode=0,
            needSuffixLink{},
            remainder=0,
            active_node, active_length{}, active_edge{};
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
public:
    explicit SuffixTrie()=default;
    explicit SuffixTrie(std::string s, const string& final="\1") {
        rebuild(s, final);
    }
    void addSuffixLink(int64_t node);
    char getActiveEdge();
    bool walkDown(int64_t next);
    int64_t newNode(int64_t start, int64_t end, int64_t suffixIndex=-1);
    void addChar(char c);
    /**
     * 2022-04-03 11:59:22 GMT+8
     * @param s 要构建后缀树的字符串
     * @param last 结尾字符, 在对两个不同字符串查询公共部分时, 用来区分不同的字符串
     */
    void rebuild(std::string &s, const string& final="\1");

    void dfs(int64_t now, vector<int64_t> *v) const;
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
    string findMostRepeatSubstring(){
        
    }
    /**
     * 2022-03-31 18:07:44 GMT+8
     * @param q 第一个串
     * @param r 第二个串
     * @return q和r的公共最长子串
     */
    static string findLongestCommon(string &q, string &r){

    }

    string edgeString(int64_t node);
    /**
     * 2022-04-14 21:32:35 GMT+8
     * 打印构造的树的结构
     */
    void showInfo();
    void printLeaves(int64_t x);
    void printInternalNodes(int64_t x);
    void printEdges(int64_t x);
    void printSLinks(int64_t x);
};

#endif //_SUFFIXTRIE_H
