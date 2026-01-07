#include <iostream>
#include <unordered_map>
using namespace std;
using ll = long long;
constexpr ll MOD = 1e9 + 7;
void proc1(ll n);
void proc2(ll n);
int main()
{
	// f[n] = f[n-1] + f[n-2]

	ll n;
	cin >> n;

	proc1(n);
	proc2(n);
}

void proc1(ll n)
{
	unordered_map<ll, ll> dp;
	dp[0] = 0;
	dp[1] = 1;
	for (ll i = 2; i <= n; i++) {
		dp[i] = dp[i - 1] + dp[i - 2];
	}
	printf("f[%lld]=%lld\n", n, dp[n]);
}

void proc2(ll n)
{
	/*
	1.
f[2n] = f[2n-1] + f[2n-2]
f[2n+1] = f[2n] + f[2n-1]
f[2n+2] = f[2n+1] + f[2n]
f[2n+3] = f[2n+2] + f[2n+1]
f[2n+4] = f[2n+3] + f[2n+2]
f[2n+5] = f[2n+4] + f[2n+3]


2.

f[2n+1] = f[2n-1] + f[2n-2] + f[2n-1] = 2f[2n-1] + f[2n-2]
f[2n+2] = 2f[2n-1] + f[2n-2] + f[2n-1] + f[2n-2] = 3f[2n-1] + 2f[2n-2]
f[2n+3] = 3f[2n-1] + 2f[2n-2] + 2f[2n-1] + f[2n-2] = 5f[2n-1] + 3f[2n-2]
f[2n+4] = 5f[2n-1] + 3f[2n-2] + 3f[2n-1] + 2f[2n-2] = 8f[2n-1] + 5f[2n-2]
f[2n+5] = 8f[2n-1] + 5f[2n-2] + 5f[2n-1] + 3f[2n-2] = 13f[2n-1] + 8f[2n-2]
...
f[2n+k] = f[k+2] * f[2n-1] + f[k+1] * f[2n-2]

2n+k = x

f[x] = f[k+2] * f[x-k-1] + f[k+1] * f[x-k-2]


	*/
	unordered_map<ll, ll> dp;
	dp[0] = 0, dp[1] = 1, dp[2] = 1, dp[3] = 2;
	dp[4] = 3;

	auto f = [&dp](auto&& self, ll x) -> ll {
		if (dp.contains(x))
			return dp[x];
		ll k = x / 2;
		dp[x] = self(self, k + 2) * self(self, x - k - 1) + self(self, k + 1) * self(self, x - k - 2);
		return dp[x] %= MOD;
		};
	printf("f[%lld]=%lld\n", n, f(f, n));
}
