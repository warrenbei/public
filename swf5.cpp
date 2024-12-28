#include <iostream>
#include <ncurses.h>
#include <iomanip>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <thread>
#include <assert.h>
using namespace std;
typedef int32_t i32;
typedef uint64_t u64;
template<class C> using vc=std::vector<C>;

bool argp(char arg[], string s) {
    for(i32 i=0; i<s.size(); i++) if(arg[i]!=s[i]) return 0;
    return 1;
}

namespace game {
    struct card {
        i32 col, shp, num, fil, stk;
        bool del=0;
    };
    static const card dcard{0,0,0,0,0,1};
    bool is_set(card a, card b, card c) {
        return
        !((a.col+b.col+c.col)%3) &&
        !((a.shp+b.shp+c.shp)%3) &&
        !((a.num+b.num+c.num)%3) &&
        !((a.fil+b.fil+c.fil)%3) &&
        !((a.stk+b.stk+c.stk)%3) &&
        1;
    }
    vc<card> shuffle(u64 seed) {
        vc<card> v;
        for(i32 col:{0,1,2})
        for(i32 shp:{0,1,2})
        for(i32 num:{0,1,2})
        for(i32 fil:{0,1,2})
        for(i32 stk:{0,1,2})
            v.push_back(card{col,shp,num,fil,stk});
        mt19937_64 rng(seed);
        for(i32 i=v.size()-1; i; i--) swap(v[i],v[rng()%(i+1)]);
        return v;
    }
    struct properties {
        i32 color[3];
        i32 bg;
        i32 border, border_bg;
        i32 fill, fill_select;
        i32 set_wrong, set_right;
        static const i32 x=6, y=6;
        char shape[3][3][x][y+1];
    };
    static const properties light
    {
        {
            0x800080,
            0x008002,
            0xff0101,
        },
        0x7f7f7f,
        0x000000, 0xffffff,
        0xe0e0e0, 0xa0d0d0,
        0xd07070, 0x70d070,
        {
            //diamond
            {
                //outline
                {
                    "``/\\``",
                    "`/  \\`",
                    "/    \\",
                    "\\    /",
                    "`\\  / ",
                    "``\\/ `",
                },
                //striped
                {
                    "``/\\``",
                    "`/..\\`",
                    "/....\\",
                    "\\..../",
                    "`\\../ ",
                    "``\\/ `",
                },
                //solid
                {
                    "``/\\``",
                    "`/##\\`",
                    "/####\\",
                    "\\####/",
                    "`\\##/ ",
                    "``\\/ `",
                },
            },
            //oval
            {
                //outline
                {
                    "``-==`",
                    "`/   \\",
                    "|    |",
                    "|    |",
                    "\\   / ",
                    " ==- `",
                },
                //striped
                {
                    "``-==`",
                    "`/...\\",
                    "|....|",
                    "|....|",
                    "\\.../ ",
                    " ==- `",
                },
                //solid
                {
                    "``-==`",
                    "`/###\\",
                    "|####|",
                    "|####|",
                    "\\###/ ",
                    " ==- `",
                },
            },
            //squiggle
            {
                //outline
                {
                    "``-=-`",
                    "`/   |",
                    "`| \\ `",
                    "``\\ | ",
                    "|   /`",
                    " -=- `",
                },
                //striped
                {
                    "``-=-`",
                    "`/...|",
                    "`|.\\ `",
                    "``\\.| ",
                    "|.../`",
                    " -=- `",
                },
                //solid
                {
                    "``-=-`",
                    "`/###|",
                    "`|#\\ `",
                    "``\\#| ",
                    "|###/`",
                    " -=- `",
                },
            },
        },
    };
    struct keyboard {
        const i32 mincards, addcards;
        i32 n,m;
        map<char,i32> mp;
        keyboard(i32 minc, i32 addc, vc<string> v):
        mincards(minc), addcards(addc) {
            n=v.size(), m=v[0].size();
            for(i32 i=0; i<n; i++) {
                assert(v[i].size()==m);
                for(i32 j=0; j<m; j++) mp[v[i][j]]=i*m+j;
            }
        }
    };
    static const keyboard stdkeys
    {
        18, 6,
        {
            "123456",
            "qwerty",
            "asdfgh",
            "zxcvbn",
            "!@#$%^",
            "QWERTY",
            "ASDFGH",
            "ZXCVBN",
        },
    };
}

string help_str="\
... [options]\n\
    h   Help\n\
Keybinds:\n\
0 to exit\n\
123456\n\
qwerty\n\
asdfgh\n\
zxcvbn\n\
!@#$%^\n\
QWERTY\n\
ASDFGH\n\
ZXCVBN\n\
";

signed main(i32 argc, char *argv[]) {
    try {
        for(i32 i=1; i<argc; i++) {
            if(argp(argv[i],"h")) cout<<help_str, exit(0);
        }
    }
    catch(...) {
        cout<<"Unknown option\n";
        return 1;
    }
    const u64 seed=chrono::steady_clock::now().time_since_epoch().count();
    chrono::steady_clock::time_point t0(chrono::steady_clock::now());
    i32 t_tot;
    vc<game::card> v=game::shuffle(seed), board;
    game::properties params=game::light;
    game::keyboard kboard=game::stdkeys;
    vc<i32> select(kboard.mp.size(),8);
    i32 setcnt=0;
    auto has_set=[&]() {
        for(i32 i=0;   i<board.size(); i++)
            for(i32 j=i+1; j<board.size(); j++)
                for(i32 k=j+1; k<board.size(); k++)
                    if(is_set(board[i],board[j],board[k])) return 1;
        return 0;
    };
    auto get_time=[&]() {
        t_tot=chrono::duration_cast<chrono::milliseconds>
                (chrono::steady_clock::now()-t0).count();
    };
    auto time_str=[&]() {
        stringstream S;
        S<<t_tot/60000<<":";
        S<<setfill('0')<<setw(2)<<(t_tot%60000)/1000<<":";
        S<<setfill('0')<<setw(2)<<(t_tot%1000)/10;
        string s; getline(S,s); return s;
    };
    initscr(), cbreak(), noecho(), curs_set(0);
    keypad(stdscr,1), nodelay(stdscr,1), start_color();
    auto setcol=[&](i32 i, i32 c) {
        init_color(i,
            ( (c>>16)       *1000)>>8,
            ( ((c>>8)&255)  *1000)>>8,
            ( (c&255)       *1000)>>8
        );
    };
    setcol(0,params.bg);
    setcol(1,params.border), setcol(2,params.border_bg);
    setcol(3,params.fill), setcol(4,params.fill_select);
    setcol(5,params.set_wrong), setcol(6,params.set_right);
    init_pair(0,0,0); //0: background
    init_pair(1,1,2); //1: border
    init_pair(2,1,4); //2: time text
    bkgd(COLOR_PAIR(0));
    for(i32 i=0; i<3; i++) {
        setcol(i+8,params.color[i]);
        init_pair(i+8,i+8,3); //i+8: i
        init_pair(i+12,i+8,4); //i+12: select i
        init_pair(i+16,i+8,5); //i+16: set i wrong
        init_pair(i+20,i+8,6); //i+20: set i right
    }
    auto display=[&](bool upd) {
        assert(board.size()<=kboard.mp.size());
        if(upd) clear();
        attron(COLOR_PAIR(2));
        get_time();
        mvaddstr(1,4,(" "+time_str()+" ").c_str());
        mvaddstr(3,2,(" Sets found: "+to_string(setcnt)+" ").c_str());
        attroff(COLOR_PAIR(2));
        if(upd) {
            static const i32 cx=game::properties::x, cy=game::properties::y,
                dx=cx+9, dy=3*cy+9;
            for(i32 t=0; t<board.size(); t++) {
                const i32 x0=5+(t/kboard.m)*dx, y0=1+(t%kboard.m)*dy;
                //borders
                attron(COLOR_PAIR(1));
                mvaddch(x0,     y0+1,   '/');
                mvaddch(x0,     y0+dy-3,'\\');
                mvaddch(x0+1,   y0,     '/');
                mvaddch(x0+1,   y0+dy-2,'\\');
                mvaddch(x0+dx-3,y0,     '\\');
                mvaddch(x0+dx-3,y0+dy-2,'/');
                mvaddch(x0+dx-2,y0+1,   '\\');
                mvaddch(x0+dx-2,y0+dy-3,'/');
                for(i32 i=2; i<=dy-4; i++) {
                    mvaddch(x0,     y0+i,   '-');
                    mvaddch(x0+dx-2,y0+i,   '-');
                }
                for(i32 i=2; i<=dx-4; i++) {
                    mvaddch(x0+i,   y0,     '|');
                    mvaddch(x0+i,   y0+dy-2,'|');
                }
                attroff(COLOR_PAIR(1));
                //card background
                i32 bcol=board[t].col+select[t];
                attron(COLOR_PAIR(bcol));
                for(i32 i=1; i<=dx-3; i++)
                    for(i32 j=1; j<=dy-3; j++)
                        mvaddch(x0+i,y0+j,' ');
                //layers
                const char (*g)[cy+1]=params.shape[board[t].shp][board[t].fil];
                auto paste=[&](i32 x, i32 y, i32 opt) {
                    for(i32 i=0; i<cx; i++)
                        for(i32 j=0; j<cy; j++)
                            if(g[i][j]!='`') mvaddch(x+i,y+j,g[i][j]|opt);
                };
                auto paste_row=[&](i32 x, i32 y, i32 opt) {
                    if(board[t].num==0)
                        paste(x,y+cy+1,opt);
                    if(board[t].num==1)
                        paste(x,y+cy+cy/2+1,opt),
                        paste(x,y+cy/2,opt);
                    if(board[t].num==2)
                        paste(x,y+cy+cy+2,opt),
                        paste(x,y+cy+1,opt),
                        paste(x,y,opt);
                };
                auto paste_stk=[&](i32 x, i32 y) {
                    if(board[t].stk==0)
                        paste_row(x+2,y+1,A_BOLD);
                    if(board[t].stk==1)
                        paste_row(x+3,y+2,A_DIM),
                        paste_row(x+1,y+1,A_BOLD);
                    if(board[t].stk==2)
                        paste_row(x+4,y+2,A_DIM),
                        paste_row(x+2,y+1,A_DIM),
                        paste_row(x,y,A_BOLD);
                };
                paste_stk(x0+2,y0+2);
                attroff(COLOR_PAIR(bcol));
            }
        }
        refresh();
    };
    auto quit=[&]() {
        display(1);
        attron(COLOR_PAIR(2));
        mvaddstr(2,3," No sets left ");
        attroff(COLOR_PAIR(2));
        refresh(), this_thread::sleep_for(chrono::milliseconds(2000));
        timeout(-1); i32 c=getch(); endwin();
        cout<<time_str()<<'\n';
        exit(0);
    };
    auto fill_cards=[&]() {
        auto add_card=[&]() {
            game::card c=v.back(); v.pop_back(); return c;
        };
        if(board.size()>kboard.mincards) {
            for(i32 i=board.size()-1; i>=0; i--) if(board[i].del)
                board.erase(board.begin()+i);
        }
        if(board.size()<kboard.mincards) board.resize(kboard.mincards,game::dcard);
        for(i32 i=0; i<board.size(); i++) if(board[i].del) {
            if(v.empty()) break;
            board[i]=add_card();
        }
        for(i32 i=0; i<board.size(); i++) if(board[i].del)
            board.erase(board.begin()+i), i--;
        if(has_set()) return;
        while(!v.empty() &&
            ((board.size()-kboard.mincards)%kboard.addcards || !has_set()))
            board.push_back(add_card());
        if(!has_set()) quit();
    };
    auto quit_fast=[&]() {
        endwin();
        cout<<"Exit\n";
        exit(0);
    };
    display(1), fill_cards();
    while(1) {
        display(1);
        i32 c=ERR;
        while(c==ERR) {
            display(0), this_thread::sleep_for(chrono::milliseconds(100));
            c=getch();
        }
        flushinp();
        auto it=kboard.mp.find(c);
        if(it!=kboard.mp.end()) {
            i32 k=it->second;
            if(k<board.size()) select[k]^=4;
        }
        else if(c=='0') {
            quit_fast();
        }
        vc<i32> v;
        for(i32 i=0; i<select.size(); i++) if(select[i]==12) v.push_back(i);
        if(v.size()==3) {
            bool valid=game::is_set(board[v[0]],board[v[1]],board[v[2]]);
            for(i32 i=0; i<3; i++) select[v[i]]=valid?20:16;
            display(1), this_thread::sleep_for(chrono::milliseconds(200));
            if(valid) {
                for(i32 i=0; i<3; i++) board[v[i]].del=1;
                setcnt++;
            }
            for(i32 i=0; i<select.size(); i++) select[i]=8;
            fill_cards();
        }
    }
}
