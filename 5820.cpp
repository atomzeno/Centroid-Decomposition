#pragma warning(disable:4996)
#include<stdio.h>
#include<vector>
#include<math.h>
#include<algorithm>
#define N 200009
#define K 1000009
#define INF 1e8
using namespace std;
int n, k, chk[K], siz[N], cutted[N];
typedef struct NODE {
	int ind, dst;
	void input(int ind2, int dst2) {
		ind = ind2, dst = dst2;
	}
}NODE;
vector<NODE> edge[N], cur_close[N];
vector<int> cur_values;
int cal_size(int node, int par);
int cal_centroid(int node, int par, int capacity);
int cal_ans(int node, int par, int distance, int numb);
void update_chk(int node, int par, int distance, int numb);
int decompose(int cur);
void input();
int main() {
	input();
	int i;
	for (i = 1; i <= k; i++) {
		chk[i] = INF;
	}
	chk[0] = 0;
	int ret_ans = decompose(0);
	if (ret_ans == INF) {
		ret_ans = -1;
	}
	printf("%d", ret_ans);
}
void input() {
	int i;
	NODE imsi;
	scanf("%d%d", &n, &k);
	for (i = 1; i < n; i++) {
		int u, v, w;
		scanf("%d%d%d", &u, &v, &w);
		imsi.input(v, w);
		edge[u].push_back(imsi);
		imsi.input(u, w);
		edge[v].push_back(imsi);
	}
}
int decompose(int cur) {
	int cur_num = cal_size(cur, -1);
	int cur_centroid = cal_centroid(cur, -1, cur_num / 2);
	cutted[cur_centroid] = 1;
	int ret_ans = INF;
	for (NODE nxt : edge[cur_centroid]) {
		if (cutted[nxt.ind] == 1) {
			continue;
		}
		ret_ans = min(ret_ans, cal_ans(nxt.ind, cur_centroid, nxt.dst, 1));
		update_chk(nxt.ind, cur_centroid, nxt.dst, 1);
	}
	for (int clear_val : cur_values) {
		chk[clear_val] = INF;
	}
	cur_values.clear();
	for (NODE nxt : edge[cur_centroid]) {
		if (cutted[nxt.ind] == 1) {
			continue;
		}
		ret_ans = min(ret_ans, decompose(nxt.ind));
	}
	//cutted[cur]=0;
	return ret_ans;
}
void update_chk(int node, int par, int distance, int numb) {
	if (distance > k) {
		return;
	}
	chk[distance] = min(chk[distance], numb);
	cur_values.push_back(distance);
	for (NODE nxt : edge[node]) {
		if (nxt.ind == par || cutted[nxt.ind] == 1) {
			continue;
		}
		update_chk(nxt.ind, node, distance + nxt.dst, numb + 1);
	}
	return;
}
int cal_ans(int node, int par, int distance, int numb) {
	int ret_ans = INF;
	if (distance > k) {
		return INF;
	}
	if (chk[k - distance] != INF) {
		ret_ans = min(ret_ans, chk[k - distance] + numb);
	}
	for (NODE nxt : edge[node]) {
		if (nxt.ind == par || cutted[nxt.ind] == 1) {
			continue;
		}
		ret_ans = min(ret_ans, cal_ans(nxt.ind, node, distance + nxt.dst, numb + 1));
	}
	return ret_ans;
}
int cal_size(int node, int par) {//서브트리 노드 개수를 구하는 부분
	siz[node] = 1;
	for (NODE nxt : edge[node]) {
		if (nxt.ind == par || cutted[nxt.ind] == 1) {
			continue;
		}
		siz[node] += cal_size(nxt.ind, node);
	}
	return siz[node];
}
int cal_centroid(int node, int par, int capacity) {//centroid를 구하는 부분
	for (NODE nxt : edge[node]) {
		if (nxt.ind == par || cutted[nxt.ind] == 1) {
			continue;
		}
		if (siz[nxt.ind] > capacity) {
			return cal_centroid(nxt.ind, node, capacity);
		}
	}
	return node;
}