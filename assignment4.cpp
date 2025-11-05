//Anushka Badhe
//TY-IT-A
//123B1F004

/*Problem Statement
Scenario: Smart Traffic Management for Emergency Vehicles
A smart city is implementing an intelligent traffic management system to assist ambulances
in reaching hospitals as quickly as possible. The city‘s road network is represented as a
graph, where:
● Intersections (junctions) are nodes.
● Roads between intersections are edges with weights representing travel time (in minutes)
considering traffic congestion.
An ambulance is currently at Source (S) and needs to reach the nearest hospital (Destination
D) in the shortest possible time. Due to dynamic traffic conditions, the weight of each road
segment may change in real time.
As a transportation engineer, you are assigned to:
1. Implement Dijkstra‘s algorithm to find the shortest path from the ambulance's current
location (S) to all possible hospitals.
2. Account for dynamic weight updates as traffic conditions change.
3. Optimize the system to work efficiently for a large city with thousands of intersections
and roads.
4. Provide a visual representation of the optimal path for navigation.
Expected Outcome:
The system should suggest the quickest route for the ambulance, updating dynamically
based on real-time traffic conditions, ensuring minimal response time to emergencies
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <set>
#include <algorithm>
#include <iomanip>
using namespace std;

// Practicing structured, maintainable class style
class Graph
{
public:
    struct Edge
    {
        int to;
        double weight;
    };

    unordered_map<int, vector<Edge>> adj;

    void addEdge(int from, int to, double weight, bool bidirectional = true)
    {
        adj[from].push_back({to, weight});
        if (bidirectional)
        {
            adj[to].push_back({from, weight});
        }
    }

    // Update traffic weight for both directions
    void updateWeight(int from, int to, double newWeight)
    {
        for (auto &edge : adj[from])
            if (edge.to == to) edge.weight = newWeight;

        for (auto &edge : adj[to])
            if (edge.to == from) edge.weight = newWeight;
    }

    // Better formatted road listing
    void printAllEdges() const
    {
        cout << "------------------------------------------------------\n";
        cout << " Road Network Travel Times (minutes)\n";
        cout << "------------------------------------------------------\n";
        cout << left << setw(15) << "From"
             << setw(15) << "To"
             << setw(10) << "Time\n";
        cout << string(45, '-') << "\n";

        set<pair<int,int>> printed;
        for (const auto &kv : adj)
        {
            int from = kv.first;
            for (const auto &e : kv.second)
            {
                int to = e.to;
                if (!printed.count({min(from,to), max(from,to)}))
                {
                    cout << left << setw(15) << from
                         << setw(15) << to
                         << setw(10) << e.weight << "\n";

                    printed.insert({min(from,to), max(from,to)});
                }
            }
        }
        cout << "------------------------------------------------------\n\n";
    }
};

// Dijkstra for nearest hospital
vector<int> dijkstra(const Graph& city, int src, const set<int>& hospitals, double& totalTime, int& nearestHospital)
{
    unordered_map<int, double> dist;
    unordered_map<int, int> prev;
    set<int> visited;

    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq;

    for (const auto &node : city.adj)
        dist[node.first] = numeric_limits<double>::infinity();

    dist[src] = 0.0;
    pq.push({0.0, src});

    nearestHospital = -1;
    double best = numeric_limits<double>::infinity();

    while (!pq.empty())
    {
        auto [curDist, u] = pq.top();
        pq.pop();

        if (visited.count(u)) continue;
        visited.insert(u);

        if (hospitals.count(u) && curDist < best)
        {
            best = curDist;
            nearestHospital = u;
        }

        for (const auto &edge : city.adj.at(u))
        {
            int v = edge.to;
            double w = edge.weight;
            if (curDist + w < dist[v])
            {
                dist[v] = curDist + w;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    totalTime = best;
    vector<int> path;

    if (nearestHospital != -1)
    {
        int node = nearestHospital;
        while (node != src)
        {
            path.push_back(node);
            node = prev[node];
        }
        path.push_back(src);
        reverse(path.begin(), path.end());
    }

    return path;
}

// Improved formatted path output
void printPath(const vector<int>& path, const Graph& city, double totalTime)
{
    cout << "Optimal Emergency Route\n";
    cout << "------------------------------------------------------\n";

    if (path.empty())
    {
        cout << "No reachable hospital detected.\n";
        return;
    }

    cout << "Route: ";
    for (size_t i = 0; i < path.size(); ++i)
    {
        cout << path[i];
        if (i != path.size()-1) cout << " -> ";
    }

    cout << "\nTotal Travel Time: " << totalTime << " min\n\n";
    cout << "Segment Breakdown:\n";
    cout << left << setw(10) << "From"
         << setw(10) << "To"
         << setw(12) << "Time\n";
    cout << string(32, '-') << "\n";

    for (size_t i = 1; i < path.size(); ++i)
    {
        int u = path[i-1], v = path[i];
        double w = -1;
        for (const auto& e : city.adj.at(u))
            if (e.to == v) w = e.weight;

        cout << left << setw(10) << u
             << setw(10) << v
             << setw(12) << w << "\n";
    }

    cout << "------------------------------------------------------\n\n";
}

int main()
{
    Graph city;

    // Sample city intersections
    city.addEdge(1,2,5);
    city.addEdge(2,3,5);
    city.addEdge(1,4,10);
    city.addEdge(4,5,6);
    city.addEdge(3,5,2);
    city.addEdge(5,6,3);

    set<int> hospitals = {3, 6};
    int ambulanceStart = 1;

    cout << "EMERGENCY ROUTING SYSTEM\n";
    cout << "======================================================\n\n";

    cout << "Step 1: Initial Road Network\n";
    city.printAllEdges();

    cout << "Step 2: Calculating Initial Optimal Route\n\n";
    double time;
    int nearestHospital;
    auto path = dijkstra(city, ambulanceStart, hospitals, time, nearestHospital);
    printPath(path, city, time);

    cout << "Step 3: Traffic Update (2 <-> 3 increases to 20 minutes)\n\n";
    city.updateWeight(2,3,20);
    city.printAllEdges();

    cout << "Step 4: Recomputing Optimal Route with Traffic\n\n";
    path = dijkstra(city, ambulanceStart, hospitals, time, nearestHospital);
    printPath(path, city, time);

    return 0;
}
