#include <bits/stdc++.h>
#include "SuffixTrie.h"
#define es << ' '
#define el << '\n'
#define ll long long
using namespace std;
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    freopen(R"(data\input.txt)","r",stdin);
    fstream f(R"(data\myoutput.txt)",ios::out);
    ll n,opt;
    string str1,str2;
    SuffixTrie st("");
    cin>>n;
    vector<ll> *p;
    while (n--){
        cin>>opt;
        switch (opt) {
            case 1:
                cin>>str1;
                st.rebuild(str1);
                break;
            case 2:
                cin>>str1;
                p=st.findSubstring(str1);
                if(p!= nullptr){
                    sort(p->begin(),p->end());
                    for (int i = 0; i < p->size(); ++i)
                        f<<(*p)[i]<<" \n"[i==p->size()-1];
                }else f<<"404 not found.\n";
                break;
            case 3:
                cin>>str1;
                f<<st.statisticSubstring(str1) el;
                break;
            case 4:
                f<<st.findMostRepeatSubstring() el;
                break;
            case 5:
                cin>>str1>>str2;
                f<<SuffixTrie::findLongestCommon(str1,str2) el;
        }
    }
    f.flush();f.close();
    system(R"(fc data\output.txt data\myoutput.txt)");
    return 0;
}