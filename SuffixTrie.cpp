//
// Created by admin on 22-4-14 014.
//

#include "SuffixTrie.h"

#include <fstream>

void SuffixTrie::displaySuffixTrie() {
    fstream f("st.txt", ios::out);
    f<<"digraph {"<<'\n';
    f<<"\trankdir = LR;"<<'\n';
    f<<"\tedge [arrowsize=0.4,fontsize=10]"<<'\n';
    f<<"\tnode0 [label=\"\",style=filled,fillcolor=lightgrey,"
          "shape=circle,width=.1,height=.1];"<<'\n';
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

void SuffixTrie::addSuffixLink(int64_t node) {
    if (needSuffixLink > 0)
        nodes[needSuffixLink].link = node;
    needSuffixLink = node;
}

char SuffixTrie::getActiveEdge() const {
    return text[active_edge];
}

bool SuffixTrie::walkDown(int64_t next) {
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
    needSuffixLink = -1;
    remainder++;
    while(remainder > 0) {
        if (active_length == 0)
            active_edge = position;
        if (nodes[active_node].next.find(getActiveEdge())==nodes[active_node].next.end()){
            //说明当前节点的字节中没有以字符c开头的边, 直接新增节点
            int64_t leaf = newNode(position, presetMax, position-remainder+1);
            nodes[active_node].next[getActiveEdge()]=leaf;
            addSuffixLink(active_node); //rule 2
        } else {
            int64_t next = nodes[active_node].next[getActiveEdge()];
            if (walkDown(next)) continue;
            if (text[nodes[next].start + active_length] == c) {   //被隐式包含了, 什么都不做
                active_length++;
                addSuffixLink(active_node);
                break;
            }
            //节点分裂
            int64_t split = newNode(nodes[next].start, nodes[next].start + active_length);
            nodes[active_node].next[getActiveEdge()]=split;
            int64_t leaf = newNode(position, presetMax, position-remainder+1);
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

void SuffixTrie::dfs(int64_t now, vector<int64_t> *v) const {
    if(nodes[now].next.empty())
        v->push_back(nodes[now].suffixIndex);
    else for (const auto &item : nodes[now].next)
            dfs(item.second,v);
}

vector<int64_t>* SuffixTrie::findSubstring(const std::string &t) const{
    int64_t step=0, now=root, cnt=0;bool flag=0;
    while (1){
        if(step==t.size()){
            //从这里开始类dfs搜索
            auto* v=new vector<int64_t>;
            dfs(now,v);
            return v;
        }
        flag=1;
        for (const auto &item : nodes[now].next){
            if(item.first==t[step]){
                flag=0;
                ++cnt;
                if(step==t.size()-1 or cnt>=nodes[item.second].end-nodes[item.second].start) {
                    now=item.second;
                    cnt=0;
                }
                break;
            }
        }
        ++step;
        if(flag)    return nullptr;
    }
}

int64_t SuffixTrie::statisticSubstring(const string &t) const{
    auto res=findSubstring(t);
    return res== nullptr?0:res->size();
}

string SuffixTrie::findMostRepeatSubstring(int64_t now,int64_t cnt,int64_t flag){
    string str;
    static int64_t tail,maxn=INT64_MIN;
    for (const auto &item : nodes[now].next)
        if(nodes[item.second].end!=presetMax){
            cnt+=nodes[item.second].end-nodes[item.second].start;
            findMostRepeatSubstring(item.second,cnt,1);
            cnt-=nodes[item.second].end-nodes[item.second].start;
        }
        else if(cnt>maxn){
            maxn=cnt;
            tail=nodes[now].end;
        }
    if(!flag)
        for(int64_t i=0;i<maxn;++i)
            str+=text[i+tail-maxn];
    return str;
}

int64_t SuffixTrie::getSize() const{
    return size;
}

void SuffixTrie::outputNodeInfo() const{
    for(int i=0;i<currentNode;++i){
        SuffixTrie::SuffixNode &node = nodes[i];
        cout<<node.start<<' '<<(node.end==INT64_MAX?-1:node.end)<<' ';
        cout<<node.suffixIndex<<'\n';
    }
}

string SuffixTrie::findLongestCommon(int64_t now,int64_t cnt,int64_t flag){

}