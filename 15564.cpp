#pragma warning(disable:4996)
#include <stdio.h>
#include <vector>
#include <queue>
#include <math.h>
#include <algorithm>
using namespace std;
#define N 100001
#define SZN 23
const int INF = 1e9 + 9;
int spa[SZN][N], dep[N];
int siz[N], cutted[N];
int cdtp[N];//centroid decomposition tree parent array
vector<int> edge[N];
int ST[N];
void dfs(int node, int par_node, int depth);
int cal_size(int node, int par);
int cal_centroid(int node, int par, int capacity);
void decompose(int cur, int par_cur);
int lca(int u, int v);
int get_dist(int u, int v);
void upd_violin(int node);
int closest_violin(int node);
int main() {
	int n, i, j, q;
	scanf("%d%d", &n, &q);
	for (i = 2; i <= n; i++) {
		int u, v;
		scanf("%d", &v);
		u = i;
		//printf("%d %d\n", u, v);
		edge[u].push_back(v);
		edge[v].push_back(u);
	}

	dfs(1, 1, 1);
	for (i = 1; i < (SZN - 1); i++) {
		for (j = 1; j <= n; j++) {
			spa[i][j] = spa[i - 1][spa[i - 1][j]];
			//spa[i][j] : 원래 트리 상의 j번째 정점의 2^i번째 부모
		}
	}
	decompose(1, -1);//트리를 centroid decomposition을 완료
	//가장 처음에 모든 정점의 색은 검정색(0번)이다.
	for (i = 1; i <= n; i++) {
		ST[i] = INF;
	}
	for (i = 1; i <= q; i++) {
		int type, vert_num;
		scanf("%d%d", &type, &vert_num);
		if (type == 1) {
			//1 u : u번 시간축이 바이올린이 있던 시간축이 된다.
			upd_violin(vert_num);
		}
		else {
			//2 u : u번 시간축에서 시작하는 여행이 전설의 바이올린을 찾을 때까지 최소로 이동해야 하는 횟수를 출력한다. 
			//방법이 없으면 -1을 출력한다.
			printf("%d\n", closest_violin(vert_num));
		}
	}
}

void upd_violin(int node) {
	int pv = node;
	for (;;) {
		int distance_pn = get_dist(pv, node);
		ST[pv] = min(ST[pv], distance_pn);
		if (pv == cdtp[pv]) {
			break;
		}
		pv = cdtp[pv];
	}
}
int closest_violin(int node) {
	int pv = node;
	int ans_node = INF;
	for (;;) {
		int dist_np = get_dist(node, pv);
		if (ST[pv] != INF) {
			int dist_pmn = ST[pv];
			ans_node = min(ans_node, dist_np + dist_pmn);
		}
		if (pv == cdtp[pv]) {
			break;
		}
		pv = cdtp[pv];
	}
	if (ans_node == INF) {
		ans_node = -1;
	}
	return ans_node;
}
void dfs(int node, int par_node, int depth) {
	spa[0][node] = par_node;
	dep[node] = depth;
	for (int next : edge[node]) {
		if (next == par_node) {
			continue;
		}
		dfs(next, node, depth + 1);
	}
}
int cal_size(int node, int par) {
	siz[node] = 1;
	for (int nxt : edge[node]) {
		if (nxt == par || cutted[nxt] == 1) {
			continue;
		}
		siz[node] += cal_size(nxt, node);
	}
	return siz[node];
}
int cal_centroid(int node, int par, int capacity) {
	for (int nxt : edge[node]) {
		if (nxt == par || cutted[nxt] == 1) {
			continue;
		}
		if (siz[nxt] > capacity) {
			return cal_centroid(nxt, node, capacity);
		}
	}
	return node;
}
void decompose(int cur, int par_cur) {
	int cur_num = cal_size(cur, -1);
	int cur_centroid = cal_centroid(cur, -1, cur_num / 2);
	cutted[cur_centroid] = 1;
	cdtp[cur_centroid] = par_cur;
	if (par_cur == -1) {
		cdtp[cur_centroid] = cur_centroid;
	}
	for (int nxt : edge[cur_centroid]) {
		if (cutted[nxt] == 1) {
			continue;
		}
		decompose(nxt, cur_centroid);
	}
}
int lca(int u, int v) {
	if (dep[u] < dep[v]) {
		swap(u, v);
	}
	//dep[u]>=dep[v]
	int i, difer = dep[u] - dep[v];
	for (i = 0; i < SZN; i++) {
		if ((difer & (1 << i)) == 0) {
			continue;
		}
		u = spa[i][u];
	}
	if (u == v) {
		return u;
	}
	for (i = SZN - 1; i >= 0; i--) {
		if (((1 << i) <= dep[u]) && spa[i][u] != spa[i][v]) {
			u = spa[i][u];
			v = spa[i][v];
		}
	}
	return spa[0][u];
}
int get_dist(int u, int v) {
	int lca_uv = lca(u, v);
	return dep[u] + dep[v] - 2 * dep[lca_uv];
}