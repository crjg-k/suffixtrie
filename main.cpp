#include <bits/stdc++.h>
#include "SuffixTrie.h"
#define es << ' '
#define el << '\n'
using namespace std;

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    string str1="xabxa";
    string str2="babxba";
    string str3="abracadabra";
    string str4="cabccabccabc";
    string str5="banana";
    string str6="edfabcedfzabc";

    SuffixTrie st1(str3);
    st1.displaySuffixTrie();
    cout<<"st1: \n";
    cout<<st1.findMostRepeatSubstring() el;
    auto p=st1.findSubstring("ab");
    if(p!= nullptr){
        for (const auto &item : *p)
            cout<<item es;
        cout el;
    }else cout<<"404 not found." el;

    SuffixTrie st2(str4);
//    st2.displaySuffixTrie();
    cout<<"st2: \n";
    cout<<st2.findMostRepeatSubstring() el;
    p=st2.findSubstring("cca");
    if(p!= nullptr){
        for (const auto &item : *p)
            cout<<item es;
        cout el;
    }else cout<<"404 not found." el;

    SuffixTrie st3(str5);
//    st3.displaySuffixTrie();
    cout<<"st3: \n";
    cout<<st3.findMostRepeatSubstring() el;
    p=st3.findSubstring(" ");
    if(p!= nullptr){
        for (const auto &item : *p)
            cout<<item es;
        cout el;
    }else cout<<"404 not found." el;

    SuffixTrie st4(str6);
//    st4.displaySuffixTrie();
    cout<<"st4: \n";
    cout<<st4.findMostRepeatSubstring() el;

    cout<<"str1 and str2: \n";
    cout<<SuffixTrie::findLongestCommon(str1, str2) el;

    return 0;
}