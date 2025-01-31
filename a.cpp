#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<int>;
#define pb push_back
#define all(x) begin(x), end(x)
#define rsz resize

#define F0R(i, a) for (int i = 0; i < (a); i++)
#define FOR(i, a, b) for (int i = (a); i <= (b); i++)
#define R0F(i, a) for (int i = (a)-1; i >= 0; i--)
#define ROF(i, a, b) for (int i = (b); i >= a; i--)
#define trav(a, x) for (auto &a : x)

/* SCC from BenQ */
struct SCC {
	int N;
	vector<vi> adj, radj;
	vi todo, comps, comp;
	vector<bool> vis;
	void init(int _N) {
		N = _N;
		adj.rsz(N), radj.rsz(N), comp = vi(N, -1), vis.rsz(N);
	}
	void ae(int x, int y) { adj[x].pb(y), radj[y].pb(x); }
	void dfs(int x) {
		vis[x] = 1;
		trav(y, adj[x]) if (!vis[y]) dfs(y);
		todo.pb(x);
	}
	void dfs2(int x, int v) {
		comp[x] = v;
		trav(y, radj[x]) if (comp[y] == -1) dfs2(y, v);
	}
	void gen(int _N) {  // fills allComp
		FOR(i, 1, _N) if (!vis[i]) dfs(i);
		reverse(all(todo));
		trav(x, todo) if (comp[x] == -1) {
			dfs2(x, x);
			comps.pb(x);
		}
	}
};

const int maxn = 1e5 + 5;

int n, m;
SCC scc;          // scc
int value[maxn];  // value of each room
ll group[maxn];   // value in each SCC
vi rgraph[maxn];  // reverse graph
ll dp[maxn];

// calculates dp[i]
ll DP(int i) {
	if (dp[i]) return dp[i];
	// start at i
	dp[i] = group[i];
	// simulate traveling from another SCC to this SCC
	trav(j, rgraph[i]) dp[i] = max(dp[i], DP(j) + group[i]);
	return dp[i];
}

int main() {
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> n >> m;
	scc.init(n + 1);
	FOR(i, 1, n) cin >> value[i];
	while (m--) {
		int a, b;
		cin >> a >> b;
		scc.ae(a, b);
	}

	// generate SCC
	scc.gen(n);
	// set dp to 0
	fill(dp + 1, dp + n + 1, 0);
	// precompute group value for each SCC
	FOR(i, 1, n) group[scc.comp[i]] += value[i];
	// create reverse edges for SCC
	FOR(i, 1, n) trav(j, scc.adj[i]) {
		if (scc.comp[i] == scc.comp[j]) continue;
		rgraph[scc.comp[j]].pb(scc.comp[i]);
	}
	// find dp value for each SCC
	ll ans = 0;
	trav(i, scc.comps) ans = max(ans, DP(i));

	cout << ans << '\n';
}
