#pragma GCC optimize("O3")
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define L(x) for(int x=1; x<=13; x++)
#define f first
#define s second
using namespace std; typedef vector<int> vi; typedef pair<int,int> pi;
pi op(pi x, pi y, int t)
{
    if (!x.s or !y.s) return pi(0,0);
    if (t==0) return pi(x.f*y.s+x.s*y.f,x.s*y.s);
    else if (t==1) {if (x.f*y.s < x.s*y.f) return pi(0,0); else return pi(x.f*y.s-x.s*y.f,x.s*y.s);}
    else if (t==2) return pi(x.f*y.f,x.s*y.s);
    else if (!y.f) return pi(0,0);
    else return pi(x.f*y.s,x.s*y.f);
}
int main(int argc, char* argv[])
{
    if (argc != 2) cerr << "Usage: ./163 [target]" << '\n', exit(0);
    int target = stoi(argv[1]); vi v = {4,0,0,0,0,4,4,4,4}; int T = 0, K = 0; ofstream fout;
    do
    {
        bool bad = 0; for (int i=0, t=1; i<9; i++, t+=1-2*(!v[i])) if (!t) bad = 1;
        if (bad) continue; vector<vi> cur(1,v), prv;
        for (int i=0; i<9; i++) if (!v[i])
        {
            prv = cur, cur.clear();
            for (vi V : prv) {vi tmp=V; for (int j=0; j<4; j++) tmp[i]=j, cur.push_back(tmp);}
        }
        for (int i=0, t=0; i<9; i++) if (v[i]==4) {for (int j=0; j<256; j++) cur[j][i] += t; t++;}
        for (vi V : cur) L(a)L(b)L(c)L(d)L(e)
        {
            if (!T--) T = 1e7, K++, fout.close(), fout.open("res/"+to_string(K));
            vector<pi> l; vi val = {a,b,c,d,e};
            for (int t : V) if (t<4) l[l.size()-2] = op(l[l.size()-2],l.back(),t), l.pop_back();
            else l.push_back(pi(val[t-4],1));
            if (l[0].s) if (l[0].f==l[0].s*target)
            {
                for (int t : V) if (t<4) fout << "+-*/"[t] << ' '; else fout << val[t-4] << ' ';
                fout << '\n';
            }
        }
    } while (next_permutation(v.begin(),v.end()));
}