#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <set>
using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge {
    int u, v, weight;
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Kruskal算法求最小生成树
vector<Edge> kruskalMST(int n, vector<Edge>& edges) {
    vector<int> parent(n);
    iota(parent.begin(), parent.end(), 0);

    function<int(int)> find = [&](int x) {
        return x == parent[x] ? x : parent[x] = find(parent[x]);
    };

    vector<Edge> mst;
    sort(edges.begin(), edges.end());
    for (const auto& edge : edges) {
        int u = find(edge.u), v = find(edge.v);
        if (u != v) {
            parent[u] = v;
            mst.push_back(edge);
        }
    }
    return mst;
}

// 寻找最小权完美匹配（这里用简单实现，实际需要Blossom算法）
vector<pair<int, int>> minWeightPerfectMatching(const vector<vector<int>>& graph, const vector<int>& oddVertices) {
    int k = oddVertices.size();
    vector<pair<int, int>> matching;
    set<int> used;
    for (int i = 0; i < k; ++i) {
        if (used.count(i)) continue;
        int minWeight = INF, bestJ = -1;
        for (int j = i + 1; j < k; ++j) {
            if (!used.count(j) && graph[oddVertices[i]][oddVertices[j]] < minWeight) {
                minWeight = graph[oddVertices[i]][oddVertices[j]];
                bestJ = j;
            }
        }
        if (bestJ != -1) {
            matching.emplace_back(oddVertices[i], oddVertices[bestJ]);
            used.insert(i);
            used.insert(bestJ);
        }
    }
    return matching;
}

// 欧拉回路构造与哈密顿回路转化略
// ...

int main() {
    int n; // 顶点数
    cin >> n;
    vector<Edge> edges; // 边集合

    // 输入完全图
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int weight;
            cin >> weight;
            edges.push_back({i, j, weight});
        }
    }

    // Step 1: 最小生成树
    vector<Edge> mst = kruskalMST(n, edges);

    // Step 2: 找出奇度顶点
    vector<int> degree(n, 0);
    for (const auto& edge : mst) {
        degree[edge.u]++;
        degree[edge.v]++;
    }
    vector<int> oddVertices;
    for (int i = 0; i < n; ++i) {
        if (degree[i] % 2 == 1) {
            oddVertices.push_back(i);
        }
    }

    // Step 3: 最小权完美匹配
    vector<vector<int>> graph(n, vector<int>(n, INF));
    for (const auto& edge : edges) {
        graph[edge.u][edge.v] = edge.weight;
        graph[edge.v][edge.u] = edge.weight;
    }
    auto matching = minWeightPerfectMatching(graph, oddVertices);

    // 后续步骤略...
    return 0;
}