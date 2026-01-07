#include <iostream>
#include <unordered_map>
#include <chrono>
using namespace std;

using ll = long long;
constexpr ll MOD = 1000000007LL;

ll proc1(ll n) {
    unordered_map<ll, ll> dp;
    dp.reserve((size_t)n + 1); // 해시 리사이즈 줄이기(측정 안정화)
    dp[0] = 0;
    dp[1] = 1;
    for (ll i = 2; i <= n; i++) {
        dp[i] = (dp[i - 1] + dp[i - 2]) % MOD;
    }
    return dp[n];
}

ll proc2(ll n) {
    unordered_map<ll, ll> dp;
    dp.reserve(1024); // 얘는 많이 안 채워질 가능성 높음
    dp[0] = 0; dp[1] = 1; dp[2] = 1; dp[3] = 2; dp[4] = 3;

    auto f = [&dp](auto&& self, ll x) -> ll {
        if (auto it = dp.find(x); it != dp.end()) return it->second;
        ll k = x / 2;
        ll a = self(self, k + 2);
        ll b = self(self, x - k - 1);
        ll c = self(self, k + 1);
        ll d = self(self, x - k - 2);
        ll val = (a * b + c * d) % MOD;
        dp.emplace(x, val);
        return val;
        };

    return f(f, n);
}

template <class F>
long long bench_ms(F&& fn, ll n, int iters) {
    // 워밍업(첫 실행은 느릴 수 있음)
    volatile ll warm = fn(n);
    (void)warm;

    auto start = chrono::high_resolution_clock::now();
    volatile ll sink = 0;
    for (int i = 0; i < iters; ++i) {
        sink ^= fn(n); // 최적화로 함수 호출이 날아가지 않게
    }
    auto end = chrono::high_resolution_clock::now();
    (void)sink;

    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

int main() {
    ll n;
    cin >> n;

    // proc1은 O(n)이라 너무 큰 n이면 시간이 오래 걸릴 수 있음.
    int iters1 = 3;
    int iters2 = 1000;

    auto t1 = bench_ms(proc1, n, iters1);
    auto t2 = bench_ms(proc2, n, iters2);

    cout << "proc1(ms, " << iters1 << " runs): " << t1 << "\n";
    cout << "proc2(ms, " << iters2 << " runs): " << t2 << "\n";

    cout << "f1=" << proc1(n) << " f2=" << proc2(n) << "\n"; // 결과 동일성 확인
}
