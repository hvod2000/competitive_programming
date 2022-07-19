#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
using namespace std;
using namespace std::chrono;

const bool DEBUG = false;
typedef vector<map<int, pair<int, set<int>>>> Graph;
typedef tuple<int, int, vector<vector<int>>> Query;

int number_of_edges;
Graph G;
vector<Query> queris;

inline vector<int> simple_bfs(int s, int t) {
	int n = G.size() - 1;
	vector<int> visited(n + 1);
	queue<int> Q;
	Q.push(s);
	visited[s] = -1;
	while (!visited[t]) {
		int u = Q.front(); Q.pop();
		if (DEBUG) cout << u << endl;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			visited[v] = u;
			Q.push(v);
		}
	}
	vector<int> path;
	int u = t; path.push_back(u);
	while (visited[u] != -1) path.push_back(u = visited[u]);
	return path;
}

inline vector<int> simple_dfs(int s, int t) {
	int n = G.size() - 1;
	vector<int> visited(n + 1);
	stack<int> Q;
	Q.push(s);
	visited[s] = -1;
	while (!visited[t]) {
		int u = Q.top(); Q.pop();
		if (DEBUG) cout << u << endl;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			visited[v] = u;
			Q.push(v);
		}
	}
	vector<int> path;
	int u = t; path.push_back(u);
	while (visited[u] != -1) path.push_back(u = visited[u]);
	return path;
}

inline vector<bool> get_doomsdays(vector<int> &path) {
	vector<bool> risks(number_of_edges + 1);
	int path_length = path.size();
	for (int i = 1; i < path_length; i++)
		for (int risk : G[path[i - 1]][path[i]].second)
			risks[risk] = true;
	return risks;
}

inline vector<int> dijkstra(int s, int t, vector<bool> risks) {
	int n = G.size() - 1;
	int doomsday_cost = 25 * n;
	vector<int> visited(n + 1);
	priority_queue<tuple<int, int, int>> Q;
	Q.push({-0, s, -1});
	while ((not Q.empty()) and !visited[t]) {
		auto [dist, u, prev] = Q.top(); Q.pop();
		if (DEBUG) cout << -dist << " - " << u << " prev=" << prev << endl;
		if (visited[u]) continue;
		if (DEBUG) cout << "not skipped" << endl;
		visited[u] = prev;
		dist = -dist;
		for (auto &[v, edge] : G[u]) {
			if (DEBUG) cout << u << " -> " << v << endl;
			if (visited[v]) continue;
			auto &[edge_dist, edge_risks] = edge;
			int dist2 = 0;
			for (int risk : edge_risks)
				dist2 += risks[risk] ? doomsday_cost : 0;
			Q.push({-(dist + dist2), v, u});
		}
	}
	vector<int> path;
	int u = t; path.push_back(u);
	while (visited[u] != -1) path.push_back(u = visited[u]);
	return path;
}

inline void print_path(vector<int> &path) {
	int n = path.size();
	cout << n - 1;
	for (int i = n - 1; i >= 0; i--) cout << " " << path[i];
	cout << endl;
}

void process_query(void *query_as_void_pointer) {
	Query *query = (Query *) query_as_void_pointer;
	auto &[u, v, paths] = *query;
	paths.push_back(simple_dfs(u, v));
	paths.push_back(simple_bfs(u, v));
}

int main() {
	auto start = high_resolution_clock::now();
	int n, m, q;
	cin >> n >> m >> q;
	number_of_edges = m;
	G.resize(n + 1);
	queris.resize(q);
	
	for (int i = 0; i < m; i++) {
		int u, v, w, k;
		cin >> u >> v >> w >> k;
		set<int> C;
		for (int i = 0; i < k; i++) {
			int c;
			cin >> c;
			C.insert(c);
		}
		G[u][v] = {w, C};
		G[v][u] = {w, C};
	}

	vector<vector<int>> empty_list(0);
	for (int i = 0; i < q; i++) {
		int u, v;
		cin >> u >> v;
		queris[i] = {u, v, empty_list};
	}

	for (int i = 0; i < q; i++)
		process_query(&queris[i]);
	for (int i = 0; true; i = (i + 1) % q) {
		auto stop = high_resolution_clock::now();
		                                                      // 666 to win!
		if (duration_cast<microseconds>(stop - start).count() > 966613) break;
		auto &[u, v, paths] = queris[i];
		paths.push_back(dijkstra(u, v, get_doomsdays(paths.back())));
	}

	for (int i = 0; i < q; i++) {
		vector<vector<int>> &paths = get<2>(queris[i]);
		int last = paths.size() - 1;
		print_path(paths[last]);
		print_path(paths[last-1]);
	}
	return 0;
}
