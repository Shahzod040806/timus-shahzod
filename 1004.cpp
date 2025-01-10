/* 1004. Sightseeing Trip - http://acm.timus.ru/problem.aspx?num=1004
 *
 * Strategy:
 * For each node n: run Dijkstra to get the shortest path tree, then add a single (existing) edge to
 * every possible pair of that tree to find the shortest Y-shaped loop starting at n. Report the 
 * shortest such loop after having iterated over all starting nodes. Any such loop will have a
 * degenerate "stem" of the Y and hence be a true loop.
 *
 * Performance:
 * O(N^3 log N), runs the test cases in 0.046s using 424KB memory.
 */


#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstdio>
#include <algorithm>

using namespace std;

struct Node {
    int dist;           // Кратчайшее расстояние до узла
    bool visited;       // Был ли посещен узел
    vector<Node *> adj; // Соседи узла
    Node *prev;         // Предшествующий узел в кратчайшем пути
};

Node nodes[101];
int graph[101][101];   // Матрица смежности

const int INF = INT_MAX / 3;
int minDist = INF;     // Длина самого короткого цикла
vector<int> minPath;   // Узлы этого цикла

// Возвращает список узлов от стартового узла до текущего, исключая сам стартовый узел
vector<int> getPath(Node *start, Node *node) {
    vector<int> result;
    while (node != start) {
        result.push_back(node - nodes);
        node = node->prev;
    }
    return result;
}

// Поиск минимального цикла Y-образной формы в графе, начиная с узла s
void findCycle(int s, int N) {
    priority_queue<pair<int, Node *>, vector<pair<int, Node *>>, greater<pair<int, Node *>>> pq;
    
    for (int i = 1; i <= N; ++i) {
        nodes[i].dist = INF;
        nodes[i].visited = false;
    }
    nodes[s].dist = 0;

    // Алгоритм Дейкстры для поиска кратчайших путей
    pq.push({0, nodes + s});
    while (!pq.empty()) {
        auto current = pq.top().second;
        pq.pop();
        
        if (current->visited) {
            continue;
        }
        current->visited = true;
        
        for (auto neighbor : current->adj) {
            int u = current - nodes;
            int v = neighbor - nodes;
            
            if (!neighbor->visited && neighbor->dist > current->dist + graph[u][v]) {
                neighbor->prev = current;
                neighbor->dist = current->dist + graph[u][v];
                pq.push({neighbor->dist, neighbor});
            }
        }
    }

    // Перебираем все пары узлов и пытаемся найти минимальный цикл
    pair<int, int> bestPair;
    int bestCost = INF;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            if (i != j && nodes[i].prev != nodes + j && nodes[j].prev != nodes + i) {
                if (bestCost > nodes[i].dist + nodes[j].dist + graph[i][j]) {
                    bestCost = nodes[i].dist + nodes[j].dist + graph[i][j];
                    bestPair = {i, j};
                }
            }
        }
    }

    // Если найден новый минимальный цикл, обновляем результат
    if (minDist > bestCost) {
        minDist = bestCost;
        minPath.clear();
        minPath.push_back(s); // Добавляем начальный узел
        
        auto firstPath = getPath(nodes + s, nodes + bestPair.first); // Путь к одной ветви Y
        auto secondPath = getPath(nodes + s, nodes + bestPair.second); // Путь ко второй ветви Y
        
        minPath.insert(minPath.end(), firstPath.begin(), firstPath.end());
        reverse(secondPath.begin(), secondPath.end()); // Разворачиваем вторую ветвь
        minPath.insert(minPath.end(), secondPath.begin(), secondPath.end());
    }
}

int main() {
    while (true) {
        int N, M;
        cin >> N;
        if (N == -1) {
            break;
        }
        cin >> M;
        
        fill(&graph[0][0], &graph[100][101], INF);
        while (M--) {
            int a, b, w;
            cin >> a >> b >> w;
            if (w < graph[a][b]) {
                graph[a][b] = graph[b][a] = w;
            }
        }

        for (int i = 1; i <= N; ++i) {
            for (int j = 1; j < i; ++j) {
                if (graph[i][j] != INF) {
                    nodes[i].adj.push_back(nodes + j);
                    nodes[j].adj.push_back(nodes + i);
                }
            }
        }

        minDist = INF;
        for (int i = 1; i <= N; ++i) {
            findCycle(i, N);
        }

        if (minDist == INF) {
            cout << "No solution." << endl;
        } else {
            for (int node : minPath) {
                cout << node << ' ';
            }
            cout << endl;
        }

        for (int i = 0; i < 101; ++i) {
            nodes[i] = {0, false, {}, nullptr};
        }
    }
    return 0;
}
