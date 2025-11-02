#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <tuple>

using namespace std;

// Practicing disciplined struct style
struct Edge
{
    int to;
    double cost;
};

int main()
{
    cout << "MULTISTAGE SHORTEST PATH SYSTEM\n";
    cout << "====================================================\n\n";

    int stages = 4;

    // Number of nodes in each stage
    vector<int> stageNodes = {1, 3, 3, 1};

    int totalNodes = 0;
    vector<int> stageStart(stages);

    // Compute starting index for each stage
    for (int i = 0; i < stages; ++i)
    {
        stageStart[i] = totalNodes;
        totalNodes += stageNodes[i];
    }

    // Graph storage: directed structure for stage graph
    vector<vector<Edge>> graph(totalNodes);

    // Provided multistage edges (node index, to index, cost)
    vector<tuple<int,int,double>> edges =
    {
        {0,1,1},   {0,2,2},   {0,3,3},
        {1,4,4},   {1,5,11},
        {2,4,9},   {2,5,5},  {2,6,16},
        {3,6,2},
        {4,7,18},  {5,7,13}, {6,7,2}
    };

    // Build graph adjacency lists
    for (auto &t : edges)
        graph[get<0>(t)].push_back({get<1>(t), get<2>(t)});

    // DP arrays
    vector<double> minCost(totalNodes, numeric_limits<double>::infinity());
    vector<int> nextNode(totalNodes, -1);

    // Last stage cost = 0 since it's destination stage
    for (int i = stageStart[stages-1]; i < totalNodes; ++i)
        minCost[i] = 0;

    // Bottom-up DP to compute optimal path
    for (int s = stages - 2; s >= 0; --s)
    {
        for (int n = stageStart[s]; n < stageStart[s] + stageNodes[s]; ++n)
        {
            for (const Edge &e : graph[n])
            {
                if (minCost[n] > e.cost + minCost[e.to])
                {
                    minCost[n] = e.cost + minCost[e.to];
                    nextNode[n] = e.to;
                }
            }
        }
    }

    cout << "----------------------------------------------------\n";
    cout << "Initial Minimum Costs from Stage 0 Nodes\n";
    cout << "----------------------------------------------------\n";
    for (int n = stageStart[0]; n < stageStart[0]+stageNodes[0]; ++n)
        cout << "Node " << n << " -> Best Cost = " << minCost[n] << "\n";

    cout << "\nComputed Optimal Path\n";
    cout << "----------------------------------------------------\n";

    int curr = 0;
    double routeCost = 0;
    cout << "Path: " << curr;

    while (nextNode[curr] != -1)
    {
        cout << " -> " << nextNode[curr];
        for (const Edge &e : graph[curr])
            if (e.to == nextNode[curr]) routeCost += e.cost;

        curr = nextNode[curr];
    }

    cout << "\nTotal Path Cost = " << routeCost << "\n";
    cout << "----------------------------------------------------\n";

    // ---------------- TRAFFIC UPDATE / GRAPH CHANGE -------------------
    cout << "\nTRAFFIC UPDATE: Edge (3 -> 6) cost changed 2 -> 99\n";
    cout << "Recomputing optimal path...\n\n";

    for (Edge &e : graph[3])
        if (e.to == 6) e.cost = 99;

    fill(minCost.begin(), minCost.end(), numeric_limits<double>::infinity());
    fill(nextNode.begin(), nextNode.end(), -1);

    for (int i = stageStart[stages-1]; i < totalNodes; ++i)
        minCost[i] = 0;

    for (int s = stages - 2; s >= 0; --s)
    {
        for (int n = stageStart[s]; n < stageStart[s] + stageNodes[s]; ++n)
        {
            for (const Edge &e : graph[n])
            {
                if (minCost[n] > e.cost + minCost[e.to])
                {
                    minCost[n] = e.cost + minCost[e.to];
                    nextNode[n] = e.to;
                }
            }
        }
    }

    cout << "Updated Minimum Costs from Stage 0 Nodes\n";
    cout << "----------------------------------------------------\n";
    for (int n = stageStart[0]; n < stageStart[0]+stageNodes[0]; ++n)
        cout << "Node " << n << " -> Best Cost = " << minCost[n] << "\n";

    cout << "\nUpdated Optimal Path\n";
    cout << "----------------------------------------------------\n";

    curr = 0;
    routeCost = 0;
    cout << "Path: " << curr;

    while (nextNode[curr] != -1)
    {
        cout << " -> " << nextNode[curr];
        for (const Edge &e : graph[curr])
            if (e.to == nextNode[curr]) routeCost += e.cost;

        curr = nextNode[curr];
    }

    cout << "\nTotal Path Cost = " << routeCost << "\n";
    cout << "----------------------------------------------------\n";

    return 0;
}
