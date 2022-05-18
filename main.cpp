#include <bits/stdc++.h>
#include "SuffixTrie.h"
#define es << ' '
#define el << '\n'
using namespace std;

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    string str1="xabxa";
    string str2="babxba";
    string str3="abaabbaaabbb";
//    string str4="aaaa";
//    SuffixTrie st(str4);
//    cout<<st.findMostRepeatSubstring();
//    auto p=st.findSubstring("a");
//    if(p!= nullptr)
//        for (const auto &item : *p)
//            cout<<item es;
//    else cout<<"404 not found" el;
    cout<<SuffixTrie::findLongestCommon(str1, str2);


    return 0;
}