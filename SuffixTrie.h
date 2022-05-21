//
// Created by admin on 22-3-31 031.
//

#ifndef _SUFFIXTRIE_H
#define _SUFFIXTRIE_H

#include <map>
#include <utility>
#include <vector>

#define map std::map
#define min std::min
#define fstream std::fstream
#define string std::string
#define vector std::vector

class SuffixTrie {
    char *text{};
    int64_t root{}, size{}, position = -1,
            currentNode=0,  //当前到数组中哪个位置了, 后面插入新的节点就在该位置插入
            needSuffixLink{},
            remainder=0,    //当前剩余隐式包含的后缀的数量
            active_node{}, active_length{}, active_edge{};  //三元组
    const static int64_t presetMax = INT64_MAX;
    // 不需要创建 Edge 类. 有关边的信息存储在节点中. [start, end) int64_t 的区间指定边.
    struct SuffixNode {
        map<char, int64_t> next;
        SuffixNode()=default;
        SuffixNode(int64_t start, int64_t end, int64_t suffixIndex):start(start),end(end),suffixIndex(suffixIndex) {}
        int64_t edgeLength(int64_t pos) const {
            return min(end, pos + 1) - start;
        }
        int64_t start{}, end = presetMax,
                link=0, //标识该节点是否有后缀链接
                suffixIndex{};  //该叶子节点所代表的后缀是从原串中哪个位置开始的, 从0开始
    };
    SuffixNode *nodes= nullptr;

    void printLeaves(int64_t x, fstream &f);
    void printInternalNodes(int64_t x, fstream &f);
    void printEdges(int64_t x, fstream &f);
    void printSLinks(int64_t x, fstream &f);
    void dfs1(int64_t now, vector<int64_t> *v) const;
    void addSuffixLink(int64_t node);
    bool walkDown(int64_t next);
    int64_t newNode(int64_t start, int64_t end, int64_t suffixIndex=-1);
    void addChar(char c);
    string edgeString(int64_t node) const;
    static int64_t dfs2(SuffixTrie &st,int64_t now, int64_t cnt, int64_t first, int64_t &maxn, int64_t &tail);
public:
    /**
     * 禁用掉默认构造函数
     */
    explicit SuffixTrie()=delete;
    /**
     * 2022-04-03 11:59:22 GMT+8
     * @param s 要构建后缀树的字符串
     * @param final 结尾字符, 在对两个不同字符串查询公共部分时, 用来区分不同的字符串
     */
    explicit SuffixTrie(string s, const char& final='\1'):size(s.size()) {
        s+=final;
        nodes = new SuffixNode[(s.size()<<1)+2];
        text = new char[s.size()];
        remainder=0;
        root = active_node = newNode(-1, -1);
        for (char ch : s)   addChar(ch);
    }
    /**
     * 重新构造后缀树
     * @param s 要构建后缀树的字符串
     */
    void rebuild(string s);
    /**
     * 2022-03-31 18:07:04 GMT+8
     * @param t 查找的子串
     * @return 子串t对应的开始索引
     */
    vector<int64_t>* findSubstring(const string &t) const;
    /**
     * 2022-03-31 18:07:06 GMT+8
     * @param t 统计的子串
     * @return 子串t的出现次数
     */
    int64_t statisticSubstring(const string &t) const;
    /**
    * 2022-03-31 18:07:25 GMT+8
    * @param now 开始节点, 默认为根节点
    * @param cnt 从根节点到当前遍历到的节点所经过的字符串的最长长度
    * @param flag 递归过程中使用的标志
    * @return 重复出现的最长子串
    */
    string findMostRepeatSubstring(int64_t now=0,int64_t cnt=0,int64_t flag=0);
    /**
     * 2022-03-31 18:07:44 GMT+8
     * @param q 第一个串
     * @param r 第二个串
     * @return 公共最长子串
     */
    static string findLongestCommon(string &q, string &r);
    /**
     * 2022-04-14 21:32:35 GMT+8
     * 以图形化方式输出树的结构, 以graphviz的DOT格式
     */
    void displaySuffixTrie();
    /**
     * 2022-04-19 18:32:35 GMT+8
     * 输出节点信息, 节点的信息包括: start, end, suffixIndex
     */
    void outputNodeInfo() const;
    /**
     * 2022-04-17 11:20:01 GMT+8
     * @return this->size
     */
    int64_t getSize() const;
};

#undef map
#undef min
#undef fstream
#undef string
#undef vector

#endif //_SUFFIXTRIE_H
