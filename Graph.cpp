#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

const int MAX_SIZE = 50;

class Graph {
    int V;
    vector<int> adj[MAX_SIZE];
    
public:
    Graph(int V) : V(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Đếm số thành phần liên thông
    int countConnectedComponents() {
        vector<bool> visited(V, false);
        int count = 0;
        for (int v = 0; v < V; ++v) {
            if (!visited[v]) {
                dfs(v, visited);
                ++count;
            }
        }
        return count;
    }
    void dfs(int v, vector<bool> &visited) {
        visited[v] = true;
        for (int u : adj[v]) {
            if (!visited[u])
                dfs(u, visited);
        }
    }
    bool isBipartite() {
        vector<int> color(V, -1);
        queue<int> q;
        for (int v = 0; v < V; ++v) {
            if (color[v] == -1) {
                q.push(v);
                color[v] = 0;
                while (!q.empty()) {
                    int u = q.front();
                    q.pop();
                    for (int w : adj[u]) {
                        if (color[w] == -1) {
                            color[w] = 1 - color[u];
                            q.push(w);
                        } else if (color[w] == color[u])
                            return false;
                    }
                }
            }
        }
        return true;
    }
    bool hasCycle() {
        vector<bool> visited(V, false);
        for (int v = 0; v < V; ++v) {
            if (!visited[v]) {
                if (hasCycleUtil(v, visited, -1))
                    return true;
            }
        }
        return false;
    }
    bool hasCycleUtil(int v, vector<bool> &visited, int parent) {
        visited[v] = true;
        for (int u : adj[v]) {
            if (!visited[u]) {
                if (hasCycleUtil(u, visited, v))
                    return true;
            } else if (u != parent)
                return true;
        }
        return false;
    }

     void findArticulationPoints() {
        vector<bool> visited(V, false);
        vector<int> disc(V, -1);
        vector<int> low(V, -1);
        vector<int> parent(V, -1);
        vector<bool> ap(V, false);

        for (int i = 0; i < V; ++i) {
            if (!visited[i])
                dfsArticulationPoints(i, visited, disc, low, parent, ap);
        }

        cout << "Articulation Points: ";
        for (int i = 0; i < V; ++i) {
            if (ap[i])
                cout << i <<" ";
        }
        cout << endl;
    }
    void dfsArticulationPoints(int u, vector<bool> &visited, vector<int> &disc, vector<int> &low, vector<int> &parent, vector<bool> &ap) {
        static int time = 0; // Biến đếm thời gian trong DFS

        int children = 0; // Đếm số con của nút u trong DFS

        visited[u] = true; // Đánh dấu nút u đã được thăm
        disc[u] = low[u] = ++time; // Gán thời điểm khám đầu tiên và thời điểm nhỏ nhất của u là time

        for (int v : adj[u]) {
            if (!visited[v]) {
                children++; // Tăng số lượng con của u
                parent[v] = u; // Gán u là cha của v trong cây DFS

                dfsArticulationPoints(v, visited, disc, low, parent, ap); // Gọi đệ quy DFS cho v

                low[u] = min(low[u], low[v]); // Cập nhật low[u] dựa trên low của các con của nó

                // Kiểm tra xem có cạnh ngược từ con của u đến tổ tiên của u không
                if (parent[u] == -1 && children > 1) // Nếu u là gốc của cây DFS và có nhiều hơn một con
                    ap[u] = true; // thì u là điểm giao nối
                if (parent[u] != -1 && low[v] >= disc[u]) // Nếu v không thể tiếp tục trở về nhỏ hơn thời điểm khám đầu tiên của u
                    ap[u] = true; // thì u là điểm giao nối
            } else if (v != parent[u]) { // Nếu v đã được thăm và không phải là cha của u
                low[u] = min(low[u], disc[v]); // Cập nhật low[u] dựa trên thời điểm khám của v
            }
        }
    }
};

Graph readGraph(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        exit(1);
    }

    int V;
    file >> V;
    Graph g(V);

    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            int weight;
            file >> weight;
            if (weight == 1)
                g.addEdge(i, j);
        }
    }

    file.close();
    return g;
}

int main() {
    Graph g = readGraph("adjacency_matrix.txt");

    cout << "Number of Connected Components: " << g.countConnectedComponents() << endl;

    cout << "Is Bipartite: " << (g.isBipartite() ? "Yes" : "No") << endl;

    cout << "Does Graph have Cycle: " << (g.hasCycle() ? "Yes" : "No") << endl;

    g.findArticulationPoints();

    return 0;
}
