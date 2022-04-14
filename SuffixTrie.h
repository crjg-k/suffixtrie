//
// Created by admin on 22-3-31 031.
//

#ifndef _SUFFIXTRIE_H
#define _SUFFIXTRIE_H
#include <string>
#include <cstring>
#include <map>
#include <iostream>
using namespace std;

class SuffixTrie {
    char *text;
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
            [start; end) int64_t interval specifies the edge,
            by which the node is connected to its parent node.
        */
        map<char, int64_t> next;
        SuffixNode()=default;
        SuffixNode(int64_t start, int64_t end):start(start),end(end) {}
        int64_t edgeLength(int64_t position) const {
            return min(end, position + 1) - start;
        }
        int64_t start{}, end = presetMax, link=0;
    };
    SuffixNode *nodes= nullptr;
public:
    explicit SuffixTrie(int64_t length) {
        nodes = new SuffixNode[2* length + 2];
        text = new char[length];
        memset(text, 0, sizeof(char)*length);
        remainder=0;
        root = active_node = newNode(-1, -1);
    }

    void addSuffixLink(int64_t node) {
        if (needSuffixLink > 0)
            nodes[needSuffixLink].link = node;
        needSuffixLink = node;
    }

    char getActiveEdge() {
        return text[active_edge];
    }

    bool walkDown(int64_t next) {
        if (active_length >= nodes[next].edgeLength(position)) {
            active_edge += nodes[next].edgeLength(position);
            active_length -= nodes[next].edgeLength(position);
            active_node = next;
            return true;
        }
        return false;
    }

    int64_t newNode(int64_t start, int64_t end) {
        nodes[++currentNode] = *new SuffixNode(start, end);
        return currentNode;
    }

    void addChar(char c){
        text[++position] = c;
        needSuffixLink = -1;
        remainder++;
        while(remainder > 0) {
            if (active_length == 0) active_edge = position;
            if (nodes[active_node].next.find(getActiveEdge())==nodes[active_node].next.end()){
                int64_t leaf = newNode(position, presetMax);
                nodes[active_node].next[getActiveEdge()]=leaf;
                addSuffixLink(active_node); //rule 2
            } else {
                int64_t next = nodes[active_node].next[getActiveEdge()];
                if (walkDown(next)) continue;   //observation 2
                if (text[nodes[next].start + active_length] == c) { //observation 1
                    active_length++;
                    addSuffixLink(active_node); // observation 3
                    break;
                }
                int64_t split = newNode(nodes[next].start, nodes[next].start + active_length);
                nodes[active_node].next[getActiveEdge()]=split;
                int64_t leaf = newNode(position, presetMax);
                nodes[split].next[c]=leaf;
                nodes[next].start += active_length;
                nodes[split].next[text[nodes[next].start]]=next;
                addSuffixLink(split); //rule 2
            }
            remainder--;

            if (active_node == root && active_length > 0) {  //rule 1
                active_length--;
                active_edge = position - remainder + 1;
            } else active_node = nodes[active_node].link > 0 ? nodes[active_node].link : root; //rule 3
        }
    }
    /**
     * 2022-04-03 11:59:22 GMT+8
     * @param s 要构建后缀树的字符串
     * @param last 结尾字符, 在对两个不同字符串查询公共部分时, 用来区分不同的字符串
     */
    void rebuild(const std::string &s, const char last='\0'){

    }
    /**
     * 2022-03-31 18:07:04 GMT+8
     * @param t 查找的子串
     * @return 子串t对应的开始索引
     */
    int64_t findSubstring(std::string &t){

    }
    /**
     * 2022-03-31 18:07:06 GMT+8
     * @param t 统计的子串
     * @return 子串t的出现次数
     */
    int64_t statisticSubstring(string &t){

    }
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

    string edgeString(int64_t node) {
        string str;
        for (int64_t i = nodes[node].start; i < min(position + 1, nodes[node].end); ++i){
            str+=text[i];
        }
        return str;
    }
    /**
     * 2022-04-02 09:52:53 GMT+8
     * 打印构造的树的结构
     */
    void printTree() {
        cout<<"digraph {"<<'\n';
        cout<<"//------leaves------"<<'\n';
        printLeaves(root);
        cout<<"//------internal nodes------"<<'\n';
        printInternalNodes(root);
        cout<<"//------edges------"<<'\n';
        printEdges(root);
        cout<<"//------suffix links------"<<'\n';
        printSLinks(root);
        cout<<"}"<<'\n';
    }

    void printLeaves(int64_t x) {
        if (nodes[x].next.empty())
            cout<<"\tnode"+ to_string(x)+" [label=\"\",shape=point]"<<'\n';
        else for (auto child : nodes[x].next)
            printLeaves(child.second);
    }
    void printInternalNodes(int64_t x) {
        if (x != root && !nodes[x].next.empty())
            cout<<"\tnode"+ to_string(x)+" [label=\"\",style=filled,fillcolor=lightgrey,shape=circle,width=.07,height=.07]"<<'\n';
        for (auto child : nodes[x].next)
            printInternalNodes(child.second);
    }

    void printEdges(int64_t x) {
        for (auto child : nodes[x].next) {
            cout<<"\tnode"+ to_string(x)+" -> node"+to_string(child.second)+" [label=\""+edgeString(child.second)+"\",weight=3]"<<'\n';
            printEdges(child.second);
        }
    }

    void printSLinks(int64_t x) {
        if (nodes[x].link > 0)
            cout<<"\tnode"+to_string(x)+" -> node"+ to_string(nodes[x].link)+" [label=\"\",weight=1,style=dotted]"<<'\n';
        for (auto child : nodes[x].next)
            printSLinks(child.second);
    }
};

#endif //_SUFFIXTRIE_H
