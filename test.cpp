#include <bits/stdc++.h>

#define es << ' '
#define el << '\n'
#define ll long long
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    map<char,ll> m;
    m.insert({'a',66});
    cout<<(*m.find('a')).second;
    m.insert({'a',77});
    cout<<(*m.find('a')).second;
    return 0;
}