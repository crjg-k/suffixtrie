#include <bits/stdc++.h>
#include "SuffixTrie.h"
#define es << ' '
#define el << '\n'
using namespace std;

int main(){
//    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    string str="aaabbbaaabbb$";
    SuffixTrie2 st(str.size());
    for (char i : str)
        st.addChar(i);

    st.printTree();

    return 0;
}