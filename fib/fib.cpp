#include <iostream>
#include <chrono>
#include <functional>   // std::invoke
#include <iomanip>      // setw, setprecision
#include <string_view>
using namespace std;

using ll = long long;
static constexpr ll MOD = 1000000007LL;

static inline ll mod_norm(ll x) {
    x %= MOD;
    if (x < 0) x += MOD;
    return x;
}
static inline ll mod_add(ll a, ll b) {
    a += b;
    if (a >= MOD) a -= MOD;
    return a;
}
static inline ll mod_sub(ll a, ll b) {
    a -= b;
    if (a < 0) a += MOD;
    return a;
}
static inline ll mod_mul(ll a, ll b) {
    // (1e9+7)^2 는 64-bit signed 범위 안이라 ll로도 안전
    return (a * b) % MOD;
}

// O(n) 선형 피보나치
ll fib_linear(ll n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    ll a = 0, b = 1; // F(0), F(1)
    for (ll i = 2; i <= n; ++i) {
        ll c = mod_add(a, b);
        a = b;
        b = c;
    }
    return b;
}

// fast doubling: (F(n), F(n+1)) 반환, O(log n)
pair<ll, ll> fib_pair(ll n) {
    if (n == 0) return { 0, 1 };
    auto [a, b] = fib_pair(n / 2); // a=F(k), b=F(k+1)

    ll two_b_minus_a = mod_sub(mod_add(b, b), a);
    ll c = mod_mul(a, two_b_minus_a);                 // F(2k)
    ll d = mod_add(mod_mul(a, a), mod_mul(b, b));     // F(2k+1)

    if ((n & 1LL) == 0) return { c, d };
    return { d, mod_add(c, d) }; // F(2k+1), F(2k+2)
}

ll fib_fast(ll n) {
    return fib_pair(n).first;
}

struct BenchResult {
    string_view name;
    string_view complexity;
    ll n = 0;
    ll value = 0;
    long long iters = 0;
    long long elapsed_ns = 0;
    double total_ms = 0.0;
    double ns_per_call = 0.0;
};

template <class F>
BenchResult bench(string_view name, string_view complexity, F&& fn, ll n, long long target_ms = 200) {
    using clock = chrono::steady_clock;

    // warm-up + 값 확보
    volatile ll warm = std::invoke(fn, n);
    ll value = warm;

    long long iters = 1;
    long long elapsed_ns = 0;

    while (true) {
        auto start = clock::now();
        volatile ll sink = 0;
        for (long long i = 0; i < iters; ++i) {
            sink ^= std::invoke(fn, n);
        }
        auto end = clock::now();
        (void)sink;

        elapsed_ns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        if (elapsed_ns >= target_ms * 1'000'000LL) break;
        iters *= 2;
        if (iters > (1LL << 30)) break;
    }

    BenchResult r;
    r.name = name;
    r.complexity = complexity;
    r.n = n;
    r.value = value;
    r.iters = iters;
    r.elapsed_ns = elapsed_ns;
    r.total_ms = elapsed_ns / 1e6;
    r.ns_per_call = (double)elapsed_ns / (double)iters;
    return r;
}

static void print_header(ll n) {
    cout << "N=" << n << "  (mod " << MOD << ")\n";
    cout << string(86, '=') << "\n";
    cout << left
        << setw(20) << "Algorithm"
        << setw(12) << "Complexity"
        << right
        << setw(14) << "Result"
        << setw(12) << "Iters"
        << setw(14) << "Total(ms)"
        << setw(14) << "ns/call"
        << "\n";
    cout << string(86, '-') << "\n";
}

static void print_row(const BenchResult& r) {
    cout << left
        << setw(20) << r.name
        << setw(12) << r.complexity
        << right
        << setw(14) << r.value
        << setw(12) << r.iters
        << setw(14) << fixed << setprecision(3) << r.total_ms
        << setw(14) << fixed << setprecision(2) << r.ns_per_call
        << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n;
    cin >> n;

    // 같은 목표 시간으로 돌리면 비교가 더 공정함
    constexpr long long TARGET_MS = 200;

    auto r1 = bench("fib_linear", "O(n)", fib_linear, n, TARGET_MS);
    auto r2 = bench("fib_fast", "O(log n)", fib_fast, n, TARGET_MS);

    print_header(n);
    print_row(r1);
    print_row(r2);
    cout << string(86, '=') << "\n";

    // 값이 같은지 빠르게 sanity check
    if (r1.value != r2.value) {
        cout << "WARNING: results differ! (" << r1.value << " vs " << r2.value << ")\n";
    }

    return 0;
}
