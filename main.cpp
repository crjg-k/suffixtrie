#include <bits/stdc++.h>
#include "SuffixTrie.h"
#define es << ' '
#define el << '\n'
using namespace std;

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    string str1="abaaaabbaaaabbb";
    string str2="abaaababaabbb";
    cout<<SuffixTrie::findLongestCommon(str1,str2) el;
//    SuffixTrie st(str);
//    cout<<st.findMostRepeatSubstring() el;
//    auto p=st.findSubstring("ab");
//    for (const auto &item : *p)
//        cout<<str.substr(item) el;
//    cout<<st.statisticSubstring("ab") el;
//    for(int i=0;i<st.currentNode;++i){
//        SuffixTrie::SuffixNode &node = st.nodes[i];
//        cout<<node.start es<<(node.end==INT64_MAX?-1:node.end) es;
//        cout<<node.suffixIndex el;
//    }

    return 0;
}