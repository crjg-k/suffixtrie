//
// Created by admin on 22-4-14 014.
//

#include "SuffixTrie.h"

#include <fstream>
#include <iostream>

#define min std::min
#define fstream std::fstream
#define cout std::cout
#define string std::string
#define to_string std::to_string
#define vector std::vector
#define copy std::copy

void SuffixTrie::displaySuffixTrie() {
    fstream f("st.txt", std::ios::out);
    f<<"digraph {"<<'\n';
    f<<"\trankdir = LR;"<<'\n';
    f<<"\tedge [arrowsize=0.4,fontsize=10]"<<'\n';
    f<<"\tnode0 [label=\"\",style=filled,fillcolor=lightgrey,shape=circle,width=.1,height=.1];"<<'\n';
    f<<"//------leaves------"<<'\n';
    printLeaves(root,f);
    f<<"//------internal nodes------"<<'\n';
    printInternalNodes(root,f);
    f<<"//------edges------"<<'\n';
    printEdges(root,f);
    f<<"//------suffix links------"<<'\n';
    printSLinks(root,f);
    f<<"}"<<'\n';
    f.flush();f.close();
    system("dot -Tsvg st.txt > output.svg");
    system("output.svg");
}

void SuffixTrie::printLeaves(int64_t x, fstream &f) {
    if (nodes[x].next.empty())
        f<<"\tnode"+ to_string(x)+" [label=\""+ to_string(nodes[x].suffixIndex)+"\",shape=point]\n";
    else for (auto child : nodes[x].next)
            printLeaves(child.second, f);
}

void SuffixTrie::printInternalNodes(int64_t x, fstream &f) {
    if (x != root && !nodes[x].next.empty())
        f<<"\tnode"+ to_string(x)+" [label=\"\",style=filled,fillcolor=lightgrey,shape=circle,width=.07,height=.07]\n";
    for (auto child : nodes[x].next)
        printInternalNodes(child.second, f);
}

void SuffixTrie::printEdges(int64_t x, fstream &f) {
    for (auto child : nodes[x].next) {
        f<<"\tnode"+ to_string(x)+" -> node"+to_string(child.second)+" [label=\""+edgeString(child.second)+"\"]"<<'\n';
        printEdges(child.second, f);
    }
}

void SuffixTrie::printSLinks(int64_t x, fstream &f) {
    if (nodes[x].link > 0)
        f<<"\tnode"+to_string(x)+" -> node"+ to_string(nodes[x].link)+" [label=\"\",weight=1,style=dotted]"<<'\n';
    for (auto child : nodes[x].next)
        printSLinks(child.second, f);
}

string SuffixTrie::edgeString(int64_t node) const {
    string str;
    for (int64_t i = nodes[node].start; i < min(position + 1, nodes[node].end); ++i)
        if(text[i]<32){
            str+=("\\\\"+ to_string(text[i]));
            break;
        }
        else str+=text[i];
    return str;
}

void SuffixTrie::outputNodeInfo() const{
    for(int i=0;i<currentNode;++i){
        SuffixTrie::SuffixNode &node = nodes[i];
        cout<<node.start<<' '<<(node.end==INT64_MAX?-1:node.end)<<' '<<node.suffixIndex<<'\n';
    }
}

void SuffixTrie::addSuffixLink(int64_t node) {
    if (needSuffixLink > 0) nodes[needSuffixLink].link = node;
    needSuffixLink = node;
}

bool SuffixTrie::walkDown(int64_t next) {   //判断当前节点所代表的子串是否被"走完"然后active_node"行进到"下一个节点
    if (active_length >= nodes[next].edgeLength(position)) {
        active_edge += nodes[next].edgeLength(position);
        active_length -= nodes[next].edgeLength(position);
        active_node = next;
        return 1;
    }
    return 0;
}

int64_t SuffixTrie::newNode(int64_t start, int64_t end, int64_t suffixIndex) {
    nodes[currentNode] = *new SuffixNode(start, end, suffixIndex);
    return currentNode++;
}

void SuffixTrie::addChar(char c){
    text[++position] = c;
    needSuffixLink = -1;++remainder;
    while(remainder > 0) {
        if (active_length == 0) active_edge = position;
        if (nodes[active_node].next.find(text[active_edge])==nodes[active_node].next.end()){
            //说明当前节点的字节中没有以字符c开头的边, 直接新增节点
            int64_t leaf = newNode(position, presetMax, position-remainder+1);
            nodes[active_node].next[text[active_edge]]=leaf;
            addSuffixLink(active_node); //rule 3
        } else {
            int64_t next = nodes[active_node].next[text[active_edge]];
            if (walkDown(next)) continue;
            if (text[nodes[next].start + active_length] == c) {   //被隐式包含了, 什么都不做
                active_length++;
                break;
            }
            //节点分裂
            int64_t split = newNode(nodes[next].start, nodes[next].start + active_length);
            nodes[active_node].next[text[active_edge]]=split;
            int64_t leaf = newNode(position, presetMax, position-remainder+1);
            nodes[split].next[c]=leaf;
            nodes[next].start += active_length;
            nodes[split].next[text[nodes[next].start]]=next;
            addSuffixLink(split); //rule 3
        }
        --remainder;

        if (active_node == root and active_length > 0) {  //rule 1
            active_length--;
            active_edge = position - remainder + 1;
        } else active_node = nodes[active_node].link > 0 ? nodes[active_node].link : root; //rule 2
    }
}

void SuffixTrie::rebuild(string s){
    SuffixTrie st(std::move(s));
    *this=st;
}

void SuffixTrie::dfs1(int64_t now, vector<int64_t> *v) const {
    if(nodes[now].next.empty()) v->emplace_back(nodes[now].suffixIndex);
    else for (const auto &item : nodes[now].next)
            dfs1(item.second,v);
}

vector<int64_t>* SuffixTrie::findSubstring(const string &t) const{
    int64_t step=0, now=root, cnt=0, start, next, end;bool flag;
    while (1){
        if(step==t.size()){ //从这里开始类dfs搜索
            auto* v=new vector<int64_t>;
            dfs1(now,v);
            return v;
        }
        //按照在字典树中查找子串的规则进行查找
        flag=1;
        for (const auto &item : nodes[now].next)
            if(item.first==t[step]){
                flag=0;
                start=nodes[next=item.second].start;
                end=nodes[item.second].end;
                break;
            }
        while (!flag and text[start+cnt]==t[step]){
            ++cnt;++step;
            if(step==t.size() or cnt>=end-start) {
                now=next;
                cnt=0;break;
            }
        }
        if(flag)    return nullptr;
    }
}

int64_t SuffixTrie::statisticSubstring(const string &t) const{
    auto res=findSubstring(t);
    return res== nullptr?0:res->size();
}

string SuffixTrie::findMostRepeatSubstring(int64_t now,int64_t cnt,int64_t flag){
    static string str;str.clear();
    static int64_t tail, maxn=INT64_MIN;
    for (const auto &item : nodes[now].next)
        if(nodes[item.second].end!=presetMax){
            cnt+=nodes[item.second].end-nodes[item.second].start;
            findMostRepeatSubstring(item.second,cnt,1); //dfs
            cnt-=nodes[item.second].end-nodes[item.second].start;
        }
        else if(cnt>maxn){  //记录当前找到的最长的
            maxn=cnt;
            tail=nodes[now].end;
        }
    if(!flag){  //按照dfs得到的参数拼接字符串并返回
        for(int64_t i=0;i<maxn;++i)
            str+=text[i+tail-maxn];
        maxn=INT64_MIN;
    }
    return str;
}

int64_t SuffixTrie::dfs2(SuffixTrie &st,int64_t now, int64_t cnt, int64_t first, int64_t &maxn, int64_t &tail) {
    bool is1=0,is2=0,is3=0; //用来标记当前节点的所有后代叶子节点的组合情况的状态
    if(st.nodes[now].next.empty()){ //叶子节点的状态可以简单得到
        if(st.nodes[now].start>first)   return 2;
        else return 1;
    }
    for (const auto &item : st.nodes[now].next){
        auto res=dfs2(st, item.second, cnt+st.nodes[item.second].end-st.nodes[item.second].start, first, maxn, tail);
        if(res==1)  is1=1;
        if(res==2)  is2=1;
        if(res==3 or (is1 and is2)) is3=1;  //通过组合得到非叶子节点的状态
        if(is3 and cnt>maxn){  //记录当前找到的最长的
            maxn=cnt;
            tail=st.nodes[now].end;
        }
    }
    if(is3) return 3;   //状态回溯
    if(is2) return 2;   //状态回溯
    return 1;   //状态回溯
}

string SuffixTrie::findLongestCommon(string &q, string &r){
    int64_t maxn=INT64_MIN, tail;
    r+='\2';
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
    //以上过程均为构造广义后缀树
    dfs2(st,0,0,q.size(),maxn,tail);    //dfs找最长的
    string temp;
    for(int64_t i=0;i<maxn;++i)  //按照dfs得到的参数拼接字符串并返回
        temp+=st.text[i+tail-maxn];
    delete []tempNodes;delete []tempText;
    return temp;
}

int64_t SuffixTrie::getSize() const{ return size; }

#undef min
#undef fstream
#undef cout
#undef string
#undef to_string
#undef vector
#undef copy
