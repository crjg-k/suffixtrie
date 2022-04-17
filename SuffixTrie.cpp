//
// Created by admin on 22-4-14 014.
//

#include "SuffixTrie.h"

void SuffixTrie::showInfo() {
    cout<<"SuffixTrie {"<<'\n';
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

void SuffixTrie::printLeaves(int64_t x) {
    if (nodes[x].next.empty())
        cout<<"\tnode"+ to_string(x)+" [suffixIndex="+ to_string(nodes[x].suffixIndex)+"]"<<'\n';
    else for (auto child : nodes[x].next)
            printLeaves(child.second);
}

void SuffixTrie::printInternalNodes(int64_t x) {
    if (x != root && !nodes[x].next.empty())
        cout<<"\tnode"+ to_string(x)+" [label=\"\"]"<<'\n';
    for (auto child : nodes[x].next)
        printInternalNodes(child.second);
}

void SuffixTrie::printEdges(int64_t x) {
    for (auto child : nodes[x].next) {
        cout<<"\tnode"+ to_string(x)+" -> node"+to_string(child.second)+" [label=\""+edgeString(child.second)+"\"]"<<'\n';
        printEdges(child.second);
    }
}

void SuffixTrie::printSLinks(int64_t x) {
    if (nodes[x].link > 0)
        cout<<"\tnode"+to_string(x)+" -> node"+ to_string(nodes[x].link)+" [label=\"\"]"<<'\n';
    for (auto child : nodes[x].next)
        printSLinks(child.second);
}

string SuffixTrie::edgeString(int64_t node) const {
    string str;
    for (int64_t i = nodes[node].start; i < min(position + 1, nodes[node].end); ++i)
        str+=text[i];
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
        return true;
    }
    return false;
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
            //直接新增节点
            int64_t leaf = newNode(position, presetMax, position-remainder+1);
            nodes[active_node].next[getActiveEdge()]=leaf;
            addSuffixLink(active_node); //rule 2
        } else {
            int64_t next = nodes[active_node].next[getActiveEdge()];
            if (walkDown(next)) continue;
            if (text[nodes[next].start + active_length] == c) {
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
    return findSubstring(t)->size();
}

string SuffixTrie::findMostRepeatSubstring(int64_t now=0,int64_t cnt=0,int64_t flag=0){
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

int64_t SuffixTrie::getSize(){
    return size;
}