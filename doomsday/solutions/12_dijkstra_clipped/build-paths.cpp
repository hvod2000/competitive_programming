#include <bits/stdc++.h>
#pragma GCC optimize("-Ofast")
using namespace std;
using namespace std::chrono;

const bool DEBUG = false;
typedef vector<map<int, pair<int, set<int>>>> Graph;
typedef tuple<int, int, vector<int>, vector<int>> Query;

int number_of_edges;
Graph G;
vector<Query> queris;

vector<bool> get_doomsdays(vector<int> &path) {
	vector<bool> risks(number_of_edges + 1);
	int path_length = path.size();
	for (int i = 1; i < path_length; i++)
		for (int risk : G[path[i - 1]][path[i]].second)
			risks[risk] = true;
	return risks;
}

inline vector<int> dijkstra1(int s, int t) {
	int n = G.size() - 1;
	vector<int> visited(n + 1); for (int i = 0; i < n; i++) if (visited[i] != 0) *((int *) NULL) = 0;
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
			int dist2 = edge_risks.size();
			Q.push({-(dist + dist2), v, u});
		}
	}
	if (!visited[t]) {
		vector<int> empty_path(0);
		return empty_path;
	}
	vector<int> path;
	int u = t;
	path.push_back(u);
	while (visited[u] != -1) {
		u = visited[u];
		path.push_back(u);
	}
	reverse(path.begin(), path.end());
	return path;
}

inline vector<int> dijkstra2(int s, int t, vector<bool> risks) {
	int n = G.size() - 1;
	vector<int> visited(n + 1); for (int i = 0; i < n; i++) if (visited[i] != 0) *((int *) NULL) = 0;
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
				dist2 += risks[risk] ? 1 : 0;
			Q.push({-(dist + dist2), v, u});
		}
	}
	if (!visited[t]) {
		vector<int> empty_path(0);
		return empty_path;
	}
	vector<int> path;
	int u = t;
	path.push_back(u);
	while (visited[u] != -1) {
		u = visited[u];
		path.push_back(u);
	}
	reverse(path.begin(), path.end());
	return path;
}

void print_path(vector<int> &path) {
	int n = path.size();
	cout << n - 1;
	for (int i = 0; i < n; i++) cout << " " << path[i];
	cout << endl;
}

inline void *process_query(void *query_as_void) {
	Query *query = (Query *) query_as_void;
	int u = get<0>(*query);
	int v = get<1>(*query);
	auto start = high_resolution_clock::now();
	vector<int> path1 = dijkstra1(u, v);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	get<2>(*query) = path1;
	get<3>(*query) = (duration.count() < 312) ? dijkstra2(u, v, get_doomsdays(path1)) : path1;
	return NULL;
}

int main() {
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

	for (int i = 0; i < q; i++) {
		int u, v;
		cin >> u >> v;
		vector<int> empty_list(0);
		queris[i] = {u, v, empty_list, empty_list};
	}

	/*
	vector<pthread_t> threads(q);
	for (int i = 0; i < q; i++)
		if (pthread_create(&threads[i], NULL, process_query, &queris[i]))
			return 42;
	for (int i = 0; i < q; i++) {
   		void *status;
		pthread_join(threads[i], &status);
		print_path(get<2>(queris[i]));
		print_path(get<3>(queris[i]));
	}
	*/
	for (int i = 0; i < q; i++) {
		process_query(&queris[i]);
		print_path(get<2>(queris[i]));
		print_path(get<3>(queris[i]));
	}
}
