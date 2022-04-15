#include <bits/stdc++.h>
#include "SuffixTrie.h"
#define es << ' '
#define el << '\n'
using namespace std;

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    string str="abbcdbfsssb";
    SuffixTrie st(str);
    auto p=st.findSubstring("b");
    for (const auto &item : *p)
        cout<<str.substr(item) el;
    cout<<st.statisticSubstring("b") el;
//    st.showInfo();
//    for(int i=0;i<st.currentNode;++i){
//        SuffixTrie::SuffixNode &node = st.nodes[i];
//        cout<<node.start es<<(node.end==INT64_MAX?-1:node.end) es;
//        cout<<node.suffixIndex el;
//    }

    return 0;
}