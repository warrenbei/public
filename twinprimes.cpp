#include <fstream>
#include <cassert>
using namespace std;
typedef uint64_t u64;
typedef uint32_t uN;
const uN N=1<<17;
const int K=4;
const u64 I=1;
// compute primes <= N, then cyclic memory sieve to N^2
// 3512 primes up to N=2^15; only work with odd numbers
// at most 9 distinct prime factors (prod 2..23 = 2.23e8)
// bitmask smallest 64 primes: others >= 317 so 3x u16
// 317^4 > 2^33
const uN p[64] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313};
int main() {
    FILE* fout=fopen("results.txt","w");
    if(!fout) return 1;
    u64 a[N]={}; uN b[N][K]={};
    auto write=[&](u64 i) {
        fprintf(fout,"%lu %lu\n",2*i-1,2*i+1);
    };
    {
        uN primes[N]={}; int cnt=0;
        bool pr[N/2]; for(int i=1; i<N/2; i++) pr[i]=1;
        for(int i=1; i<N/2; i++) if(pr[i]) {
            if(cnt&&primes[cnt-1]==i-1) write(i);
            primes[cnt++]=i;
            for(int j=i; j<N/2; j+=2*i+1) pr[j]=0;
        }
        for(int i=0; i<cnt; i++) {
            if(i<64) a[primes[i]]=I<<i;
            else b[primes[i]][0]=2*primes[i]+1;
        }
        // note: N-1 and N+1 are not twin primes for N=2^k
    }
    auto ins=[&](int i, uN x) {
        if(b[i][0]==0) b[i][0]=x;
        else if(b[i][1]==0) b[i][1]=x;
        else if(b[i][2]==0) b[i][2]=x;
        else if(b[i][3]==0) b[i][3]=x;
        else assert(0);
    };
    bool prv=0;
    for(int k=0; k<N/2; k++) for(int i=0; i<N; i++) {
        // 2Nk+2i+1
        if(a[i]||b[i][0]) {
            while(a[i]) {
                int t=__builtin_ctzll(a[i]);
                a[i]-=I<<t, a[(i+p[t])&(N-1)]|=I<<t;
            }
            for(int j=0; j<K; j++) {
                if(b[i][j]) ins((i+b[i][j])&(N-1),b[i][j]), b[i][j]=0;
                else break;
            }
            prv=0;
        }
        else {
            if(prv) write(N*I*k+i);
            prv=1;
        }
    }
    fclose(fout);
}
