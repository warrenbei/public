#include <iostream>
#include <ncurses.h>
#include <assert.h>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>
using namespace std;
typedef int32_t i32;
typedef uint64_t u64;
const string help="\
Usage: ./... [c/m]\n\
Cat: X    Mouse: O\n\
Arrow keys to move\n\
q to quit\n\
";
signed main(i32 argc, char *argv[]) {
    assert(sizeof(u64)==8);
    if(argc!=2 || (argv[1][0]!='c' && argv[1][0]!='m')) cout<<help, exit(0);
    auto utos=[&](u64 n) {
        stringstream S; S<<setw(16)<<setfill('0')<<setbase(16)<<n;
        string s; S>>s; return s;
    };
    auto stou=[&](string s) {
        return stoull(s,0,16);
    };
    auto hash=[&](u64 n) {
        return n*314159265358979323ULL;
    };
    auto invhash=[&](u64 n) {
        return n*13408368749021129267ULL;
    };
    auto encode=[&](i32 x, i32 (&v)[32], string &s) {
        u64 n=0; i32 t=x;
        for(i32 i=0; i<32; i++)
            n|=(u64(abs(v[i]-t)==1)<<(2*i))|(u64(v[i]<t)<<(2*i+1)),
            t=v[i];
        s=utos(hash(n));
    };
    auto decode=[&](i32 x, i32 (&v)[32], string &s) {
        u64 n=invhash(stou(s)); i32 t=x;
        for(i32 i=0; i<32; i++)
            v[i]=t+(((n>>(2*i))&1)?1:4)*(1-2*((n>>(2*i+1))&1)),
            t=v[i];
    };
    assert(invhash(hash(1))==1);
    i32 t=0;
    auto board=[&]() {
        clear();
        mvaddstr(0,3,to_string(t/2).c_str());
        mvaddstr(1,1,"+-+-+-+-+"),
        mvaddstr(2,1,"|.|.|.|.|"),
        mvaddstr(3,1,"+-+-+-+-+"),
        mvaddstr(4,1,"|.|.|.|.|"),
        mvaddstr(5,1,"+-+-+-+-+"),
        mvaddstr(6,1,"|.|.|.|.|"),
        mvaddstr(7,1,"+-+-+-+-+"),
        mvaddstr(8,1,"|.|.|.|.|"),
        mvaddstr(9,1,"+-+-+-+-+");
    };
    auto initwin=[&]() {
        initscr(), cbreak(), noecho(), keypad(stdscr,1), timeout(-1), curs_set(0);
    };
    auto copythis=[&](string s) {
        clear();
        mvaddstr(0,2,"Copy this and");
        mvaddstr(1,2,"press space:");
        mvaddstr(2,1,s.c_str());
        refresh();
        i32 c=0; while(c!=' ') c=getch();
    };
    if(argv[1][0]=='c') {
        u64 seed;
        while(1) {
            try {
                cout<<": ";
                string s; getline(cin,s);
                seed=stou(s);
                break;
            }
            catch(...) {cout<<"Invalid\n";}
        }
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        i32 cat=rng()&15, mouse=cat; while(mouse==cat) mouse=rng()&15;
        initwin();
        rng.seed(seed);
        copythis(utos(hash((rng()>>8)+(u64(cat)<<60)+(u64(mouse)<<56))));
        i32 v[32], cstart=cat;
        while(1) {
            board();
            mvaddch(2+2*(cat/4),2+2*(cat%4),'X');
            refresh();
            this_thread::sleep_for(chrono::milliseconds(200));
            i32 c=getch(); bool x=0;
            if(c==KEY_LEFT  &&  cat%4!=0)   cat-=1, x=1;
            if(c==KEY_RIGHT &&  cat%4!=3)   cat+=1, x=1;
            if(c==KEY_UP    &&  cat/4!=0)   cat-=4, x=1;
            if(c==KEY_DOWN  &&  cat/4!=3)   cat+=4, x=1;
            if(c=='q') goto endc;
            if(x) {
                v[t%32]=cat, t++;
                if(!(t%32)) {
                    string s; encode(cstart,v,s), cstart=cat;
                    copythis(s);
                }
            }
        }
        endc:
        endwin();
        i32 claim;
        cout<<"Claimed score\n";
        while(1) {
            try {
                cout<<": ";
                string s; getline(cin,s);
                claim=stoi(s);
                break;
            }
            catch(...) {cout<<"Invalid\n";}
        }
        cout<<"Verify claim\n";
        while(1) {
            try {
                cout<<": ";
                string s; getline(cin,s);
                if(invhash(stou(s))==(seed^claim)) break;
            }
            catch(...) {cout<<"Invalid\n";}
        }
        cout<<"Result is valid\n";
        return 0;
    }
    else if(argv[1][0]=='m') {
        initwin();
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        u64 seed=rng();
        copythis(utos(seed));
        i32 cat=-1, mouse=-1;
        endwin();
        while(1) {
            try {
                cout<<": ";
                string s; getline(cin,s);
                u64 n=invhash(stou(s));
                cat=n>>60, mouse=(n>>56)&15;
                rng.seed(seed);
                if(((n<<8)^rng())>>8) throw(0);
                break;
            }
            catch(...) {cout<<"Invalid\n";}
        }
        initscr();
        i32 v[32];
        auto Board=[&]() {
            board();
            mvaddch(2+2*(cat/4),2+2*(cat%4),'X');
            mvaddch(2+2*(mouse/4),2+2*(mouse%4),'O');
            refresh();
            this_thread::sleep_for(chrono::milliseconds(200));
        };
        while(1) {
            if(!(t%32)) {
                endwin();
                while(1) {
                    cout<<": ";
                    try {
                        string s; getline(cin,s);
                        decode(cat,v,s);
                        break;
                    }
                    catch(...) {cout<<"Invalid\n";}
                }
                initwin();
            }
            Board();
            while(1) {
                i32 c=getch(); bool x=0;
                if(c==KEY_LEFT  &&  mouse%4!=0) mouse-=1, x=1;
                if(c==KEY_RIGHT &&  mouse%4!=3) mouse+=1, x=1;
                if(c==KEY_UP    &&  mouse/4!=0) mouse-=4, x=1;
                if(c==KEY_DOWN  &&  mouse/4!=3) mouse+=4, x=1;
                if(c=='.')                                x=1;
                if(c=='q') endwin(), exit(0);
                if(x) break;
            }
            if(mouse==cat) goto endm;
            cat=v[t++%32], Board();
            if(mouse==cat) goto endm;
            cat=v[t++%32], Board();
            if(mouse==cat) goto endm;
        }
        endm:
        board();
        mvaddch(2+2*(cat/4),2+2*(cat%4),'#');
        refresh();
        this_thread::sleep_for(chrono::milliseconds(1000));
        endwin();
        cout<<"Game over: "<<t/2<<" turns\n";
        cout<<"Verify: "<<utos(hash(seed^(t/2)))<<'\n';
        return 0;
    }
}
