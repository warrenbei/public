#include <iostream>
#include <ncurses.h>
#include <vector>
#include <string.h>
#include <random>
#include <chrono>
#include <thread>
#include <functional>
#include <cassert>
#include <sstream>
using namespace std;
typedef int32_t i32;
typedef uint64_t u64;
#define For(i,a,b) for(i32 i=(a);i<(b);i++)
template<class C> using vc=vector<C>;
using str=string;

namespace game
{
    const i32 mx=26; i32 n=10;
    u64 seed=chrono::steady_clock::now().time_since_epoch().count();
    enum vmode{normal,compressed,minimal} viewmode=normal;
    str level="";
    enum goal{psort,tsort,avl} task=tsort;
    char hyphens[2*mx];
    void init()
    {
        For(i,0,2*mx) hyphens[i]='-';
    }
    str get_goal(goal G)
    {
        switch(G)
        {
            case psort:
            {
                return "ps";
            }
            case tsort:
            {
                return "ts";
            }
            case avl:
            {
                return "avl";
            }
            default:
            {
                throw(0);
            }
        }
    }
}
class pos
{
public:
    typedef i32 c_type;
private:
    const i32 N;
    vc<i32> L,R,P;
    const vc<c_type> V;
    const game::goal task;
    void setc(i32 x, bool d, i32 y)
    {
        (d?R:L)[x]=y; if(y!=-1) P[y]=x;
    }
    bool dir(i32 x) const
    {
        return x==R[P[x]];
    }
    i32 k; vc<pair<i32,i32>> mv;
public:
    pos(i32 _N, vc<c_type> _V): N(_N), L(N,-1), R(N,-1), P(N,-1), V(_V), task(game::task)
    {
        vc<i32> v(N); For(i,0,N) v[i]=i;
        mt19937_64 rng(game::seed);
        For(i,0,N-1) swap(v[N-1-i],v[rng()%(N-i)]);
        i32 rt=v[0];
        For(i,1,N)
        {
            i32 t=rt;
            while(1)
            {
                bool x=rng()&1; i32 t1=(x?R:L)[t];
                if(t1==-1)
                {
                    setc(t,x,v[i]);
                    break;
                }
                else t=t1;
            }
        }
        k=rt;
    }
    void display() const
    {
        i32 dx,dy;
        switch(game::viewmode)
        {
            case game::normal:
            {
                dx=2,dy=1;
                break;
            }
            case game::compressed:
            {
                dx=2,dy=0;
                break;
            }
            case game::minimal:
            {
                dx=1,dy=0;
                break;
            }
        }
        function<void(i32,i32,i32)> smvaddch=[&](i32 x, i32 y, i32 c)
        {
            mvaddch(x+dx,y+dy,c);
        };
        function<void(i32,i32,char[],i32)> smvaddnstr=[&](i32 x, i32 y, char s[], i32 n)
        {
            mvaddnstr(x+dx,y+dy,s,n);
        };
        clear();
        vc<i32> D(N), X(N);
        function<i32(i32,i32,i32)> dfs=[&](i32 i, i32 x, i32 d)
        {
            D[i]=d;
            //dfs
            i32 ans=1;
            if(L[i]!=-1) ans+=dfs(L[i],x,d+1);
            X[i]=x+ans-1;
            if(R[i]!=-1) ans+=dfs(R[i],x+ans,d+1);
            //draw node
            switch(game::viewmode)
            {
                case game::normal:
                {
                    smvaddch(2+4*d,2+2*X[i],V[i]);
                    smvaddch(1+4*d,2+2*X[i],'|');
                    break;
                }
                case game::compressed:
                {
                    smvaddch(1+2*d,2+2*X[i],V[i]);
                    break;
                }
                case game::minimal:
                {
                    smvaddch(1+2*d,1+X[i],V[i]);
                    break;
                }
            }
            //check leaf
            if(L[i]==-1&&R[i]==-1) return ans;
            //draw branches
            switch(game::viewmode)
            {
                case game::normal:
                {
                    if(L[i]!=-1)
                        smvaddch(4+4*d,2+2*X[L[i]],'+'),
                        smvaddnstr(4+4*d,3+2*X[L[i]],game::hyphens,2*(X[i]-X[L[i]])-1);
                    if(R[i]!=-1)
                        smvaddch(4+4*d,2+2*X[R[i]],'+'),
                        smvaddnstr(4+4*d,3+2*X[i],game::hyphens,2*(X[R[i]]-X[i])-1);
                    break;
                }
                case game::compressed:
                {
                    if(L[i]!=-1)
                        smvaddch(2+2*d,2+2*X[L[i]],'+'),
                        smvaddnstr(2+2*d,3+2*X[L[i]],game::hyphens,2*(X[i]-X[L[i]])-1);
                    if(R[i]!=-1)
                        smvaddch(2+2*d,2+2*X[R[i]],'+'),
                        smvaddnstr(2+2*d,3+2*X[i],game::hyphens,2*(X[R[i]]-X[i])-1);
                    break;
                }
                case game::minimal:
                {
                    i32 xl=(L[i]==-1)?X[i]:X[L[i]], xr=(R[i]==-1)?X[i]:X[R[i]];
                    smvaddnstr(2+2*d,1+xl,game::hyphens,xr-xl+1);
                    break;
                }
            }
            //draw hook
            switch(game::viewmode)
            {
                case game::normal:
                {
                    smvaddch(3+4*d,2+2*X[i],'|');
                    smvaddch(4+4*d,2+2*X[i],'+');
                    break;
                }
                case game::compressed:
                {
                    smvaddch(2+2*d,2+2*X[i],'+');
                    break;
                }
                case game::minimal:
                {
                    break;
                }
            }
            return ans;
        };
        i32 r=-1; For(i,0,N) if(P[i]==-1) r=i; assert(r!=-1);
        //draw main tree
        dfs(r,0,0);
        //borders
        i32 md=0; For(i,0,N) md=max(md,D[i]);
        switch(game::viewmode)
        {
            case game::normal:
            {
                For(i,1,4+4*md) smvaddch(i,0,'.'), smvaddch(i,2*N+2,'.');
                For(i,0,N+2) smvaddch(0,2*i,'.'), smvaddch(4+4*md,2*i,'.');
                smvaddch(0,2+2*X[r],'|');
                break;
            }
            case game::compressed:
            {
                For(i,1,2+2*md) smvaddch(i,0,'.'), smvaddch(i,2*N+2,'.');
                For(i,0,N+2) smvaddch(0,2*i,'.'), smvaddch(2+2*md,2*i,'.');
                smvaddch(0,2+2*X[r],'|');
                break;
            }
            case game::minimal:
            {
                For(i,1,2+2*md) smvaddch(i,0,'.'), smvaddch(i,N+1,'.');
                For(i,0,N+2) smvaddch(0,i,'.'), smvaddch(2+2*md,i,'.');
                smvaddch(0,1+X[r],'|');
                break;
            }
        }
        //highlight cursor
        switch(game::viewmode)
        {
            case game::normal:
            {
                smvaddch(1+4*D[k],1+2*X[k],'\\');
                smvaddch(1+4*D[k],3+2*X[k],'/');
                smvaddch(2+4*D[k],1+2*X[k],'-');
                smvaddch(2+4*D[k],3+2*X[k],'-');
                smvaddch(3+4*D[k],1+2*X[k],'/');
                smvaddch(3+4*D[k],3+2*X[k],'\\');
                break;
            }
            case game::compressed:
            {
                smvaddch(1+2*D[k],1+2*X[k],'<');
                smvaddch(1+2*D[k],3+2*X[k],'>');
                break;
            }
            case game::minimal:
            {
                smvaddch(1+2*D[k],1+X[k],V[k]|A_STANDOUT);
                break;
            }
        }
        //stats
        switch(game::viewmode)
        {
            case game::normal:
            {
                mvaddstr(0,2,("Turn #"+to_string(mv.size())).c_str());
                break;
            }
            case game::compressed:
            {
                mvaddstr(0,1,("T="+to_string(mv.size())).c_str());
                break;
            }
            case game::minimal:
            {
                mvaddstr(0,0,(to_string(mv.size())).c_str());
                break;
            }
        }
    }
    bool chk_psort() const
    {
        For(i,0,N) if(L[i]!=-1 || P[i]!=i-1) return 0;
        return 1;
    }
    bool chk_sort() const
    {
        function<pair<i32,i32>(i32)> dfs=[&](i32 i)
        {
            if(i==-1) return pair<i32,i32>{-1,0};
            pair<i32,i32> x=dfs(L[i]), y=dfs(R[i]);
            if(L[i]!=-1 && i-L[i]!=x.second+1) throw(0);
            if(R[i]!=-1 && R[i]-i!=y.first +1) throw(0);
            return pair<i32,i32>{x.first+x.second+1,y.first+y.second+1};
        };
        i32 rt=-1; For(i,0,N) if(P[i]==-1) {rt=i; break;}
        try
        {
            dfs(rt);
            return 1;
        } catch(...) {return 0;}
    }
    bool chk_avl() const
    {
        return 0;
    }
    bool chk_win() const
    {
        switch(task)
        {
            case game::psort:
            {
                return chk_psort();
            }
            case game::tsort:
            {
                return chk_sort();
            }
            case game::avl:
            {
                return chk_avl();
            }
            default:
            {
                throw(0);
            }
        }
    }
    void mvl()
    {
        if(L[k]!=-1) k=L[k];
    }
    void mvr()
    {
        if(R[k]!=-1) k=R[k];
    }
    void mvp()
    {
        if(P[k]!=-1) k=P[k];
    }
    void rotl()
    {
        if(R[k]!=-1)
        {
            i32 t=R[k]; P[t]=P[k]; if(P[t]!=-1) setc(P[t],dir(k),t);
            setc(k,1,L[t]), setc(t,0,k);
            mv.push_back({k,0});
        }
    }
    void rotr()
    {
        if(L[k]!=-1)
        {
            i32 t=L[k]; P[t]=P[k]; if(P[t]!=-1) setc(P[t],dir(k),t);
            setc(k,0,R[t]), setc(t,1,k);
            mv.push_back({k,1});
        }
    }
    void rotp()
    {
        if(P[k]!=-1)
        {
            if(k==L[P[k]]) k=P[k], rotr(), k=P[k];
            else k=P[k], rotl(), k=P[k];
        }
    }
    void swapc()
    {
        if(L[k]==-1&&R[k]==-1) return;
        i32 l=L[k], r=R[k]; setc(k,0,r), setc(k,1,l);
        mv.push_back({k,2});
    }
    void undo()
    {
        if(!mv.empty())
        {
            pair<i32,i32> p=mv.back(); mv.pop_back();
            k=p.first;
            if(p.second==0||p.second==1) rotp(), mv.pop_back();
            if(p.second==2) swapc(), mv.pop_back();
        }
    }
    i32 mvcnt() const
    {
        return mv.size();
    }
    str show_sol() const
    {
        if(mv.empty()) return "-";
        stringstream S;
        for(pair<i32,i32> p:mv)
        {
            switch(p.second)
            {
                case 0:
                {
                    S<<"l"<<char(V[p.first])<<"-";
                    break;
                }
                case 1:
                {
                    S<<"r"<<char(V[p.first])<<"-";
                    break;
                }
                case 2:
                {
                    S<<"s"<<char(V[p.first])<<"-";
                    break;
                }
            }
        }
        str res; getline(S,res), res.pop_back(); return res;
    }
    str encode() const
    {
        stringstream S;
        S<<game::get_goal(task)<<","<<N<<":";
        For(i,0,N)
        {
            if(P[i]==-1) S<<'.';
            else S<<char('a'+P[i]+dir(i)*('A'-'a'));
        }
        str res; getline(S,res); return res;
    }
    void decode(str s)
    {
        stringstream S; S<<s;
        L=R=P=vc<i32>(N,-1);
        str t1,t2; getline(S,t1,','), getline(S,t2,':');
        if(t1!=game::get_goal(task) || stoi(t2)!=N) throw(0);
        For(i,0,N)
        {
            char c=S.get();
            if(c=='.') P[i]=-1;
            else if(c>='a'&&c<'a'+N) P[i]=c-'a', L[P[i]]=i;
            else if(c>='A'&&c<'A'+N) P[i]=c-'A', R[P[i]]=i;
        }
        vc<i32> vis(N,0);
        function<void(i32)> dfs=[&](i32 c)
        {
            if(c==-1) return;
            if(c<-1||c>=N) throw(0);
            if(vis[c]) throw(0); vis[c]=1;
            dfs(L[c]), dfs(R[c]);
        };
        k=-1;
        For(i,0,N) if(P[i]==-1)
        {
            if(k==-1) k=i;
            else throw(0);
        }
        dfs(k); For(i,0,N) if(!vis[i]) throw(0);
        mv.clear();
    }
};

const str help="\
Options:\n\
    -h      Help\n\
    -n=     Tree size (2-26)\n\
            Default = 10\n\
    -v=     Display mode\n\
            normal      N (default)\n\
            compressed  C\n\
            minimal     M\n\
    -s=     Set seed\n\
    -g=     Goal type\n\
            Sorted tree     ts (default)\n\
            Sorted R-path   ps\n\
            AVL             avl (unimplemented)\n\
    -S=     Set position\n\
";

signed main(i32 argc, char *argv[])
{
    For(i,1,argc)
    {
        if(argv[i]==str("-h")) cout<<help, exit(0);
        i32 l=strlen(argv[i]); bool done=0;
        if(!done && l>=4)
        {
            bool bad=0;
            For(j,0,3) if(argv[i][j]!="-n="[j]) bad=1;
            if(!bad) try
            {
                game::n=stoi(argv[i]+3);
                if(game::n<2||game::n>game::mx) throw(0);
                done=1;
            } catch(...){}
        }
        if(!done && l==4)
        {
            bool bad=0;
            For(j,0,3) if(argv[i][j]!="-v="[j]) bad=1;
            if(!bad) try
            {
                bad=1;
                if(argv[i][3]=='N') bad=0, game::viewmode=game::normal;
                if(argv[i][3]=='C') bad=0, game::viewmode=game::compressed;
                if(argv[i][3]=='M') bad=0, game::viewmode=game::minimal;
                if(bad) throw(0);
                done=1;
            } catch(...){}
        }
        if(!done && l>=4)
        {
            bool bad=0;
            For(j,0,3) if(argv[i][j]!="-s="[j]) bad=1;
            if(!bad) try
            {
                game::seed=stoull(argv[i]+3);
                done=1;
            } catch(...){}
        }
        if(!done && l>=4)
        {
            bool bad=0;
            For(j,0,3) if(argv[i][j]!="-S="[j]) bad=1;
            if(!bad) try
            {
                game::level=argv[i]+3;
                done=1;
            } catch(...){}
        }
        if(!done && l>=4)
        {
            bool bad=0;
            For(j,0,3) if(argv[i][j]!="-g="[j]) bad=1;
            if(!bad) try
            {
                bad=1;
                if(argv[i]+3==game::get_goal(game::psort)) bad=0, game::task=game::psort;
                if(argv[i]+3==game::get_goal(game::tsort)) bad=0, game::task=game::tsort;
                if(argv[i]+3==game::get_goal(game::avl)) bad=0, game::task=game::avl;
                done=1;
            } catch(...){}
        }
        if(!done) cout<<"Unknown option "<<argv[i]<<'\n', exit(0);
    }
    game::init();
    pos P(game::n,
        [&](i32 n)
        {
            vc<pos::c_type> v(n); For(i,0,n) v[i]=('A'+i)|A_BOLD;
            return v;
        }(game::n));
    if(!game::level.empty())
    {
        try
        {
            P.decode(game::level);
        } catch(...){cout<<"Invalid position\n", exit(0);}
    }
    pos P_i=P;
    initscr(), cbreak(), noecho(), keypad(stdscr,1), curs_set(0), timeout(-1);
    while(1)
    {
        if(P.chk_win())
        {
            endwin();
            cout<<"Success!\n";
            if(game::level=="") cout<<"Seed    "<<game::seed<<'\n';
            cout<<"Size    "<<game::n<<'\n';
            cout<<"Moves   "<<P.mvcnt()<<"\n";
            cout<<P_i.encode()<<'\n';
            cout<<P.show_sol()<<'\n';
            exit(0);
        }
        P.display();
        this_thread::sleep_for(chrono::milliseconds(100));
        i32 c=getch(); flushinp();
        if(c=='Q')          endwin(), exit(0);
        if(c==KEY_UP)       P.mvp();
        if(c==KEY_LEFT)     P.mvl();
        if(c==KEY_RIGHT)    P.mvr();
        if(c=='w')          P.rotp();
        if(c=='a')          P.rotl();
        if(c=='d')          P.rotr();
        if(c=='s')          P.swapc();
        if(c=='z')          P.undo();
    }
}
