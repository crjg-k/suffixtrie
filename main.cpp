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
    string str4="aabccabc";

    SuffixTrie st1(str3);
    cout<<"st1: \n";
    cout<<st1.findMostRepeatSubstring() el;
    auto p=st1.findSubstring("aa");
    if(p!= nullptr){
        for (const auto &item : *p)
            cout<<item es;
        cout el;
    }else cout<<"404 not found" el;

    SuffixTrie st2(str4);
    cout<<"st2: \n";
    cout<<st2.findMostRepeatSubstring() el;
    p=st2.findSubstring("abc");
    if(p!= nullptr){
        for (const auto &item : *p)
            cout<<item es;
        cout el;
    }else cout<<"404 not found" el;

    cout<<"str1 and str2: \n";
    cout<<SuffixTrie::findLongestCommon(str1, str2) el;

    return 0;
}