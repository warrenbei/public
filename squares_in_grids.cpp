#pragma GCC optimize("O3")
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <bitset>
#include <chrono>
#include <sstream>
#include <iomanip>
typedef int32_t i32;
typedef uint64_t u64;

#define PUZZLE 0

#if PUZZLE == -1
#elif PUZZLE == 0
static const i32 n = 6, m = 6;
static constexpr bool mask[n][m] = {
    {1,1,1,1,1,1},
    {1,1,1,1,1,1},
    {1,1,1,1,1,1},
    {1,1,1,1,1,1},
    {1,1,1,1,1,1},
    {1,1,1,1,1,0},
};
static const i32 D = 10;
static const i32 K = 22;
static const i32 L = 3;
static constexpr i32 g[K][L] = {
    {1,0,0},
    {1,2,1},
    {1,4,4},
    {1,6,9},
    {1,9,6},
    {2,2,5},
    {2,5,6},
    {2,8,9},
    {3,2,4},
    {3,6,1},
    {4,0,0},
    {4,4,1},
    {4,8,4},
    {5,2,9},
    {5,7,6},
    {6,2,5},
    {6,7,6},
    {7,2,9},
    {7,8,4},
    {8,4,1},
    {9,0,0},
    {9,6,1},
};
#elif PUZZLE == 1
static const i32 n = 12, m = 12;
static constexpr bool mask[n][m] = {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
};
static const i32 D = 10;
static const i32 K = 68;
static const i32 L = 4;
static constexpr i32 g[K][L] = {
    {1,0,2,4},
    {1,0,8,9},
    {1,1,5,6},
    {1,2,2,5},
    {1,2,9,6},
    {1,3,6,9},
    {1,4,4,4},
    {1,5,2,1},
    {1,6,0,0},
    {1,6,8,1},
    {1,7,6,4},
    {1,8,4,9},
    {1,9,3,6},
    {2,0,2,5},
    {2,1,1,6},
    {2,2,0,9},
    {2,3,0,4},
    {2,4,0,1},
    {2,5,0,0},
    {2,6,0,1},
    {2,7,0,4},
    {2,8,0,9},
    {2,9,1,6},
    {3,0,2,5},
    {3,1,3,6},
    {3,2,4,9},
    {3,3,6,4},
    {3,4,8,1},
    {3,6,0,0},
    {3,7,2,1},
    {3,8,4,4},
    {3,9,6,9},
    {4,0,9,6},
    {4,2,2,5},
    {4,3,5,6},
    {4,4,8,9},
    {4,6,2,4},
    {4,7,6,1},
    {4,9,0,0},
    {5,0,4,1},
    {5,1,8,4},
    {5,3,2,9},
    {5,4,7,6},
    {5,6,2,5},
    {5,7,7,6},
    {5,9,2,9},
    {6,0,8,4},
    {6,2,4,1},
    {6,4,0,0},
    {6,5,6,1},
    {6,7,2,4},
    {6,8,8,9},
    {7,0,5,6},
    {7,2,2,5},
    {7,3,9,6},
    {7,5,6,9},
    {7,7,4,4},
    {7,9,2,1},
    {8,1,0,0},
    {8,2,8,1},
    {8,4,6,4},
    {8,6,4,9},
    {8,8,3,6},
    {9,0,2,5},
    {9,2,1,6},
    {9,4,0,9},
    {9,6,0,4},
    {9,8,0,1},
};
#elif PUZZLE == 2
static const i32 n = 6, m = 7;
static constexpr bool mask[n][m] = {
    {1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1},
};
static const i32 D = 10;
static const i32 K = 31;
static const i32 L = 3;
static constexpr i32 g[K][L] = {
    {1,0,5},
    {1,2,0},
    {1,3,6},
    {1,5,3},
    {1,7,1},
    {1,9,0},
    {2,1,0},
    {2,3,1},
    {2,5,3},
    {2,7,6},
    {3,0,0},
    {3,2,5},
    {3,5,1},
    {3,7,8},
    {4,0,6},
    {4,3,5},
    {4,6,5},
    {4,9,6},
    {5,2,8},
    {5,6,1},
    {5,9,5},
    {6,3,0},
    {6,6,6},
    {7,0,3},
    {7,4,1},
    {7,8,0},
    {8,2,0},
    {8,6,1},
    {9,0,3},
    {9,4,6},
    {9,9,0},
};
#endif

namespace {
    using namespace std;

    struct trie_node {
        trie_node* adj[D] = {};
        i32 val = -1;
    } *root = new trie_node;

    static bool trie_init = false;

    void init_trie() {
        if (trie_init) return;
        trie_init = true;
        for (i32 k = 0; k < K; k++) {
            trie_node* cur = root;
            for (i32 t = 0; t < L; t++) {
                if (cur->adj[g[k][t]] == nullptr)
                    cur->adj[g[k][t]] = new trie_node;
                cur = cur->adj[g[k][t]];
            }
            cur->val = k;
        }
    }

    class obj {
    public:
        i32 a[n][m];

        obj() {}

        obj(istream& in) {
            for (i32 i = 0; i < n; i++)
                for (i32 j = 0; j < m; j++)
                    in >> a[i][j];
        }

        void explain(ostream& out) {
            char g[2*n-1][2*m-1];
            for (i32 i = 0; i <= 2*(n-1); i++)
                for (i32 j = 0; j <= 2*(m-1); j++)
                    g[i][j] = ' ';
            for (i32 i = 0; i < n; i++)
                for (i32 j = 0; j < m; j++)
                    g[2*i][2*j] = '#';
            bitset<K> res;
            i32 total = 0;
            for (i32 i = 0; i < n; i++)
                for (i32 j = 0; j <= m-L; j++) {
                    trie_node* cur = root;
                    bool bad = false;
                    for (i32 t = 0; t < L; t++)
                        if (!mask[i][j+t] || cur->adj[a[i][j+t]] == nullptr) {
                            bad = true;
                            break;
                        } else {
                            cur = cur->adj[a[i][j+t]];
                        }
                    if (!bad) {
                        res[cur->val] = 1;
                        total++;
                        for (i32 t = 0; t < L-1; t++)
                            g[2*i][2*(j+t)+1] = '-';
                        for (i32 t = 0; t < L; t++)
                            g[2*i][2*(j+t)] = '0' + a[i][j+t];
                    }
                }
            for (i32 j = 0; j < m; j++)
                for (i32 i = 0; i <= n-L; i++) {
                    trie_node* cur = root;
                    bool bad = false;
                    for (i32 t = 0; t < L; t++)
                        if (!mask[i+t][j] || cur->adj[a[i+t][j]] == nullptr) {
                            bad = true;
                            break;
                        } else {
                            cur = cur->adj[a[i+t][j]];
                        }
                    if (!bad) {
                        res[cur->val] = 1;
                        total++;
                        for (i32 t = 0; t < L-1; t++)
                            g[2*(i+t)+1][2*j] = '|';
                        for (i32 t = 0; t < L; t++)
                            g[2*(i+t)][2*j] = '0' + a[i+t][j];
                    }
                }
            int cells = 0;
            for (i32 i = 0; i < n; i++)
                for (i32 j = 0; j < m; j++)
                    if (g[2*i][2*j] != '#')
                        cells++;
            cout << '\n';
            for (i32 i = 0; i <= 2*(n-1); i++) {
                for (i32 j = 0; j <= 2*(m-1); j++)
                    cout << g[i][j];
                cout << '\n';
            }
            cout << "Cells used: " << cells << '\n';
            cout << "Total lines: " << total << " (" << res.count() << " distinct)\n";
        }

        void showGrid(ostream& out) const {
            for (i32 i = 0; i < n; i++) {
                for (i32 j = 0; j < m; j++)
                    out << a[i][j] << ' ';
                out << '\n';
            }
        }
    };

    void copy(const obj& x, obj& y) {
        for (i32 i = 0; i < n; i++)
            for (i32 j = 0; j < m; j++)
                y.a[i][j] = x.a[i][j];
    }

    template<i32 _N, i32 _M> class evolution {
    public:
        static const i32 N = _N, M = _M;
        static const i32 MAX_TICKS = 10000;
        static const i32 MUTATE_TRIES = 10;
        static const i32 PROGRESS_DOTS = 50;
        static const i32 UPHILL_PROB = 10;
        string session;
        i32 T;

    private:
        mt19937 rng;
        i32 a[N][M][n][m];
        mt19937 io_rng;

        void copy(const i32 (&x)[n][m], i32 (&y)[n][m]) const {
            for (i32 i = 0; i < n; i++)
                for (i32 j = 0; j < m; j++)
                    y[i][j] = x[i][j];
        }

    public:
        evolution(const u64 seed) {
            rng.seed(seed);
            io_rng.seed(0);
            time_t sessionT = chrono::steady_clock::now().time_since_epoch().count();
            stringstream S;
            S << hex << setfill('0') << setw(16) << sessionT;
            S >> session;
            T = 0;
            for (i32 I = 0; I < N; I++)
                for (i32 J = 0; J < M; J++)
                    for (i32 i = 0; i < n; i++)
                        for (i32 j = 0; j < m; j++)
                            a[I][J][i][j] = rng() % D;
            cout << "New soup\n"
                << "Object size: " << n << " x " << m << '\n'
                << "Soup size: " << N << " x " << M << '\n'
                << "Max ticks: " << MAX_TICKS << '\n'
                << "Digits: " << D << '\n'
                << "Strings: " << K << '\n';
        }

        void plant(const vector<obj> v) {
            i32 r[N*M];
            for (i32 t = 0; t < N*M; t++)
                r[t] = t;
            for (i32 t = N*M-1; t >= 1; t--)
                swap(r[t], r[rng() % (t+1)]);
            for (i32 t = 0; t < min(i32(v.size()), N*M); t++)
                copy(v[t].a, a[r[t]/M][r[t]%M]);
        }

    private:
        const bitset<K> getScore(const i32 (&x)[n][m]) const {
            bitset<K> res;
            for (i32 i = 0; i < n; i++)
                for (i32 j = 0; j <= m-L; j++) {
                    trie_node* cur = root;
                    bool bad = false;
                    for (i32 t = 0; t < L; t++)
                        if (!mask[i][j+t] || cur->adj[x[i][j+t]] == nullptr) {
                            bad = true;
                            break;
                        } else {
                            cur = cur->adj[x[i][j+t]];
                        }
                    if (!bad)
                        res[cur->val] = 1;
                }
            for (i32 j = 0; j < m; j++)
                for (i32 i = 0; i <= n-L; i++) {
                    trie_node* cur = root;
                    bool bad = false;
                    for (i32 t = 0; t < L; t++)
                        if (!mask[i+t][j] || cur->adj[x[i+t][j]] == nullptr) {
                            bad = true;
                            break;
                        } else {
                            cur = cur->adj[x[i+t][j]];
                        }
                    if (!bad)
                        res[cur->val] = 1;
                }
            return res;
        }

        void mutate(i32 (&x)[n][m]) {
            for (i32 t = 0; t < MUTATE_TRIES; t++) {
                i32 initScore = getScore(x).count();
                i32 i = rng() % n, j = rng() % m;
                i32 tmp = x[i][j];
                x[i][j] = rng() % D;
                if (getScore(x).count() < initScore)
                    x[i][j] = tmp;
            }
        }

        const bool activation(const i32 x) {
            return x >= 0 || x == -1 && rng() % UPHILL_PROB == 0;
        }

        void mix(i32 (&x)[n][m], const i32 (&y)[n][m]) {
            bitset<K> X = getScore(x), Y = getScore(y);
            if ((Y & X.flip()).none())
                return;
            i32 cx0 = 0, cx1 = n-1, cy0 = 0, cy1 = m-1;
            while (cx0 == 0 && cx1 == n-1 && cy0 == 0 && cy1 == m-1) {
                cx0 = rng() % n, cx1 = rng() % n;
                if (cx0 > cx1) swap(cx0, cx1);
                cy0 = rng() % m, cy1 = rng() % m;
                if (cy0 > cy1) swap(cy0, cy1);
            }
            i32 dx = rng() % (n - cx1 + cx0), dy = rng() % (m - cy1 + cy0);
            i32 z[n][m];
            copy(x, z);
            for (i32 i = cx0; i <= cx1; i++)
                for (i32 j = cy0; j <= cy1; j++)
                    z[i][j] = y[i-cx0+dx][j-cy0+dy];
            mutate(z);
            if (activation(getScore(z).count() - getScore(x).count()))
                copy(z, x);
        }

    public:
        void evolve() {
            for (i32 I = 0; I < N; I++)
                for (i32 J = 0; J < M; J++)
                    if ((I+J) % 2 == T % 2) {
                        mix(a[I][J], a[(I+1)%N][J]);
                        mix(a[I][J], a[I][(J+1)%M]);
                        mix(a[I][J], a[(I+N-1)%N][J]);
                        mix(a[I][J], a[I][(J+N-1)%M]);
                        mutate(a[I][J]);
                    }
            T++;
        }

        void save() const {
            ofstream fout;
            fout.open("data/scores");
            for (i32 I = 0; I < N; I++) {
                for (i32 J = 0; J < M; J++)
                    fout << getScore(a[I][J]).count() << '\t';
                fout << '\n';
            }
            fout.close();
            fout.open("data/soup");
            for (i32 I = 0; I < N; I++) {
                for (i32 k = 0; k <= M*(m+3); k++)
                    fout << "+";
                fout << '\n';
                for (i32 J = 0; J < M; J++) {
                    fout << "+ ";
                    for (i32 j = 0; j < m; j++)
                        fout << ' ';
                    fout << " ";
                }
                fout << "+\n";
                for (i32 i = 0; i < n; i++) {
                    for (i32 J = 0; J < M; J++) {
                        fout << "+ ";
                        for (i32 j = 0; j < m; j++)
                            fout << a[I][J][i][j];
                        fout << " ";
                    }
                    fout << "+\n";
                }
                for (i32 J = 0; J < M; J++) {
                    fout << "+ ";
                    for (i32 j = 0; j < m; j++)
                        fout << ' ';
                    fout << " ";
                }
                fout << "+\n";
            }
            for (i32 k = 0; k <= M*(m+3); k++)
                fout << "+";
            fout << '\n';
            fout.close();
        }

        const string getFile() const {
            return "data/.object:" + session + ":" + to_string(T);
        }

        void run() {
            i32 x;
            cin >> x;
            if (x <= 0 || x > MAX_TICKS) {
                cout << "Invalid #ticks: must be <= " << MAX_TICKS << "\n";
                return;
            }
            cout << "Running " << session << '\n' << flush;
            for (i32 t = 0, k = 0; t < x; t++) {
                evolve();
                while (k < (t+1)*PROGRESS_DOTS/x) {
                    k++;
                    cout << '.' << flush;
                }
            }
            cout << '\n';
            ofstream fout;
            fout.open(getFile());
            getBest().showGrid(fout);
            fout.close();
        }

        obj getBest() {
            i32 bestScore = 0;
            pair<i32,i32> ans[N*M];
            i32 numSols = 0;
            for (i32 I = 0; I < N; I++)
                for (i32 J = 0; J < M; J++) {
                    i32 score = getScore(a[I][J]).count();
                    if (score > bestScore) {
                        bestScore = score;
                        numSols = 0;
                    }
                    if (score == bestScore)
                        ans[numSols++] = {I,J};
                }
            pair<i32,i32> p = ans[io_rng() % numSols];
            obj x;
            copy(a[p.first][p.second], x.a);
            return x;
        }
    };
}

#if PUZZLE == -1
#elif PUZZLE == 0
static const i32 W = 15, H = 15;
#elif PUZZLE == 1
static const i32 W = 10, H = 10;
#elif PUZZLE == 2
static const i32 W = 15, H = 15;
#endif

signed main() {
    init_trie();
    cout << "Grid puzzle evolver\n"
        << "Initial soup seed 0\n";
    evolution<W,H> soup(0);
    vector<obj> objs;
    while (true) {
        cout << "Time " << soup.T << '\n';
        soup.save();
        cout << "Objects: " << objs.size() << '\n';
        cout << ": ";
        string S;
        cin >> S;
        try {
            if (S == "run") {
                soup.run();
            } else if (S == "newSoup" || S == "new") {
                soup = evolution<W,H>(chrono::steady_clock::now().time_since_epoch().count());
            } else if (S == "addObject") {
                cout << "Not recommended, use loadObject instead\n";
                objs.push_back(soup.getBest());
            } else if (S == "loadObject" || S == "load") {
                cin >> S;
                ifstream fin("data/.object:" + S);
                if (fin) {
                    objs.push_back(obj(fin));
                    objs.back().explain(cout);
                }
                fin.close();
            } else if (S == "loadThis" || S == "push") {
                ifstream fin(soup.getFile());
                if (fin) {
                    objs.push_back(obj(fin));
                    objs.back().explain(cout);
                }
                fin.close();
            } else if (S == "delLast" || S == "pop") {
                if (!objs.empty()) objs.pop_back();
            } else if (S == "clearObjects") {
                objs.clear();
            } else if (S == "plant") {
                soup.plant(objs);
            } else {
                cout << "HELP:\n"
                    << "\trun [ticks]\n"
                    << "\tnewSoup new\n"
                    << "\taddObject\n"
                    << "\tloadObject load\n"
                    << "\tloadThis push\n"
                    << "\tdelLast pop\n"
                    << "\tclearObjects\n"
                    << "\tplant\n"
                    << '\n';
            }
        } catch(...) {
            cout << "Error\n";
        }
    }
}
